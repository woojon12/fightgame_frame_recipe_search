#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <set>

class FrameRecipe {
	int target_frame;

	static std::map<int, std::map<std::pair<int, int>, std::map<int, int>>> result_recipe; //결과 저장할 곳. 13은 예를 들어 3이 3개, 4가 1개로 이루어져있다는 식으로 저장
	//어차피 wstring 정보는 읽기작업을 위해 반드시 필요한 아래 character_moves가 있으니 여기선 레시피의 숫자만 저장한다.
	std::map<int, std::vector<std::wstring>> character_moves; //메모장 데이터 뽑아올 곳. first(int)는 동작프레임, second(wstring)는 기술명이다
	static std::map<int, std::vector<std::pair<int, int>>> bipartition;

	void recipe_find(int next_target);
	bool not_visited(int next_target);
	bool exist_recipe(int next_target);
		int min_frame;
	bool read_memo(int, int, int);
public:
	FrameRecipe(int target_frame, std::wifstream& frame_data) : target_frame(target_frame) {
		if (frame_data.fail()) { std::wcout << "파일이 없습니다\n" << std::endl; return; }

		std::wstring input;

		while (!frame_data.eof()) {
			getline(frame_data, input, wchar_t(L' ')); //그냥 L' '라고만 해줘도 되는데 wchar_t로 넣어야 된다는 점을 표시하기 위해 적음.
			int move_frame = stoi(input);

			getline(frame_data, input);
			//character_moves.push_back(move(input_frame, input));
			
			character_moves[move_frame].push_back(input);
			//만약 map을 이처럼 []로 사용하지 않는다면 아래 코드와 같이 사용해야 한다. 정말 구린 가독성과 시간낭비.
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