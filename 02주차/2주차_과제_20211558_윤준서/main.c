#include "Header.h"

int main()
{
	int T;
	scanf("%d", &T);
	
	for(int i = 0; i < T; i++)
	{
		int page;
		scanf("%d", &page);
		int number_list[10] = {0, };//테스트 시행마다 각 숫자의 개수가 담긴 리스트를 초기화한다.
		calculate_number(page, number_list);//각 숫자의 개수를 리스트에 담는 함수다.
		print_number(number_list);//수정된 리스트를 출력시키는 함수다.
	}
	return 0;
}
