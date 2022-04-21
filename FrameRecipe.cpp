#include "FrameRecipe.h"

//�ð��� ���� �����Ѵٰ� ���� �� �ȵ��� ������ ������ ������ �������� �ʰ� �� �ð��� �����ڴٴ� ��.
//�ð��� �ִ�� ���¥�� �ȵ��� �� ������ŭ �����ض�. (�ð��� �����ص� ���� �ְ� �� �� �ִٸ��̶� ������ �ϰ�)
//�ʰ� ������ ���ϴ� �ΰ��̶�� �ð��� �ص��� ���¥�� �ȵ����� ���� ��. �� �ð��� ������� ���� �ű⿡.

void FrameRecipe::recipe_find(int current_target) {
	//TODO: �ִ�����(��ǥ���ּ�) ����ȭ, 
	//DOING: ��Ī ����ȭ

	if (current_target == 0) return;
	
	std::map<int, std::vector<std::wstring>>::iterator mitr = character_moves.begin();

	for (; mitr != character_moves.end(); ++mitr) {
		//�ּ�ã��
		int current_min_frame = mitr->first; //���ʿ� map�̶� �ڷᱸ���� key�������� ���� �س��⿡ ù��°�� ���� ���� ��.
		//�ٵ� �Ḹ �� �ּҴ� 0 �ƴѰ�. 9�� �����ϱ� ���� 0�� 9�� ���Ҿƴ� ������ �ؾ� ���� �ʳ� (Done)
		if (current_min_frame > current_target) break;

		bool is_aliquot_part = is_A_factor_of_B(current_min_frame, current_target);

		for (int i = 0; true; ++i) {
			int one_of_next_targets1 = current_min_frame * i;
			int one_of_next_targets2 = current_target - one_of_next_targets1;

			if (one_of_next_targets2 < current_target / 2 && is_aliquot_part) //��Ī�ߺ� ���� �� �������� //DONE : current_min_frame �� current_target�� ������� ��Ī�� �߻���
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

	//���� �ּҸ� ã�� �ݺ�. ���� �ּҰ� Ÿ�ٺ��� Ŀ���ų� ��� ���긦 �湮�� ������. (Done)
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

//result_recipe[13][0][4] = 1 �̶� �� 13�� ������ �� ù��°(�ε��� �� 0��°) �߰ߵ� �����Ǵ� 4�� 1�� ���ٴ� ��. ������ 3334�� �Ϻθ� ��Ÿ�� ��.
void FrameRecipe::read_memo(int current_target, int next_target1, int next_target2)
{
	std::vector<std::map<int, int>>& current_result_vector = result_recipe[current_target];
	std::vector<std::map<int, int>>::iterator candidate = --current_result_vector.end();

	current_result_vector.push_back(*new std::map<int, int>);

	for (const std::map<int, int>& res1 : result_recipe[next_target1])
		for (const std::map<int, int>& res2 : result_recipe[next_target2]) {
			//��°���� res1,2�� std::map<int, int>::iterator �� ���� �� ����. const �������� �޾���. ����.

			if (next_target1 != 0) //0�� ��� next_target2�� ����ؾ� ��
				for (const std::pair<int, int>& p1 : res1) {
					(*candidate)[p1.first] += p1.second;
				}

			for (const std::pair<int, int>& p2 : res2) {
				(*candidate)[p2.first] += p2.second;
			}

			//DONE : ���� ������ �ִ��� Ȯ���� ��. ������ �� ��Ʈ�� result_recipe[current_target].erase(--����.end())�� ����
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

//(�����1(16) * 3) + (�����2, �����3)(17) + �����4(23)...
//Ư�� ���̽� �� ��ġ�� ((�����2, �����3)(17) * 2)
std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr)
{
	of << "#" << fr.target_frame << std::endl;

	if (fr.result_recipe[fr.target_frame].size() == 0) {
		of << L"�߰ߵ� ������ ����\n\n";
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
		o << L"�߰ߵ� ������ ����\n\n";
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