#include "ofApp.h"

using namespace std;
ofPolyline path;
ofPolyline all_path;

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
        for (int i = 0; i < this->LINE.size(); i++) maze_line(this->LINE[i]); //LINE에 저장된 선 출력
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r'){
        cout << "r key Pressed" << endl;
        readFile();
        this->draw_flag = 1;
    }
    if (key == 'b'){
        this->bfs_flag = 1;
        cout << "BFS key Pressed" << endl;
    }
    if (key == 'd'){
        if(this->draw_flag == 1){
            this->dfs_flag = 1;
                
            cout << "DFS key Pressed" << endl;
        }
    }
    if (key == 'q'){
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
        string lines;
        string location = openFileResult.getPath();
        ifstream input, input2;
        input.open(location);
        input2.open(location);
        
        if(input.is_open()) {
            while(getline(input, lines)) {
                WIDTH = lines.length();
                width = (lines.length() - 1) / 2; // 벽으로 정의된 칸도 WIDTH에 포함되니 빼주기
                HEIGHT++;
            }
            height = (HEIGHT - 1) / 2; // 벽으로 정의된 칸도 WIDTH에 포함되니 빼주기
            
            cout << "WIDTH : " << width << " HEIGHT : " << height << endl;
            
            maze.resize(HEIGHT, vector<char>(WIDTH)); // 2차원 벡터로 초기화
            
            string temp;
            
            for(int i = 0; i < HEIGHT; i++) {
                getline(input2, temp); //HEIGHT, WIDTH 구하느라 이미 아래로 쭉 읽었으니 한 번 더 처음부터 파일 읽기
                for(int j = 0; j < WIDTH; j++) maze[i][j] = temp[j]; // - | 저장하기
            }
            
            Line line_temp;
            
            for(int i = 0; i < HEIGHT; i++) {
                for(int j = 0; j < WIDTH; j++) {
                    if(maze[i][j] == '-') { // 선 그리기, 선 간의 간격 맞추기 위해 0.5, 1.5 의 단위 사용
                        line_temp.sX = j; // 해당 maze 좌표의 선 그림의 시작과 끝 좌표 저장
                        line_temp.sY = i + 1;
                        line_temp.eX = j + 2;
                        line_temp.eY = i + 1;
                        this->LINE.push_back(line_temp); //선을 LINE에 벡터로 저장
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
    ofSetLineWidth(1);
    ofDrawLine(10 * line.sX, 10 * line.sY, 10 * line.eX, 10 * line.eY);
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
    
    while(!m_block.empty()) m_block.pop();
    while(!a_path.empty()) a_path.pop();
    
    this->draw_flag = 0;
    this->dfs_flag = 0;
    this->bfs_flag = 0;
    path.clear();
    all_path.clear();
}
