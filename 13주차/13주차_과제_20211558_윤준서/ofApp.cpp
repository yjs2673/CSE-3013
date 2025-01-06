#include "ofApp.h"

using namespace std;
ofPolyline dfs_line; //dfs 최단 탐색 경로 선
ofPolyline ad_line; //dfs 모든 탐색 경로 선
ofPolyline bfs_line;

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

        if(this->bfs_flag == 1) {
            for(int i = 0; i < this->PATH.size(); i++) {
                path_line(this->PATH[i]);
            }

            ofSetColor(255, 0, 0);
            bfs_line.clear(); //초기화를 통해 불필요한 오류 방지하기
            queue<Block> v_queue = b_path; //path가 변하지 않게 따로 벡터를 만들어 저장하기
            while(!v_queue.empty()) {
                bfs_line.addVertex(v_queue.front().x * 10 + 10, v_queue.front().y * 10 + 10);
                v_queue.pop(); //좌표에 10을 더하는 이유 : 칸의 정중앙에 dfs선을 그리기 위함
            }
            bfs_line.draw(); //최단 경로를 그린다
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
            this->visit.clear();
            vector<vector<bool>>().swap(this->visit);
            this->PATH.clear();
            vector<Line>().swap(this->PATH);
            while(!d_path.empty()) d_path.pop();
            while(!ad_path.empty()) ad_path.pop();
            while(!b_path.empty()) b_path.pop();
            while(!r_path.empty()) r_path.pop();
            this->dfs_flag = 0;
            this->bfs_flag = 0;
            dfs_line.clear();
            ad_line.clear();
            bfs_line.clear();
            
            this->bfs_flag = 1;
            cout << "BFS key Pressed" << endl;
            int size_a = b_path.size();
            bfs();
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

void ofApp::path_line(Line line) {
    ofSetColor(0, 255, 255);
    ofSetLineWidth(2);
    ofDrawLine(10 * line.sX + 10, 10 * line.sY + 10, 10 * line.eX + 10, 10 * line.eY + 10);
}

bool ofApp::bfs() {
    this->draw_flag = 1;
    this->bfs_flag = 1;
    visit.resize(HEIGHT, vector<bool>(WIDTH, false));
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(maze[i][j] != ' ') visit[i][j] = true;
        }
    }

    queue<Block> Q;
    Block start;
    start.x = 1;
    start.y = 1;
    Q.push(start);
    visit[1][1] = true;

    map<pair<int, int>, pair<int, int>> parent;
    parent[{1, 1}] = {-1, -1};

    int x_move[4] = {0, 1, -1, 0};
    int y_move[4] = {1, 0, 0, -1};

    while(!b_path.empty()) b_path.pop();

    while(!Q.empty()) {
        Block curr = Q.front();
        Q.pop();
        
        if(curr.x >= WIDTH - 3 && curr.y >= HEIGHT - 3) {
            pair<int, int> process = {curr.x, curr.y};
            while(process != make_pair(1, 1)) {
                Block q_block;
                q_block.x = process.first;
                q_block.y = process.second;
                r_path.push(q_block);
                process = parent[process];
            }

            Block temp_block;
            temp_block.x = 1;
            temp_block.y = 1;
            r_path.push(temp_block);

            // b_path에 저장된 경로를 올바른 순서로 변환
            while(!r_path.empty()) {
                b_path.push(r_path.front());
                r_path.pop();
            }
            return true;
        }

        
        Line b2_path;
        
        for(int i = 0; i < 4; i++) {
            int newX = curr.x + x_move[i];
            int newY = curr.y + y_move[i];
            b2_path.eX = curr.x + x_move[i];
            b2_path.eY = curr.y + y_move[i];
            b2_path.sX = curr.x;
            b2_path.sY = curr.y;
            if(newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && !visit[newY][newX]) {
                Block next;
                next.x = newX;
                next.y = newY;
                Q.push(next);
                this->PATH.push_back(b2_path);
                visit[newY][newX] = true;
                parent[{newX, newY}] = {curr.x, curr.y};
            }
        }
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
    this->PATH.clear();
    vector<Line>().swap(this->PATH);

    while(!d_path.empty()) d_path.pop();
    while(!ad_path.empty()) ad_path.pop();
    while(!b_path.empty()) b_path.pop();
    while(!r_path.empty()) r_path.pop();

    this->draw_flag = 0;
    this->dfs_flag = 0;
    this->bfs_flag = 0;
    dfs_line.clear();
    ad_line.clear();
    bfs_line.clear();
}
