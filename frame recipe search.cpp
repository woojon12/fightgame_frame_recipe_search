#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
#include "FrameRecipe.h"
using namespace std;

/*
-���α׷��� ���� : �������ӿ��� ���� ��븦 �ٿ��Ű��
��Ȯ�ϰ� ������ ��� ���� �Ϻη� Ư���ൿ�� �Ͽ� Ÿ�̹��� ���ߴµ�
�� Ư���ൿ�� ������ �ϸ� '������' ��� �Ѵ�.

� ĳ������ ��� �ൿ�� �����Ӱ�
���ϴ� ������ ��������
�Է����� �־�����
�ش� ������ �������� ������Ű�� ��� �ൿ�� ������ ����Ѵ�


-��� : ��� �ൿ ������ �޸����� �غ��� �� ���ϴ� �������� �Է� �� �����Ǹ� ��� �� ���ο� �޸��忡 ����
���� ���ϴ� �������� ���� ���� ��쵵 ����. 20~40 ���� ��� �����Ǹ� �䱸�� �� ����.


-���� ���̵��
1 : ���� �ൿ�� ��� �����ذ��ٰ� ������ ���� �������� A�� ��
������ A�϶��� �����غôٸ� �� A�� ������Ų �ൿ�� ��� �����ص״� �ٽ� ������ ���̴� ������� �غ� ��.
�̷� �� �Ƹ� �޸������̼�(memoization)�̶� �ϴ� ��.

2 : �׸��� ���� �� ���� �ִٸ� �����صδ� �޸𸮸� 10000�� ������ �����ϰ�
�� �̻� ���� ���ٰ� �ǴܵǴ� �����ʹ� ������ �޸� �ڸ��� ���⵵ �غ��� �ͱ� ��

3 : �׸��� ���� ���� �� �ٸ� ����� �����ٰ� �� ���� �ֵ��� ����� ������ �����غ��� �ϴϱ�
Ŭ����(���⼭�� FrameRecipe) �ϳ��� �����ڿ� ���ϴ� ��ǥ ������ ������
���ο��� �˾Ƽ� ��� �����Ǹ� �� �˾Ƴ� ��
�����(=�� �ڵ� ������ �ٸ� ������)�� �װ� �޼ҵ� �ϳ��� ȣ���ϸ� �ٷ� ��� ��½�ų �� �ֵ��� ����

����1 : �ᱹ ���ո� ������ �Ǵϱ�
���ൿ�� ������ �ٲ� �Ǵ°ǵ�
�׷� ���۸� 3���� �ǵ� 3���丮��� 6������ �ߺ��� ��Ÿ����.
�� ��� �ߺ��� �ϳ��� �ν��� ���, �� �ߺ��� ���� ����� �����ΰ�?
���� ��� ��ǥ�� 10�̰� 2,3,5�� ������ 2,5,3 �̳� 3,2,5 ���� �ϳ��� ���� �����?
#(�ĺ�) : �׳� �ƹ����Գ� �� ã�� �������� �ߺ��� Ž���ؼ� ����
#(Ż��) : �ٸ� �����ǰ� �ֳ� ã���� �� �� ���� �ϳ��� ��� Ž�� �����ϴµ� �� ������ ������ ���� �� �ִٸ� ����
#(Ż��) : �������þ��� ���縸�� Ȯ���ϴ� set�� ���.
#(�հ�)set���� �ع����� 9�� 333���� ���� �� ����. �׷��� ���� �� ���� �� �ϼ� �ֵ� �� ������ ���ٸ� ���� ��. AAAAAE �� AAEAAA �� �� �� A�� 5���̰� E�� 1���̹Ƿ� ���� ��.

���ǻ��� : set<>�� ���� �ȿ� ���� Ÿ���� operator< �� �ݵ�� �־�� ��.
�Դٰ� ���� �� ����(set)�� ������ ������? �� ���� set == set �� �Ϸ��� operator==�� �ݵ�� �־�� ��.
�׸��� ���� operator< ���� ��� ���� �ϳ��� ���ϸ� �ٸ� ��������� �ٸ����� ���� �ɷ� �ν��ؼ� insert ���� ����.
(�ű��� �� set�� operator< �ϳ������ε� insert �� �� �������� ==�� �� �� ��.)
�׷��� operator< ���� �����ϸ� ��� ��� ������ ��� �񱳸� ����� �Ѵ�.

����2 : ��ǥ�� 10�ε� �̰� (0,10) (1,9) (2,8) ... (8,2) (9,1) (10,0) �̷��� 1�� �и��ذ��鼭 �ϴٰ� ��
(1,9)���� 9�� �� (1,8)�� ������ �ݺ��ϸ� ����Լ��� �ѳ� ������ �� ������
#�׷��� ��� ���� �� ���� ���� ���������� ��ư��鼭 ã��
#�׸��� ��ǥ�����Ӹ� �ּҵ������� ���� ������ �ؼ� ������ ���� �̷л� �ִ� ���� ������ ���� ����. �װ� ���� Ž�� ���� �� ������.
#�׸��� (1,9)�� ã�� (9,1)�� ã�� ���� �Ŵ� ��Ī����� Ž�� �� �ϱ�

����3 : ���� �������� ������ �ٸ� ������ ��� ó���ؾ� �ϴ°�?
#(Ż��)����ڿ��� ���� �������� ������ �� �� ���� �� ���� ��޶�� ��Ź�ϰų�
#(�հ�)�׳� ����ڴ� �Ǵ´�� �ᵵ ��ǻ�Ͱ� �о �� ������ ���� �� �ϳ��� �ν��ϵ��� �����ϰų�

����4 : �޸� �ϴ� ���(�޸������̼�)�� ��� �� ���ΰ�?
���� ��� 9�� �޸��� �� 333 ������ �����ٵ� �� ������ ����ϴ� ����
ó������ 3,3,3 ���� �����ص��� 3,6���� �����ߴٰ� 6�� �޸� �����ͼ� ��ĥ��.
#(����)�� ���� �Ұ��� ������ �ּ�ġ�� ��� �����ϴ� ��? �ּ�3 Ÿ��10�̸� (10) (3, 10-3=7) (3, 3, 10-3-3=4) �̷���
#(����)������ ������ �� ���θ� �ϰ� �ּ�ġ�� ��� ���ذ��� ��? �ּҰ� 3�̰� Ÿ���� 10�̸� (0, 10) (3, 7) (3+3=6, 4) (3+3+3=9, 1) �̷���
#3,3,3 ����(����)�� ���� �ߺ� ������ ������ �־ �׳� 3,6(����)�� ����. �׸��� ���� ���� ��Ī�� �ߺ��� �ν��ϱ⵵ ���� ��.
#�ٵ� 333�� �����ϴ� ���������θ� ��� ��Ұ� �����Ǵµ� 3,6�� 6�� �������� ���� ����. �׷��� ���ڴ� ���� �� �ߺ��� �Ͼ.
#�׷��� �˰����� ������ �� ���ڷ� ���ٰ� �������� ��� ������ �� ���ڷ� ��ȯ�ؾ� �� ��
#�� ����ٺ��� �˾Ҵµ� ������ ���ڷ� �ؾ���. �� �׷� �޸��� �ǹ̵� ���� �޸��� ���� ����. �Ƹ�? ��Ȯ�� �� �� �� �����غ�����
#(�հ�)Ž�������� ���� �� ���� ����ϴ� Ž����� �ǵ������� ���� �� �� �̻� �����ϴ� ���.
#�׸��� �� ���ڴ� �̹� Ž���� �߰� �׷��� �����ǰ� �����ϴ��� ���ϴ����� �����ϴ� 0,1,2�� ����Ǵ� ������ ������ ���� ��
#�ٵ� �׷��� �����̶� �� �������� �ǰ� ������ ������ �ƹ��� �۰� ��ƺ��� char(1byte=8bit)�� �Ǵµ� ��Ȯ�� 2bit�� ����ϴ� bool �� ���� ���� ������

���� ����Ʈ :
https://programming119.tistory.com/105 gitignore ����

���� 4�ð��� ���� ���� .gitignore ��
�ݵ�� git bash �󿡼� touch ���ɾ�� �����ؾ� �ȴٴ� �� �˾���. �ƴ� �׳� �޸������� .gitignore.txt �����ϰ� .txt�� ���������� ��.
https://kiros33.tistory.com/entry/%EC%9C%88%EB%8F%84%EC%9A%B0%EC%97%90%EC%84%9C-touch-%EB%AA%85%EB%A0%B9%EC%9D%84-%EB%8C%80%EC%B2%B4%ED%95%98%EB%8A%94-%EB%B0%A9%EB%B2%95
�� ����Ʈ�� ������ �� ó�� �׳� ������ cmd���� ���� �����ϴ� ������� gitignore�� ����� �׾��� ����� �۵� �� ��.


*/



int main()
{
	locale::global(locale("ko_KR.UTF-8"));
	int target_frame;
	wifstream frame_data("frame_data.txt");
	FrameRecipe fr(20, frame_data);

	fr.debug_file();
	fr.debug_set();
}