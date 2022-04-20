#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <set>

class FrameRecipe {
	int target_frame;

	static std::map<int, std::map<std::pair<int, int>, std::map<int, int>>> result_recipe; //��� ������ ��. 13�� ���� ��� 3�� 3��, 4�� 1���� �̷�����ִٴ� ������ ����
	//������ wstring ������ �б��۾��� ���� �ݵ�� �ʿ��� �Ʒ� character_moves�� ������ ���⼱ �������� ���ڸ� �����Ѵ�.
	std::map<int, std::vector<std::wstring>> character_moves; //�޸��� ������ �̾ƿ� ��. first(int)�� ����������, second(wstring)�� ������̴�
	static std::map<int, std::vector<std::pair<int, int>>> bipartition;

	void recipe_find(int next_target);
	bool not_visited(int next_target);
	bool exist_recipe(int next_target);
		int min_frame;
	bool read_memo(int, int, int);
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

		//recipe_find_start();
	}

	void debug_file();
	void debug_set();
	friend std::wofstream& operator<<(std::wofstream& of, FrameRecipe& fr);
	friend std::wostream& operator<<(std::wostream& o, FrameRecipe& fr);
};