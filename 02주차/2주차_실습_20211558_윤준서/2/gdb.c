#include <stdio.h>

main(void)
{
	int i;
	double num;
	
	for(i = 0; i < 5; i++){
		num = i/2 + i;
		printf("num is %f \n", num);
	}
}
/* gdb 를 실행 후 break main 을 입력 후
run 실행 후 display i 와 display num 을 입력해 실시간으로 값을 확인한다.

next 를 반복 입력한 결과 i 가 홀수일 때 num 의 값이 올바르게 나오지 않는다.
0.5 가 계속 빠지게 되는데 이는 i 가 정수형이기 때문에 num 과 연산을 하면
계속 계산값이 정수로 출력이 되기 때문이다.

이를 해결하기 위해서 i 의 값을 0.0 으로 바꾸거나
num = (double)i/2 + i 로 코드를 수정한다. */
