#include "Header.h"

void calculate_number(int page, int* number_list){
	int under_number = 0; //자릿수 숫자의 나머지 개수 합 : 개수를 세기 편하게 각 자릿수의 숫자와 같은 수들을 따로 계산한다.

	for(int i = 1; page; i *= 10) //1의 자리부터 계산한다. 점점 커질 수록 계산이 쉽기 때문이다.
	{
		int digit = page % 10; //각 자릿수의 숫자
		page /= 10; //한 자릿수 계산 후 다음 계산을 위해 page의 끝자리를 없앤다.

		for(int j = 0; j < digit; ++j) //(1 ~ i 자리의 숫자 - 1) 까지는 모든 수들의 개수가 같다.
		{
			number_list[j] += (page + 1) * i; //각 자리의 수 전까지 숫자는 (0 ~ 자릿수의 숫자) * i 만큼 있다.
		}

		number_list[digit] += page * i + under_number + 1; //i 자리의 숫자를 따로 계산한다.

		for(int j = digit + 1; j < 10; ++j)
		{
			number_list[j] += page * i; //(i 자리의 숫자+ 1) ~ 9 는 따로 숫자를 센다. 
		}

		number_list[0] -= i; //이전 계산 시에도 0의 계산이 들어갔으므로 중복을 빼준다.
		under_number += digit * i; //다음 자릿수 숫자의 나머지 개수를 미리 구해준다.
	}
}	
