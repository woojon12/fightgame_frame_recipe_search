#include "FrameRecipe.h"

//�ð��� ���� �����Ѵٰ� ���� �� �ȵ��� ������ ������ ������ �������� �ʰ� �� �ð��� �����ڴٴ� ��.
//�ð��� �ִ�� ���¥�� �ȵ��� �� ������ŭ �����ض�. (�ð��� �����ص� ���� �ְ� �� �� �ִٸ��̶� ������ �ϰ�)
//�ʰ� ������ ���ϴ� �ΰ��̶�� �ð��� �ص��� ���¥�� �ȵ����� ���� ��. �� �ð��� ������� ���� �ű⿡.

void FrameRecipe::recipe_find(int current_target) {
	//TODO: �ִ�����(��ǥ���ּ�) ����ȭ, 
	//DONE: ��Ī ����ȭ

	if (current_target == 0) return;
	
	std::map<int, std::vector<std::wstring>>::iterator mitr = character_moves.begin();
	min_frame = mitr->first;

	for (; mitr != character_moves.end(); ++mitr) {
		//�ּ�ã��
		int current_min_frame = mitr->first; //���ʿ� map�̶� �ڷᱸ���� key�������� ���� �س��⿡ ù��°�� ���� ���� ��.
		//�ٵ� �Ḹ �� �ּҴ� 0 �ƴѰ�. 9�� �����ϱ� ���� 0�� 9�� ���Ҿƴ� ������ �ؾ� ���� �ʳ�
		if (current_min_frame > target_frame) break;

		for (int i = 0; true; ++i) {
			int one_of_next_targets1 = current_min_frame * i;
			int one_of_next_targets2 = current_target - one_of_next_targets1;

			if (one_of_next_targets2 < current_target / 2) //��Ī�ߺ� ���� �� ��������
				break;

			bipartition[current_target].push_back({ one_of_next_targets1, one_of_next_targets2 });

			if (not_visited(one_of_next_targets1)) recipe_find(one_of_next_targets1);
			if (not_visited(one_of_next_targets2)) recipe_find(one_of_next_targets2);
			
			if (exist_recipe(one_of_next_targets1)
			&& exist_recipe(one_of_next_targets2))
				read_memo(current_target, one_of_next_targets1, one_of_next_targets2);
		}
	}
	//'��ǥ ���̳ʽ� �ּ�'�� ����Ұǵ�
		//�׷��� ���� �̹� �޸������̼� �� �� ������ ��������

	//��ͿϷ�� �� �޸������̼�(����)

	//���� �ּҸ� ã�� �ݺ�. ���� �ּҰ� Ÿ�ٺ��� Ŀ���ų� ��� ���긦 �湮�� ������.
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


//(�����1(16) * 3) + (�����2, �����3)(17) + �����4(23)...
//Ư�� ���̽� �� ��ġ�� ((�����2, �����3)(17) * 2)
std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr)
{
	of << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe.size() == 0) {
		of << "�߰ߵ� ������ ����\n\n";
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
		o << "�߰ߵ� ������ ����\n\n";
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