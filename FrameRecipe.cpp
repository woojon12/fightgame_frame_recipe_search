#include "FrameRecipe.h"

//시간을 많이 투자한다고 햇을 때 안도가 생기지 않으면 온전히 집중하지 않고 그 시간을 보내겠다는 뜻.
//시간을 최대로 쥐어짜면 안도할 수 있을만큼 집중해라. (시간이 없다해도 만약 있게 할 수 있다면이란 가정을 하고)
//너가 집중을 못하는 인간이라면 시간을 극도로 쥐어짜도 안도하지 못할 것. 그 시간을 무쓸모로 보낼 거기에.

void FrameRecipe::recipe_find(int current_target) {
	//TODO: 최대조합(목표÷최소) 최적화, 
	//DONE: 대칭 최적화

	if (current_target == 0) return;
	
	std::map<int, std::vector<std::wstring>>::iterator mitr = character_moves.begin();
	min_frame = mitr->first;

	for (; mitr != character_moves.end(); ++mitr) {
		//최소찾기
		int current_min_frame = mitr->first; //애초에 map이란 자료구조가 key오름차순 정렬 해놓기에 첫번째가 제일 작은 놈.
		//근데 잠만 찐 최소는 0 아닌가. 9를 분할하기 전에 0과 9로 분할아닌 분할을 해야 되지 않나
		if (current_min_frame > target_frame) break;

		for (int i = 0; true; ++i) {
			int one_of_next_targets1 = current_min_frame * i;
			int one_of_next_targets2 = current_target - one_of_next_targets1;

			if (one_of_next_targets2 < current_target / 2) //대칭중복 방지 및 종료조건
				break;

			bipartition[current_target].push_back({ one_of_next_targets1, one_of_next_targets2 });

			if (not_visited(one_of_next_targets1)) recipe_find(one_of_next_targets1);
			if (not_visited(one_of_next_targets2)) recipe_find(one_of_next_targets2);
			
			if (exist_recipe(one_of_next_targets1)
			&& exist_recipe(one_of_next_targets2))
				read_memo(current_target, one_of_next_targets1, one_of_next_targets2);
		}
	}
	//'목표 마이너스 최소'를 재귀할건데
		//그러기 전에 이미 메모이제이션 된 게 있으면 꺼내쓰기

	//재귀완료된 건 메모이제이션(저장)

	//다음 최소를 찾고 반복. 다음 최소가 타겟보다 커지거나 모든 무브를 방문할 때까지.
}

bool FrameRecipe::not_visited(int next_target)
{
	std::map<int, std::vector<std::pair<int, int>>>::iterator memoitr;
	memoitr = bipartition.find(next_target);

	if (next_target <= min_frame) return false;

	if (memoitr == bipartition.end()) return true;

	return false;
}

bool FrameRecipe::exist_recipe(int next_target)
{
	std::map<int, std::map<int, int>>::iterator resitr;
	resitr = result_recipe.find(next_target);

	if (next_target == min_frame) return true;

	if (resitr == result_recipe.end()) return false;

	return true;
}

bool FrameRecipe::read_memo(int current_target, int next_target1, int next_target2)
{
	std::map<int, int>::iterator resitr = result_recipe[next_target1].begin(); resitr != result_recipe[next_target1].end(); ++resitr;

	result_recipe[current_target][resitr->first] += resitr->second;
}

void FrameRecipe::debug_file()
{
	for (const std::pair<int, std::vector<std::wstring>>& pivw : character_moves) {
		size_t size = pivw.second.size();
		std::wcout << pivw.first << ' ' << pivw.second[0];
		for (int i = 1; i < size; ++i) {
			std::wcout << ", " << pivw.second[i];
		}
		std::cout << std::endl;
	}
}

void FrameRecipe::debug_set()
{
	std::cout << "max_size : " << character_moves.max_size() << std::endl;
}


//(기술명1(16) * 3) + (기술명2, 기술명3)(17) + 기술명4(23)...
//특이 케이스 다 합치면 ((기술명2, 기술명3)(17) * 2)
std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr)
{
	of << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe.size() == 0) {
		of << "발견된 레시피 없음\n\n";
		return of;
	}

	for (const std::pair<int, std::map<int,int>>& si : fr.result_recipe) {
		int size = si.size();
		int repeat = 0;
		of << '(' << *si.begin() << ')' << fr.character_moves[*si.begin()][repeat++];
		for (std::set<int>::iterator sitr = ++si.begin(); sitr != si.end(); ++sitr) {
			of << " + " << '(' << *sitr << ')' << fr.character_moves[*sitr][repeat++];
		}
		of << '\n';
	}

	of << '\n';

	return of;
}

std::wostream& operator<<(std::wostream& o, FrameRecipe& fr)
{
	o << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe.size() == 0) {
		o << "발견된 레시피 없음\n\n";
		return o;
	}

	for (const std::set<int>& si : fr.result_recipe) {
		int size = si.size();
		int repeat = 0;
		o << '(' << *si.begin() << ')' << fr.character_moves[*si.begin()][repeat++];
		for (std::set<int>::iterator sitr = ++si.begin(); sitr != si.end(); ++sitr) {
			o << " + " << '(' << *sitr << ')' << fr.character_moves[*sitr][repeat++];
		}
		o << '\n';
	}

	o << '\n';

	return o;
}