#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int parent;
    int level;
} UnionFind;

int Find(UnionFind uf[], int i) { // 노드의 부모 level 탐색
    if(uf[i].parent != i) uf[i].parent = Find(uf, uf[i].parent);
    return uf[i].parent;
}

void Union(UnionFind uf[], int i, int j) { // 두 독립된 구역 합치기
	int level_i = Find(uf, i);
    int level_j = Find(uf, j);

    if(level_i != level_j) {
        if(uf[level_i].level < uf[level_j].level) uf[level_i].parent = level_j;
        else if(uf[level_i].level > uf[level_j].level) uf[level_j].parent = level_i;
        else {
            uf[level_j].parent = level_i;
            uf[level_i].level++;
        }
    }
}

int main() {
    int n, m, i, j;
    printf("height & width : ");
    scanf("%d %d", &n, &m);
    srand(time(0));

    // 미로 방과 벽 초기화
    int maze[n][m];
    for(i = 0; i < n; i++) for(j = 0; j < m; j++) maze[i][j] = 0;
    char height_wall[n][m - 1];
    for(i = 0; i < n; i++) for(j = 0; j < m - 1; j++) height_wall[i][j] = '|';
    char width_wall[n - 1][m];
    for(i = 0; i < n - 1; i++) for(j = 0; j < m; j++) width_wall[i][j] = '-';
    
    // Union-Find 초기화
    UnionFind uf[n * m];
    for(i = 0; i < n * m; i++) {
        uf[i].parent = i;
        uf[i].level = 0;
    }

    // 방마다 탐색 후 벽 없애기 반복
	while (1) {
        int count = 0;
        for(i = 0; i < n; i++) for(j = 0; j < m; j++) if(Find(uf, i * m + j) == Find(uf, 0)) count++;
        if(count == n * m) break; // 모든 칸이 이어진 경우 반복문 종료

        int rand_i = rand() % n;
        int rand_j = rand() % m;
        int direct = rand() % 2;

        if(direct == 0 && rand_j + 1 < m && Find(uf, rand_i * m + rand_j) != Find(uf, rand_i * m + rand_j + 1)) {
            Union(uf, rand_i * m + rand_j, rand_i * m + rand_j + 1);
			height_wall[rand_i][rand_j] = ' ';
        }
		else if(direct == 1 && rand_i + 1 < n && Find(uf, rand_i * m + rand_j) != Find(uf, rand_i * m + rand_j + m)) {
            Union(uf, rand_i * m + rand_j, rand_i * m + rand_j + m);
            width_wall[rand_i][rand_j] = ' ';
        }
	}

    // 완성된 미로 출력
    for(i = 0; i < m; i++) printf("+-");
    printf("+\n");

    for(i = 0; i < n; i++) {
        // 세로선
        printf("|");
        for(j = 0; j < m - 1; j++) printf(" %c", height_wall[i][j]);
        printf(" |\n");

		//가로선
        if(i < n - 1) {
            for(j = 0; j < m; j++) printf("+%c", width_wall[i][j]);
            printf("+\n");
        }
    }

    for(i = 0; i < m; i++) printf("+-");
    printf("+\n");
	
    return 0;
}

