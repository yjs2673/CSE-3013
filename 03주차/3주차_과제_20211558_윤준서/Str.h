#ifndef __Str__
#define __Str__

class Str
{
	protected:
		char *str; //string 내용
		int len; //string 길이

	public:
		Str(int leng); //leng은 string의 길이
		Str(char *neyong); //neyong은 초기화할 내용
		~Str(); //소멸자

		int length(void); //string의 길이 리턴

		char *contents(void); //string의 내용 리턴

		int compare(class Str& a); //a의 내용과 strcmp
		int compare(char *a); //a의 내용과strcmp

		void operator = (char *a); //string의 값 대입
		void operator = (class Str& a); //Str의 내용 대입
};
#endif
