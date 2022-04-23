#include "FrameRecipe.h"

void FrameRecipe::recipe_find(int current_target) {
	//TODO: �ִ�����(��ǥ���ּ�) ����ȭ, 
	
	std::map<int, std::vector<std::wstring>>::iterator mitr = character_moves.begin();

	for (; mitr != character_moves.end(); ++mitr) {
		//�ּ�ã��
		int current_min_frame = mitr->first; //���ʿ� map�̶� �ڷᱸ���� key�������� ���� �س��⿡ ù��°�� ���� ���� ��.

		if (current_min_frame > current_target) break;

		bool is_aliquot_part = is_A_factor_of_B(current_min_frame, current_target);

		for (int i = 0; true; ++i) {
			int one_of_next_targets1 = current_min_frame * i;
			int one_of_next_targets2 = current_target - one_of_next_targets1;

			if (one_of_next_targets2 < 0) break; //�Ʒ� ���Ǹ����� ��� �ƴϸ� �������� ������ ����
			if (one_of_next_targets2 < current_target / 2 && is_aliquot_part) break;

			bipartition[current_target].push_back({ one_of_next_targets1, one_of_next_targets2 });

			if (not_visited(one_of_next_targets1)) recipe_find(one_of_next_targets1);
			if (not_visited(one_of_next_targets2)) recipe_find(one_of_next_targets2);
			
			if (exist_recipe(one_of_next_targets1)
			&& exist_recipe(one_of_next_targets2))
				read_memo(current_target, one_of_next_targets1, one_of_next_targets2);
		}
	}
}

bool FrameRecipe::not_visited(int next_target)
{
	std::map<int, std::vector<std::pair<int, int>>>::iterator bipart_itr;
	bipart_itr = bipartition.find(next_target);

	if (bipart_itr == bipartition.end()) return true;

	return false;
}

bool FrameRecipe::exist_recipe(int next_target)
{
	std::map<int, std::vector<std::map<int, int>>>::iterator resitr;
	resitr = result_recipe.find(next_target);

	if (resitr == result_recipe.end()) return false;

	return true;
}

//result_recipe[13][0][4] = 1 �̶� �� 13�� ������ �� ù��°(�ε��� �� 0��°) �߰ߵ� �����Ǵ� 4�� 1�� ���ٴ� ��. ������ 3334�� �Ϻθ� ��Ÿ�� ��.
void FrameRecipe::read_memo(int current_target, int next_target1, int next_target2)
{
	std::vector<std::map<int, int>>& current_result_vector = result_recipe[current_target];

	for (const std::map<int, int>& res1 : result_recipe[next_target1])
		for (const std::map<int, int>& res2 : result_recipe[next_target2]) {
			current_result_vector.push_back(std::map<int, int>());
			std::vector<std::map<int, int>>::iterator candidate = --current_result_vector.end();

			//��°���� res1,2�� std::map<int, int>::iterator �� ���� �� ����. res1,2 �� const �������� �޾���. ����.

			if (next_target1 != 0) //0�� ��� next_target2�� ����ؾ� ��
				for (const std::pair<int, int>& p1 : res1) {
					(*candidate)[p1.first] += p1.second;
				}

			for (const std::pair<int, int>& p2 : res2) {
				(*candidate)[p2.first] += p2.second;
			}

			if (current_result_vector.size() > 1 && already_exist(candidate, current_result_vector))
				current_result_vector.erase(candidate);
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
		std::map<int, int>::iterator mitr;

		for (mitr = candidate->begin(); mitr != candidate->end(); ++mitr) {
			std::map<int, int>::iterator itr_of_pair_of__first = vitr->find(mitr->first);

			if (itr_of_pair_of__first == vitr->end()) break;
			if (itr_of_pair_of__first->second != mitr->second) break;
			//������������ 16 + 24 �� 16 + 11 + 13 �� �������� ���ߴ�. ���� 16�� 1�� �ִٴ� �� ���ٴ� ������.
		}
		if (mitr == candidate->end()) return true;
	}
	return false;
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

void FrameRecipe::debug_map_init()
{
	std::vector<std::map<int, int>> vm;	
	vm.push_back(std::map<int, int>());

	vm[0][1] = 2;

	std::cout << vm[0][1] << std::endl;
}

//(�����1(16) * 3) + (�����2, �����3)(17) + �����4(23)...
//Ư�� ���̽� �� ��ġ�� ((�����2, �����3)(17) * 2)
std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr)
{
	of << L"# " << fr.target_frame << L"\n\n";

	if (fr.result_recipe[fr.target_frame].size() == 0) {
		of << L"�߰ߵ� ������ ����\n\n";
		return of;
	}

	for (std::map<int, int>& one_of_results : fr.result_recipe[fr.target_frame]) {

		fr.outputting(of, one_of_results.begin());
		
		for (std::map<int, int>::iterator mitr = ++one_of_results.begin(); mitr != one_of_results.end(); ++mitr) {
			of << L" + ";
			fr.outputting(of, mitr);
		}

		of << L'\n';
	}

	return of;
}

std::wostream& operator<<(std::wostream& o, FrameRecipe& fr)
{
	o << L"# " << fr.target_frame << L"\n\n";

	if (fr.result_recipe[fr.target_frame].size() == 0) {
		o << L"�߰ߵ� ������ ����\n\n";
		return o;
	}

	for (std::map<int, int>& one_of_results : fr.result_recipe[fr.target_frame]) {

		fr.outputting(o, one_of_results.begin());

		for (std::map<int, int>::iterator mitr = ++one_of_results.begin(); mitr != one_of_results.end(); ++mitr) {
			o << L" + ";
			fr.outputting(o, mitr);
		}

		o << L'\n';
	}

	return o;
}

//�� ���ݱ��� ���ø��� ���� �ʹ� �Ҿ��ߴµ� (����ڵ��� ����ġ ���� �Ű������� �ѱ�� ����� �ϰ�)
//�������ϱ� ����ڵ��� ���� �ϴ� �� �ƴ϶�
//���ø��� ���ο����� �Ἥ ���� �� ���� �ǳ�?
//���� ���ĸ� ���� operator<< �Լ����� wofstream, wostream �� �͸� �ٸ��� �� ��������
//����ڰ� ���� �Լ��ϱ� ���ø� �� �Ἥ �����߰� ������ ���ο����� �˾Ƽ� ���ø� ����� �Լ� ����
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