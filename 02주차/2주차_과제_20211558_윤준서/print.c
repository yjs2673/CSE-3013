#include "Header.h"

void print_number(int* number_list){
	for(int i = 0; i < 10; i++)//숫자가 0부터 9까지이므로 리스트의 인덱스 수는 총 10이다.
	{
		if(i == 9) printf("%d\n", number_list[i]);
		else printf("%d ", number_list[i]);//차례대로 0부터 9까지의 숫자의 개수를 출력한다.
	}
}
