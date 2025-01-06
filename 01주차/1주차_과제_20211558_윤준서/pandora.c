#include <stdio.h>

typedef struct Node
{
	int data;
	struct Node* next;
}

void insertFront(struct Node, int);

void deleteNode(struct Node);

void printList(struct Node);

int main()
{
	Node node;
	while(1)
	{
	int n, data, m = 0;
	scanf("%d %d", &n, &data);
	if(n==1) insertFront(node, data);
	else if(n==2) deleteNode(node);
	else if(n==3) printList(node);
	else if(n==4) break;
	else printf("잘못된 입력입니다.");
	}
	return 0;
}

void insertFront(Node node, int data)
{
	struct Node new_node = node.next;
	new_node.data = data;
}

void deleteNode(struct Node node, int data) //시간 내에 구현하지 못했습니다
{
	while(1)
	{
		Node* scan = node;
		if(scan.data == node.data)
		{
			Node* temp = node + 1;
			
			
		}
	}
}

void printList(struct Node node)
{
	while(1)
	{
		Node *scan = node;
		printf("%d ", scan.data);
		scan = node.next
	}
}
