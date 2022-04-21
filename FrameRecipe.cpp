#include "FrameRecipe.h"

//시간을 많이 투자한다고 햇을 때 안도가 생기지 않으면 온전히 집중하지 않고 그 시간을 보내겠다는 뜻.
//시간을 최대로 쥐어짜면 안도할 수 있을만큼 집중해라. (시간이 없다해도 만약 있게 할 수 있다면이란 가정을 하고)
//너가 집중을 못하는 인간이라면 시간을 극도로 쥐어짜도 안도하지 못할 것. 그 시간을 무쓸모로 보낼 거기에.

void FrameRecipe::recipe_find(int current_target) {
	//TODO: 최대조합(목표÷최소) 최적화, 
	//DOING: 대칭 최적화

	if (current_target == 0) return;
	
	std::map<int, std::vector<std::wstring>>::iterator mitr = character_moves.begin();

	for (; mitr != character_moves.end(); ++mitr) {
		//최소찾기
		int current_min_frame = mitr->first; //애초에 map이란 자료구조가 key오름차순 정렬 해놓기에 첫번째가 제일 작은 놈.
		//근데 잠만 찐 최소는 0 아닌가. 9를 분할하기 전에 0과 9로 분할아닌 분할을 해야 되지 않나 (Done)
		if (current_min_frame > current_target) break;

		bool is_aliquot_part = is_A_factor_of_B(current_min_frame, current_target);

		for (int i = 0; true; ++i) {
			int one_of_next_targets1 = current_min_frame * i;
			int one_of_next_targets2 = current_target - one_of_next_targets1;

			if (one_of_next_targets2 < current_target / 2 && is_aliquot_part) //대칭중복 방지 및 종료조건 //DONE : current_min_frame 이 current_target의 약수여야 대칭이 발생함
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

	//다음 최소를 찾고 반복. 다음 최소가 타겟보다 커지거나 모든 무브를 방문할 때까지. (Done)
}

bool FrameRecipe::not_visited(int next_target)
{
	std::map<int, std::vector<std::pair<int, int>>>::iterator memoitr;
	memoitr = bipartition.find(next_target);

	if (memoitr == bipartition.end()) return true;

	return false;
}

bool FrameRecipe::exist_recipe(int next_target)
{
	std::map<int, std::vector<std::map<int, int>>>::iterator resitr;
	resitr = result_recipe.find(next_target);

	if (resitr == result_recipe.end()) return false;

	return true;
}

//result_recipe[13][0][4] = 1 이란 건 13의 레시피 중 첫번째(인덱스 상 0번째) 발견된 레시피는 4가 1번 들어간다는 뜻. 레시피 3334의 일부를 나타낸 거.
void FrameRecipe::read_memo(int current_target, int next_target1, int next_target2)
{
	std::vector<std::map<int, int>>& current_result_vector = result_recipe[current_target];
	std::vector<std::map<int, int>>::iterator candidate = --current_result_vector.end();

	current_result_vector.push_back(*new std::map<int, int>);

	for (const std::map<int, int>& res1 : result_recipe[next_target1])
		for (const std::map<int, int>& res2 : result_recipe[next_target2]) {
			//어째선지 res1,2를 std::map<int, int>::iterator 로 받을 수 없다. const 빼버리면 받아짐. 뭐지.

			if (next_target1 != 0) //0인 경우 next_target2만 고려해야 됨
				for (const std::pair<int, int>& p1 : res1) {
					(*candidate)[p1.first] += p1.second;
				}

			for (const std::pair<int, int>& p2 : res2) {
				(*candidate)[p2.first] += p2.second;
			}

			//DONE : 같은 조합이 있는지 확인할 것. 있으면 이 파트는 result_recipe[current_target].erase(--생략.end())로 삭제
			if (already_exist(candidate, current_result_vector)) current_result_vector.erase(--current_result_vector.end());
		}
}

bool FrameRecipe::is_A_factor_of_B(int current_min_frame, int current_target)
{
	if (current_target % current_min_frame == 0) return true;

	return false;
}

bool FrameRecipe::already_exist(std::vector<std::map<int, int>>::iterator candidate, std::vector<std::map<int, int>>& current_result_vector)
{
	for (std::vector<std::map<int, int>>::iterator vitr = current_result_vector.begin(); vitr != --current_result_vector.end(); ++vitr) {
		for (std::map<int, int>::iterator mitr = candidate->begin(); mitr != candidate->end(); ++mitr) {
			std::map<int, int>::iterator first_of_stdpair = vitr->find(mitr->first);
			if (vitr->find(mitr->first) == vitr->end()) return false;
			if (vitr->find(mitr->first)->second != mitr->second) return false;
		}
	}
	return true;
}

void FrameRecipe::debug_file()
{
	for (const std::pair<int, std::vector<std::wstring>>& pivw : character_moves) {
		size_t size = pivw.second.size();
		std::wcout << pivw.first << ' ' << pivw.second[0];
		for (size_t i = 1; i < size; ++i) {
			std::wcout << ", " << pivw.second[i];
		}
		std::cout << std::endl;
	}
}

//(기술명1(16) * 3) + (기술명2, 기술명3)(17) + 기술명4(23)...
//특이 케이스 다 합치면 ((기술명2, 기술명3)(17) * 2)
std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr)
{
	of << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe[fr.target_frame].size() == 0) {
		of << L"발견된 레시피 없음\n\n";
		return of;
	}

	for (std::map<int, int>& one_of_results : fr.result_recipe[fr.target_frame]) {

		fr.outputting(of, one_of_results.begin());
		
		for (std::map<int, int>::iterator mitr = ++one_of_results.begin(); mitr != one_of_results.end(); ++mitr) {
			of << " + ";
			fr.outputting(of, mitr);
		}
	}

	//for (const std::pair<int, std::vector<std::map<int,int>>>& pvm : fr.result_recipe) {
	//	int size = pvm.size();
	//	int repeat = 0;
	//	of << '(' << *pvm.begin() << ')' << fr.character_moves[*pvm.begin()][repeat++];
	//	for (std::set<int>::iterator sitr = ++pvm.begin(); sitr != pvm.end(); ++sitr) {
	//		of << " + " << '(' << *sitr << ')' << fr.character_moves[*sitr][repeat++];
	//	}
	//	of << '\n';
	//}

	of << '\n';

	return of;
}

std::wostream& operator<<(std::wostream& o, FrameRecipe& fr)
{
	o << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe[fr.target_frame].size() == 0) {
		o << L"발견된 레시피 없음\n\n";
		return o;
	}

	for (std::map<int, int>& one_of_results : fr.result_recipe[fr.target_frame]) {

		fr.outputting(o, one_of_results.begin());

		for (std::map<int, int>::iterator mitr = ++one_of_results.begin(); mitr != one_of_results.end(); ++mitr) {
			o << L" + ";
			fr.outputting(o, mitr);
		}
	}

	o << '\n';

	return o;
}

//나 지금까지 템플릿에 대해 너무 불안했는데 (사용자들이 예상치 못한 매개변수를 넘기면 어떡하지 하고)
//이제보니까 사용자들이 쓰게 하는 게 아니라
//템플릿을 내부에서만 써서 나만 잘 쓰면 되네?
//무슨 얘기냐면 위에 operator<< 함수보면 wofstream, wostream 인 것만 다르고 다 같은데도
//사용자가 쓰는 함수니까 템플릿 안 써서 구현했고 오로지 내부에서만 알아서 템플릿 적용된 함수 쓴다
template<typename Out>
void FrameRecipe::outputting(Out& out, std::map<int, int>::iterator mitr)
{
	if (mitr->second > 1) out << '(';

	if (character_moves[mitr->first].size() > 1) out << '(';
	out << character_moves[mitr->first][0];
	for (std::vector<std::wstring>::iterator vitr = ++character_moves[mitr->first].begin(); vitr != character_moves[mitr->first].end(); ++vitr)
		out << ", " << *vitr;
	if (character_moves[mitr->first].size() > 1) out << ')';

	out << '(' << mitr->first << ')';
	if (mitr->second > 1) out << " * " << mitr->second << ')';
}