#include "ofApp.h"

using namespace std;
ofPolyline dfs_line; //dfs 최단 탐색 경로 선
ofPolyline ad_line; //dfs 모든 탐색 경로 선

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("MAZE");
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag = 0;
    this->dfs_flag = 0;
    this->bfs_flag = 0;
    ofSetLineWidth(5);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(this->draw_flag == 1) {
        ofSetColor(0, 0, 0);
        for(int i = 0; i < this->LINE.size(); i++) {
            maze_line(this->LINE[i]);
        }
        
        if(this->dfs_flag == 1) {
            ofSetColor(0, 255, 255);
            ad_line.clear(); //초기화를 통해 불필요한 오류 방지하기
            stack<Block> u_stack = ad_path; //path가 변하지 않게 따로 벡터를 만들어 저장하기
            while(!u_stack.empty()) {
                ad_line.addVertex(u_stack.top().x * 10 + 10, u_stack.top().y * 10 + 10);
                u_stack.pop(); //좌표에 10을 더하는 이유 : 칸의 정중앙에 dfs선을 그리기 위함
            }
            ad_line.draw(); //모든 탐색 경로를 그린다
            
            ofSetColor(255, 0, 0);
            dfs_line.clear(); //초기화를 통해 불필요한 오류 방지하기
            stack<Block> v_stack = d_path; //path가 변하지 않게 따로 벡터를 만들어 저장하기
            while(!v_stack.empty()) {
                dfs_line.addVertex(v_stack.top().x * 10 + 10, v_stack.top().y * 10 + 10);
                v_stack.pop(); //좌표에 10을 더하는 이유 : 칸의 정중앙에 dfs선을 그리기 위함
            }
            dfs_line.draw(); //최단 경로를 그린다
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r') {
        cout << "r key Pressed" << endl;
        readFile();
        this->draw_flag = 1;
    }
    
    if(key == 'b') {
        if(this->draw_flag == 1) {
            this->bfs_flag = 1;
            cout << "BFS key Pressed" << endl;
        }
    }
    
    if(key == 'd') {
        if(this->draw_flag == 1){
            this->dfs_flag = 1;
            cout << "DFS key Pressed" << endl;
            int size_d = d_path.size();
            dfs();
        }
    }
    
    if(key == 'q') {
        cout << "Quit" << endl;
        freememory();
        OF_EXIT_APP(0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
bool ofApp::readFile() {
    freememory();
    HEIGHT = 0;
    WIDTH = 0;
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Input the maz file");
    
    if(openFileResult.bSuccess) {
        int ith = 0;
        string lines;
        
        string location = openFileResult.getPath();
        ifstream input_file, input_file2;
        input_file.open(location);
        input_file2.open(location);
        
        if(input_file.is_open()) {
            while(getline(input_file, lines)) {
                WIDTH = lines.length();
                width = (lines.length() - 1) / 2;
                HEIGHT++;
            }
            height = (HEIGHT - 1) / 2;
            
            cout << "WIDTH : " << width << " HEIGHT : " << height << endl;
            
            maze.resize(HEIGHT, vector<char>(WIDTH));
            
            string temp;
            
            for(int i = 0; i < HEIGHT; i++) {
                getline(input_file2, temp);
                for(int j = 0; j < WIDTH; j++) maze[i][j] = temp[j];
            }
            
            Line line_temp;
            
            for(int i = 0; i < HEIGHT; i++) {
                for(int j = 0; j < WIDTH; j++) {
                    if(maze[i][j] == '-') {
                        line_temp.sX = j;
                        line_temp.sY = i + 1;
                        line_temp.eX = j + 2;
                        line_temp.eY = i + 1;
                        this->LINE.push_back(line_temp);
                    }
                    else if(maze[i][j] == '|') {
                        line_temp.sX = j + 1;
                        line_temp.sY = i;
                        line_temp.eX = j + 1;
                        line_temp.eY = i + 2;
                        this->LINE.push_back(line_temp);
                    }
                }
            }
            return true;
        }
        else {
            cout << "Open File Failure." << endl;
            this->draw_flag = 0;
            return false;
        }
    }
    else {
        cout << "Input file canceled." << endl;
        return false;
    }
}

void ofApp::maze_line(Line line) {
    ofSetColor(0, 0, 0);
    ofSetLineWidth(2);
    ofDrawLine(10 * line.sX, 10 * line.sY, 10 * line.eX, 10 * line.eY);
}

bool ofApp::dfs() {
    this->draw_flag = 1;
    this->dfs_flag = 1;
    visit.resize(HEIGHT, vector<bool>(WIDTH, false)); //maze와 크기가 같은 visit 배열 생성
    for(int i = 0; i < HEIGHT;  i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(maze[i][j] != ' ') visit[i][j] = true;
        }
    } //visit의 역할은 미로의 모든 칸마다 방문의 여부를 저장하는데, 미로의 칸이 그냥 벽인 경우 true를 저장한다

    while(!d_path.empty()) d_path.pop(); //최단 경로를 저장하는 스택 초기화
    while(!ad_path.empty()) ad_path.pop(); //모든 경로를 저장하는 스택 초기화

    Block start;
    start.x = 1;
    start.y = 1;
    d_path.push(start);
    visit[1][1] = true; //(1, 1)에서 시작, visit도 true로 저장해 방문했음을 표시한다

    int x_move[4] = {0, 1, -1, 0}; //방향을 저장하는 배열
    int y_move[4] = {1, 0, 0, -1};
    
    while(!d_path.empty()){
        Block top = d_path.top();
        cout << "top : " << top.y << ' ' << top.x << endl; //최단 경로의 모든 칸의 좌표를 출력한다

        ad_path.push(d_path.top()); //경로 도착 유무를 판단하기 전에 모든 경로를 저장하는 벡터에 현재 경로 저장한다
        
        if(top.x >= WIDTH - 3 && top.y >= HEIGHT - 3) {
            return true; //path가 목표 끝 좌표까지 왔을 경우
        }
        
        bool possible = false; //이동 가능성 여부를 판단하는 변수
        for (int i = 0; i < 4; i++) {
            int newX = top.x + x_move[i]; //하, 우, 좌, 상 순서대로 이동한다
            int newY = top.y + y_move[i];

            if(newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && !visit[newY][newX]) {
                Block next; //이동이 가능한 경우
                next.x = newX;
                next.y = newY;
                d_path.push(next); //다음 칸을 생성하여 d_path에 저장
                visit[newY][newX] = true; //방문 표시하기
                possible = true;
                break;
            }
        }
        if(!possible) d_path.pop(); //더 이상 이동할 수 없으므로 경로를 지운다
    }
    return false;
}

void ofApp::freememory() {
    this->maze.clear();
    vector<vector<char>>().swap(this->maze);
    this->LINE.clear();
    vector<Line>().swap(this->LINE);
    this->visit.clear();
    vector<vector<bool>>().swap(this->visit);
    this->maze_block.clear();
    vector<vector<Direct>>().swap(this->maze_block);
    
    while(!d_path.empty()) d_path.pop();
    
    this->draw_flag = 0;
    this->dfs_flag = 0;
    this->bfs_flag = 0;
    dfs_line.clear();
    ad_line.clear();
}
