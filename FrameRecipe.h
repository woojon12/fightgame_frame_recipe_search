#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <set>

class FrameRecipe {
	int target_frame;

	std::map<int, std::vector<std::map<int, int>>> result_recipe; //��� ������ ��. 13�� ���� ��� 3�� 3��, 4�� 1���� �̷�����ִٴ� ������ ����
	//������ wstring ������ �б��۾��� ���� �ݵ�� �ʿ��� �Ʒ� character_moves�� ������ ���⼱ �������� ���ڸ� �����Ѵ�.
	std::map<int, std::vector<std::wstring>> character_moves; //�޸��� ������ �̾ƿ� ��. first(int)�� ����������, second(wstring)�� ������̴�
	std::map<int, std::vector<std::pair<int, int>>> bipartition;

	void recipe_find(int next_target);
	bool not_visited(int next_target);
	bool exist_recipe(int next_target);
	void read_memo(int, int, int);
	bool is_A_factor_of_B(int current_min_frame, int current_target);
	bool already_exist(std::vector<std::map<int, int>>::iterator candidate, std::vector<std::map<int, int>>& current_result_vector);

	int debug_pseudo_global_var;

public:
	FrameRecipe(int target_frame, std::wifstream& frame_data) : target_frame(target_frame) {
		if (frame_data.fail()) { std::wcout << "������ �����ϴ�\n" << std::endl; return; }

		std::wstring input;

		while (!frame_data.eof()) {
			getline(frame_data, input, wchar_t(L' ')); //�׳� L' '��� ���൵ �Ǵµ� wchar_t�� �־�� �ȴٴ� ���� ǥ���ϱ� ���� ����.
			int move_frame = stoi(input);

			getline(frame_data, input);
			//character_moves.push_back(move(input_frame, input));
			
			character_moves[move_frame].push_back(input);
			//���� map�� ��ó�� []�� ������� �ʴ´ٸ� �Ʒ� �ڵ�� ���� ����ؾ� �Ѵ�. ���� ���� �������� �ð�����.
			//character_moves.insert(std::pair<int, std::vector<std::wstring>>(move_frame, *new std::vector<std::wstring>));
			//character_moves.find(move_frame)->second.push_back(input);
		}

		//result_recipe[key][0][key] = 1 �̶�� ������ ä���־�� ��
		for (const std::pair<int, std::vector<std::wstring>> pairiv : character_moves) {
			result_recipe[pairiv.first].push_back(std::map<int, int>());
			result_recipe[pairiv.first][0][pairiv.first] = 1;

			//bipartition[pairiv.first].push_back({ 0, pairiv.first }); //�̰� ���ϸ� not_visited �Լ����� �湮 �ȵƴٰ� �νĵǼ� �ƹ� �ǹ̾��� �Ŷ� �ֱ�� ���� �� �־�� ��ƴ���� ����
		}
		//���� 0�� �����ǰ� �ִٰ� �����ؾ� �׳� 0�� 1�� �����Ѵٰ� �ۼ�(result_recipe[0][0][0] = 1)�� �س��� �� �۾��� �� 0�̸� ���ܽ�Ű��
		// �� ���� �� �Ʒ� 3�� ����� ���α׷� �� ���ư��� ����.
		//result_recipe[0].push_back(std::map<int, int>());
		//result_recipe[0][0][0] = 1;
		//bipartition[0].push_back({ 0, 0 });

		recipe_find(target_frame);
	}

	void debug_file();
	void debug_map_init();

	friend std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr);
	friend std::wostream& operator<<(std::wostream& o, FrameRecipe& fr);
private: template<typename Out> void outputting(Out& of, std::map<int, int>::iterator mitr);
};