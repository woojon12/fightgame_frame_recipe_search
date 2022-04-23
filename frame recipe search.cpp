#include <locale>
#include "FrameRecipe.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
-프로그램의 목적 : 격투게임에서 흔히 상대를 다운시키고
정확하게 공격을 깔기 위해 일부러 특정행동을 하여 타이밍을 맞추는데
이 특정행동의 조합을 일명 '레시피' 라고 한다.

어떤 캐릭터의 모든 행동의 프레임과
원하는 레시피 프레임이
입력으로 주어지면
해당 레시피 프레임을 만족시키는 모든 행동의 조합을 출력한다


-기능 : 모든 행동 프레임 메모장을 준비한 뒤 원하는 프레임을 입력 시 레시피를 출력 및 새로운 메모장에 저장
만약 원하는 프레임이 여러 개인 경우도 지원. 20~40 까지 모든 레시피를 요구할 수 있음.


-개발 아이디어
1 : 만약 행동을 계속 조합해가다가 앞으로 남은 프레임이 A일 때
이전에 A일때를 경험해봤다면 그 A를 만족시킨 행동을 당시 저장해뒀다 다시 꺼내서 붙이는 방식으로 해볼 거.
이런 걸 아마 메모이제이션(memoization)이라 하는 듯.

2 : 그리고 만약 할 수만 있다면 저장해두는 메모리를 10000개 정도로 제한하고
더 이상 쓸모 없다고 판단되는 데이터는 버려서 메모리 자리를 비우기도 해보고 싶긴 함

3 : 그리고 내가 만든 걸 다른 사람이 가져다가 쓸 수도 있도록 만드는 컨셉도 경험해봐야 하니깐
클래스(여기서는 FrameRecipe) 하나에 생성자에 원하는 목표 프레임 넣으면
내부에서 알아서 모든 레시피를 다 알아낸 뒤
사용자(=내 코드 가져간 다른 개발자)는 그걸 메소드 하나만 호출하면 바로 모두 출력시킬 수 있도록 제작

난관1 : 결국 총합만 맞으면 되니까
수행동작 순서가 바뀌어도 되는건데
그럼 동작만 3개가 되도 3팩토리얼로 6가지의 중복이 나타난다.
이 모든 중복을 하나로 인식할 방법, 즉 중복을 제할 방법은 무엇인가?
예를 들어 목표가 10이고 2,3,5가 나오면 2,5,3 이나 3,2,5 등을 하나로 보는 방법은?
#(후보) : 그냥 아무렇게나 다 찾고 마지막에 중복을 탐색해서 제거
#(탈락) : 다른 레시피가 있나 찾으려 들 때 동작 하나를 잡고 탐색 시작하는데 그 동작이 이전에 사용된 적 있다면 제외
#(탈락) : 순서관련없이 존재만을 확인하는 set은 어떨까.
#(합격)set으로 해버리면 9를 333으로 나눌 수 없다. 그러니 같은 게 여러 개 일순 있돼 그 개수가 같다면 같은 거. AAAAAE 든 AAEAAA 든 둘 다 A는 5개이고 E는 1개이므로 같은 거.

주의사항 : set<>의 꺽쇠 안에 들어가는 타입은 operator< 가 반드시 있어야 함.
게다가 만약 두 집합(set)이 완전히 같은가? 를 위한 set == set 을 하려면 operator==도 반드시 있어야 함.
그리고 만약 operator< 에서 멤버 변수 하나만 비교하면 다른 멤버변수가 다르더라도 같은 걸로 인식해서 insert 되지 않음.
(신기한 게 set이 operator< 하나만으로도 insert 할 때 한정으로 ==를 할 줄 앎.)
그래서 operator< 에서 웬만하면 모든 멤버 변수의 대소 비교를 해줘야 한다.

난관2 : 목표가 10인데 이걸 (0,10) (1,9) (2,8) ... (8,2) (9,1) (10,0) 이렇게 1씩 분리해가면서 하다가 또
(1,9)에선 9를 또 (1,8)로 나누고 반복하면 재귀함수가 넘나 많아질 거 같은데
#그래서 모든 동작 중 가장 작은 프레임으로 깎아가면서 찾기
#그리고 목표프레임를 최소동작으로 정수 나누기 해서 나오는 수가 이론상 최대 조합 가능한 동작 수임. 그걸 토대로 탐색 범위 더 좁히기.
#그리고 (1,9)를 찾나 (9,1)을 찾나 같은 거니 대칭관계는 탐색 안 하기

난관3 : 같은 프레임을 보유한 다른 동작은 어떻게 처리해야 하는가?
#(탈락)사용자에게 같은 프레임의 동작은 한 줄 내에 다 같이 써달라고 부탁하거나
#(합격)그냥 사용자는 되는대로 써도 컴퓨터가 읽어낼 때 프레임 같은 건 하나로 인식하도록 정리하거나

난관4 : 메모 하는 방법(메모이제이션)을 어떻게 할 것인가?
예를 들어 9를 메모할 때 333 조합이 나올텐데 이 조합을 출력하는 것을
처음부터 3,3,3 으로 저장해둘지 3,6으로 저장했다가 6의 메모를 꺼내와서 합칠지.
#(전자)더 분할 불가할 때까지 최소치로 계속 분할하는 식? 최소3 타겟10이면 (10) (3, 10-3=7) (3, 3, 10-3-3=4) 이렇게
#(후자)무조건 분할을 두 개로만 하고 최소치를 계속 더해가는 식? 최소가 3이고 타겟이 10이면 (0, 10) (3, 7) (3+3=6, 4) (3+3+3=9, 1) 이렇게
#3,3,3 저장(전자)는 뭔가 중복 저장의 느낌이 있어서 그냥 3,6(후자)로 하자. 그리고 후자 쪽이 대칭성 중복을 인식하기도 쉬울 듯.
#근데 333은 존재하는 프레임으로만 모든 요소가 구성되는데 3,6은 6이 무존재일 수도 있음. 그래서 후자는 읽을 때 중복이 일어남.
#그래서 알고리즘 동작할 땐 후자로 가다가 마지막에 결과 정리할 때 전자로 변환해야 할 듯
#아 만들다보니 알았는데 무조건 전자로 해야함. 안 그럼 메모의 의미도 없고 메모할 수도 없음. 아마? 정확한 건 좀 더 생각해봐야함
#(합격)탐색시작할 때는 두 개만 취급하다 탐색결과 되돌려받을 때는 세 개 이상 저장하는 방식.
#그리고 이 숫자는 이미 탐색을 했고 그래서 레시피가 존재하는지 안하는지를 저장하는 0,1,2만 저장되는 변수가 있으면 좋을 듯
#근데 그러면 조금이라도 덜 직관적이 되고 저장할 변수도 아무리 작게 잡아봤자 char(1byte=8bit)이 되는데 정확히 2bit만 사용하는 bool 두 개가 낫지 않을까

참고 사이트 :
https://programming119.tistory.com/105 gitignore 사용법

장장 4시간의 사투 끝에 .gitignore 은
반드시 git bash 상에서 touch 명령어로 생성해야 된다는 걸 알았음. 아님 그냥 메모장으로 .gitignore.txt 생성하고 .txt를 지워버리면 됨.
https://kiros33.tistory.com/entry/%EC%9C%88%EB%8F%84%EC%9A%B0%EC%97%90%EC%84%9C-touch-%EB%AA%85%EB%A0%B9%EC%9D%84-%EB%8C%80%EC%B2%B4%ED%95%98%EB%8A%94-%EB%B0%A9%EB%B2%95
이 사이트에 나오는 것 처럼 그냥 윈도우 cmd에서 파일 생성하는 방법으로 gitignore을 만들면 죽었다 깨어나도 작동 안 함.


*/

void input_request(int& input) {
	do {
		if (wcin.fail()) {
			wcout << L"숫자를 입력하시오\n\n >> ";
			wcin.clear();
			wcin.ignore(100, '\n');
		}
	} while (!(wcin >> input));
}

int main()
{
	locale::global(locale("ko_KR.UTF-8"));
	int target_frame;
	wifstream frame_data("frame_data.txt");
	FrameRecipe FR;

	wcout << L"이 프로그램을 사용하기 위해서는 exe파일과 같은 경로(폴더)에 frame_data.txt 파일이 존재해야 합니다\n\n";
	wcout << L"다음 항목에서 선택하시오\n\n 0. 사용 시 유의사항 읽어보기\n 1. 원하는 프레임 단순 입력\n 2. 후상황과 깔아둘 공격의 발동과 지속 입력(해당 공격이 최대한 깔리게 하는 프레임을 알아서 계산하여 레시피를 구합니다)\n\n (이외 숫자는 프로그램 종료)\n\n >> ";

	input_request(target_frame);

	switch (target_frame) {
	case 0:
		wcout << L"\tA. frame_data.txt 를 작성할 때는 각 줄마다\n\t(기술의 동작 프레임)(띄어쓰기 한 번)(기술명) 양식으로 작성해주십시오.(ex: 51 약 승룡)\n\n";
		wcout << L"\tB. 연타 가능 기본기의 경우 두 번 연속 사용 시\n\t단순히 한 번 사용했을 때 프레임의 두 배가 되는 게 아니기 때문에\n\t결과에 (약손 * 2) 같은 게 낀 레시피는 걸러야 합니다.\n\t(연타 했을 경우 결과 프레임이 어떻게 다른지 모두 알고 frame_data.txt에 작성할 수 있다면 보완 가능)\n\n";
		wcout << L"\tC. 상대랑 거리가 많이 벌어지는 후상황이라면\n\t레시피 상에 앞대쉬와 같은 전진성 있는 동작이 잘 껴있는 것만 실전에서 사용할 수 있음에 유의하십시오\n\n";
		wcout << L"\tD. 만약 +87의 후상황에서 발동이 22이고 지속이 3인 기술을 깔고 싶다면\n\t87 - 22 - 3 + 2 = 64의 동작 레시피를 따른 후 마지막에 그 기술을 쓰면 깔리게 됩니다\n\t해당 계산을 돕기 위해 2번 항목을 만들어두었습니다\n\n";
		wcout << L"\tE. 당연하지만 한 레시피 내의 동작 순서가 바뀌더라도 같은 레시피이므로\n\t본인이 입력하기 편한 순서로 바꿔서 사용하시는 게 좋습니다\n\n";
		system("pause");
		return 0;
		break;

	case 1:
		wcout << L"\n원하는 레시피 프레임을 입력하시오\n >> ";
		input_request(target_frame);
		FR = FrameRecipe(target_frame, frame_data);
		break;
	case 2:
		int oki_frame, startup_frame, active_frame;
		wcout << L"\n띄어쓰기로 구분하여 '후상황 프레임', '깔아둘 공격의 발동 프레임', '깔아둘 공격의 지속 프레임' 순으로 입력해주십시오\n >> ";
		input_request(oki_frame);
		input_request(startup_frame);
		input_request(active_frame);
		//FR = FrameRecipe(oki_frame, startup_frame, active_frame, frame_data);
		target_frame = oki_frame - startup_frame - active_frame + 2;
		FR = FrameRecipe(target_frame, frame_data);
		break;
	default:
		return 0;
	}
	
	wstring output_filename(L"recipe ");
	output_filename.append(to_wstring(target_frame));
	output_filename.append(L".txt");	
	wofstream output(output_filename);

	output << FR;
	wcout << '\n' << FR << endl;
	
	wcout << L"위 내용은 " << output_filename << L" 라는 이름의 메모장에 기록되었습니다\n" << endl; 
	system("pause");

	//update : 후상황 프레임과 깔아둘 공격의 발동과 지속을 적으면 알아서 최대로 깔아두는 프레임 계산해주고 돌리기 (계산식 : 후상황 - 발동 - 지속 + 2)
	//last TODO : 연타 가능 기본기의 경우 연타 시 프레임이 달라져 두 번 연속 사용하는 레시피를 따를 수 없으므로 그것도 고려한 신들린 알고리즘 만들기. 이건 솔직히 귀찮음.
}