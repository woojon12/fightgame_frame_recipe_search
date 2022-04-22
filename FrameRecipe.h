#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <set>

class FrameRecipe {
	int target_frame;

	std::map<int, std::vector<std::map<int, int>>> result_recipe; //결과 저장할 곳. 13은 예를 들어 3이 3개, 4가 1개로 이루어져있다는 식으로 저장
	//어차피 wstring 정보는 읽기작업을 위해 반드시 필요한 아래 character_moves가 있으니 여기선 레시피의 숫자만 저장한다.
	std::map<int, std::vector<std::wstring>> character_moves; //메모장 데이터 뽑아올 곳. first(int)는 동작프레임, second(wstring)는 기술명이다
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

		//result_recipe[key][0][key] = 1 이라는 끝값을 채워넣어야 됨
		for (const std::pair<int, std::vector<std::wstring>> pairiv : character_moves) {
			result_recipe[pairiv.first].push_back(std::map<int, int>());
			result_recipe[pairiv.first][0][pairiv.first] = 1;

			//bipartition[pairiv.first].push_back({ 0, pairiv.first }); //이거 안하면 not_visited 함수에서 방문 안됐다고 인식되서 아무 의미없는 거라도 넣기는 개뿔 안 넣어야 빈틈없이 동작
		}
		//또한 0도 레시피가 있다고 인정해야 그냥 0을 1개 보유한다고 작성(result_recipe[0][0][0] = 1)은 해놓고 비교 작업할 땐 0이면 제외시키자
		// 는 개뿔 이 아래 3줄 없어야 프로그램 잘 돌아가네 뭐지.
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