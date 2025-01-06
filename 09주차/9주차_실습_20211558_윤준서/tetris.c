#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	createRankList();

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	
	writeRankFile();
	
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2] = rand() % 7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawShadow(blockY, blockX, nextBlock[0], blockRotate);
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(9, WIDTH + 10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for(i = 0; i < 4; i++)
	{
		move(10 + i, WIDTH + 13);
		for(j = 0; j < 4; j++)
		{
			if(block[nextBlock[2]][0][i][j] == 1)
			{
				attron(A_REVERSE);
                                printw(" ");
                                attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				if(tile == '.') printw(".");
				else
				{
					attron(A_REVERSE);
					printw("%c", tile);
				}
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block[currentBlock][blockRotate][i][j])
			{
				int x = blockX + j;
				int y = blockY + i;
				if((f[y][x]) || (x >= WIDTH) || (x < 0) || (y >= HEIGHT) || (y < 0)) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것
	int lengthy;
	switch(command)
        {
        	case KEY_UP: 
			lengthy = blockY;
			while(CheckToMove(field, nextBlock[0], (blockRotate + 3) % 4, lengthy + 1, blockX)) lengthy++;
			DrawBlock(lengthy, blockX, currentBlock, (blockRotate + 3) % 4, '.');
			DrawBlock(blockY, blockX, currentBlock, (blockRotate + 3) % 4, '.');
			break;

        	case KEY_DOWN: 
			DrawBlock(blockY - 1, blockX, currentBlock, blockRotate, '.');
			break;

        	case KEY_LEFT: 
			lengthy = blockY;
			while(CheckToMove(field, nextBlock[0], blockRotate, lengthy + 1, blockX + 1 )) lengthy++;
			DrawBlock(lengthy, blockX + 1, currentBlock, blockRotate,'.');
			DrawBlock(blockY, blockX + 1, currentBlock, blockRotate, '.');
			break;

        	case KEY_RIGHT: 
			lengthy = blockY;
			while(CheckToMove(field, nextBlock[0], blockRotate, lengthy + 1, blockX - 1)) lengthy++;
			DrawBlock(lengthy, blockX-1, currentBlock, blockRotate,'.');
			DrawBlock(blockY, blockX - 1, currentBlock, blockRotate, '.');
			break;

        	default: 
			break;        
	}	

	//3. 새로운 블록 정보를 그린다. 
	DrawShadow(blockY, blockX, currentBlock, blockRotate);
	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
}

void BlockDown(int sig){
	// user code
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX))
	{
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else
	{
		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		if(blockY == -1) gameOver = 1;
		else
		{
			score += DeleteLine(field);
			for(int i = 0; i < 2; i++) nextBlock[i] = nextBlock[i + 1];
			nextBlock[2] = (rand() + 7)%7;
			DrawNextBlock(nextBlock);
			blockY = -1;
			blockX = WIDTH / 2 - 2;
			blockRotate = 0;
			PrintScore(score);
		}
		DrawField();
	}
	timed_out = 0;
	//강의자료 p26-27의 플로우차트를 참고한다.
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    	int count = 0;
    	for(int i = 0; i < 4 ; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(block[currentBlock][blockRotate][i][j])
			{
				if((blockY + i >= 0) && (blockY + i < HEIGHT) && (blockX + j >= 0) && (blockX + j < WIDTH))
				{
					f[blockY + i][blockX + j] = 1;
					if(HEIGHT - 1 == blockY + i) count++;
				}
			}
		}
	}
	//Block이 추가된 영역의 필드값을 바꾼다.
	score += count * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	int c = 0;
	bool fit = 0;
	for(int i = 0; i < HEIGHT; i++)
	{
		fit = 1;
		for(int j = 0; j < WIDTH; j++) if(!f[i][j]) fit = 0;
		if(fit)
		{
			c++;
			for(int y = i; y >= 1; y--) for(int x = 0; x <= WIDTH; x++) f[y][x] = f[y - 1][x];
			for(int x = 0; x < WIDTH; x++) f[0][x] = 0;
			i--;
		}
	}
	
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	return c*c*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	int lengthy = y;
	while(CheckToMove(field, nextBlock[0], blockRotate, lengthy + 1, x))
	{
		lengthy++;
	}
	DrawBlock(lengthy, x, blockID, blockRotate, '/');
}

void createRankList(){
	// user code
	FILE* fp = fopen("rank.txt", "r");
	char new_name[NAMELEN];
	fscanf(fp, "%d", &amount);
	int new_score;
	for(int i = 0; i < amount; i++)
	{
		fscanf(fp, "%s%d", new_name, &new_score);
		newnode(new_name, new_score);
	}
	fclose(fp);
}

void rank(){
	// user code
	char name[NAMELEN];
	clear();
	char num;
	do
	{
		clear();
		printw("1. list ranks from X to Y\n");
		printw("2. list ranks by a specific name\n");
		printw("3. delete a specific rank\n");
		num = wgetch(stdscr);
	}while(num < '1' || num > '3');

	echo();
	int X = -1, Y = -1;
	node *index = head;

	switch(num)
	{
		case '1':
			printw("X : ");
			scanw("%d", &X);
			printw("Y : ");
			scanw("%d", &Y);
			if(X < 0) X = 1;
			if(Y < 0) Y = amount;
			noecho();

			int n = 1;

			if(X > Y)
			{
				printw("search failure : no rank in the list");
				break;
			}

			printw("      name	|      score\n");
			printw("----------------------------------\n");
			while(index != NULL)
			{
				if((X <= n) && (n <= Y)) printw(" %-15s\| %-12d\n", index->name, index->score);
				n++;
				index = index->link;
			}
			break;

		case '2':
			printw("9th Week Homework\n"); break;

		case '3':
			printw("9th Week Homework\n"); break;
		
	}
	noecho();
	getch();
}

void writeRankFile(){
	// user code
	FILE *fp = fopen("rank.txt", "w");
	node *new = head;

	fprintf(fp, "%d\n", amount);
	while(1)
	{
		fprintf(fp, "%s %d\n", new->name, new->score);
		if(new->link == NULL) break;
		new = new->link;
	}
	fclose(fp);
}

void newRank(int score){
	// user code
	clear();
	echo();
	char new_name[NAMELEN];
	printw("your name : ");
	scanw("%s", new_name);
	noecho();
	insert_node(new_name, score);
}

void newnode(char* name, int score)
{
	node *current = head;
	node *new;
	new = (node*)malloc(sizeof(node));
	new->name[0] = '\0';
	new->score = score;
	strcpy(new->name, name);
	new->link = NULL;
	
	if(head == NULL) head = new;
	else
	{
		while(current->link != NULL) current = current->link;
		current->link = new;
	}
}

void insert_node(char* name, int score)
{
	node* prev = head;
	node* current = head;
	node* new;
	new = (node*)malloc(sizeof(node));
	new->name[0] = '\0';
	new->score = score;
	new->link = NULL;
	strcpy(new->name, name);
	amount++;

	if(head == NULL) head = new;
	else if(head->link == NULL)
	{
		if((new->score) > (head->score))
		{
			new->link = head;
			head = new;
		}
		else head->link = new;
	}
	else
	{
		if((new->score) > (head->score))
		{
			new->link = head;
			head = new;
			return;
		}
		while(1)
		{
			if((new->score) > (current->score)) break;
			if(current->link == NULL) break;
			prev = current;
			current = current->link;
		}
		if((prev->score) > (new->score) && (new->score) > (current->score))
		{
			prev->link = new;
			new->link = current;
		}
		else current->link = new;
	}
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
