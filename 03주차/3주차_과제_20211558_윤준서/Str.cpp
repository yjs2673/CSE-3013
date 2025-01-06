#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Str.h"
using namespace std;

Str::Str(int leng) //str의 생성자, string의 길이를 입력받는다. 이후 해당길이의 문자열을 생성한다.
{
	len = leng; //문자열의 길이를 len으로 저장한다.
	str = new char[len + 1]; //문자열을 생성한다.
	str[0] = '\0'; //문자열의 마지막 부분을 null로 지정한다.
}

Str::Str(char* neyong) //str의 생성자, string의 내용을 초기화한다.
{
	len = strlen(neyong); //neyong의 길이를 저장한다.
	str = new char[len + 1]; //str을 생성하고 초기화한다. 이때 마지막 부분을 null로 지정하기 위해 길이를 + 1 한다.
	strcpy(str, neyong); //str에 neyong을 복사한다.
	str[len] = '\0'; //str의 마지막 부분을 null로 지정한다.
}

Str::~Str() //str의 소멸자
{
	delete[] str;
}

int Str::length() //string의 길이를 반환한다.
{
	return len;
}

char* Str::contents() //string의 내용을 반환한다.
{
	return str;
}

int Str::compare(Str& a) //strcmp 함수를 이용해 두 문자열을 비교한다.
{
	return strcmp(str, a.str);
}

int Str::compare(char* a) //strcmp 함수를 이용해 두 문자열을 비교한다.
{
	return strcmp(str, a);
}

void Str::operator=(char *a) //문자열을 Str에 대입(복사)한다.
{
	if(a == 0) //문자열이 null 포인터인 경우
	{
		len = 0; //문자열의 길이를 0으로 바꾼다.
		delete[] str; //기존 str을 소멸한다.
		str = new char[1]; //다시 str을 생성한다. null만을 값으로 갖기 위해 길이를 1로 만든다.
		str[0] = '\0'; //str의 값을 null로 지정한다.
		return;
	}
	
	int leng_a = strlen(a); //문자열이 null 포인터가 아닌 경우, 그 길이를 저장한다.

	if(len < leng_a) //문자열의 길이가 str의 길이보다 긴 경우
	{
		delete[] str; //기존 str을 소멸한다.
		str = new char[leng_a + 1]; //다시 str을 생성한다. 끝 부분을 null로 지정하기 위해 길이를 + 1 한다.
	}
	
	len = leng_a; //str의 길이를 문자열의 길이와 같게 저장한다.
	strcpy(str, a); //문자열의 내용을 str에 복사한다.
	str[len] = '\0'; //str의 끝 부분을 null로 지정한다.
}

void Str::operator=(Str& a)
{
	(*this) = a.contents(); //현재 객체인 Str을 a 객체인 문자열로 바꾼다.
}
