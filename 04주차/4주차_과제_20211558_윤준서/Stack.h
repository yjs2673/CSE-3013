#include "LinkedList.h"


//1. 템플릿 클래스로 확장해야함
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져와야함

//LinkedList class를 상속받음

template <typename T> //template 설정
class Stack : public LinkedList<T> //함수와 자료형에 T를 삽입한다
{
	public:
		bool Delete(T &element);
};

template <typename T>
bool Stack<T>::Delete(T &element)
{
			//first가 0이면 false반환
			if(this -> first == NULL) return false; //this를 이용해 first 값을 불러온다
			// LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
			element = this -> first -> data;
			Node<T> *t = this -> first;
			this -> first = this -> first -> link;
			delete t; //Stack의 첫 부분, 즉 새로 들어온 부분을 삭제한다
			this -> current_size--;

			return true;			
}
