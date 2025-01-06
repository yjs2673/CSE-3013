#include "ofApp.h"

using namespace std;
ofPoint pos; // 플레이어 위치를 표시하는 점
ofPolyline hint_line; // 힌트를 표시하는 선
string message; // 윈도우 창에 메세지를 출력하는 문구
vector<vector<bool>> visited; // 방문 체크를 위한 2차원 벡터
vector<Line> visited_lines; // 방문한 경로를 저장하는 벡터

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("MAZE");
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag = 0; // 미로 플래그 초기화
    this->hint_flag = 0; // 힌트 플래그 초기화
    ofSetLineWidth(5); // 랭크 리스트 초기화
    for (int i = 0; i < 15; i++) {
        list[i].score = 0;
        list[i].count = 0;
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofClear(255, 255, 255); // 화면을 매 프레임마다 흰색으로 지우기

    // 시작 기본 메시지 출력
    ofSetColor(0, 0, 0);
    message = "Press R To Open Your Maze File!";
    ofDrawBitmapString(message, 650, 20);
    message = "Rank List!";
    ofDrawBitmapString(message, 720, 300);
    message = "__________________________";
    ofDrawBitmapString(message, 650, 320);
    
    // 미로와 게임 관련 정보 출력
    if (draw_flag == 1) {
        ofSetColor(0, 0, 0);
        string size1 = to_string(height);
        string size2 = to_string(width);
        message = "Maze Size : ";
        message += size1 + " * " + size2;
        ofDrawBitmapString(message, 650, 40); // 미로 사이즈 출력

        message = "Press S To Start The Game!"; // 시작 안내 출력
        ofDrawBitmapString(message, 650, 60);

        for (int i = 0; i < LINE.size(); i++) { // 미로 출력
            maze_line(LINE[i]);
        }

        if (hint_flag == 1) { // 힌트 사용 시
            ofSetColor(255, 0, 0);
            message = "Use Hint!"; // 힌트를 사용한다는 문구 출력
            ofDrawBitmapString(message, 650, 80);

            ofSetColor(255, 0, 0);
            hint_line.clear(); // 이전 힌트 경로 초기화
            queue<Block> v_queue = b_path; // 힌트 경로 새로운 큐에 저장
            while (!v_queue.empty()) {
                hint_line.addVertex(v_queue.front().x * 10 + 10, v_queue.front().y * 10 + 10);
                v_queue.pop();
            }
            hint_line.draw(); // 힌트인 최단 경로 출력
        }

        ofSetColor(255, 255, 0);
        for (const auto& line : visited_lines) { // 플레이어가 방문한 경로 실시간 출력
            ofDrawLine(line.sX, line.sY, line.eX, line.eY);
        }
        
        if (start_flag == 1) { // 게임 시작 시
            ofSetColor(0, 0, 255);
            ofDrawCircle(pos, 5); // 플레이어의 현재 위치를 점으로 표시
        }

        if (end_flag == 1) { // 게임 종료
            if (hint_flag == 0) { // 힌트를 한 번도 사용하지 않았을 경우 출력
                ofSetColor(0, 0, 255);
                message = "WOW You Did Not Use Hint!";
                ofDrawBitmapString(message, 650, 80);
            }

            ofSetColor(0, 0, 0); // 종료를 알리는 문구 출력
            message = "You Find The Exit!";
            ofDrawBitmapString(message, 650, 100);

            string move = to_string(count_move); // 총 이동 횟수 출력
            message = "Total Move : ";
            message += move;
            ofDrawBitmapString(message, 650, 120);

            string score = to_string(WIDTH * HEIGHT - count_move); // 총 점수 출력
            message = "Your Score : ";
            message += score;
            ofDrawBitmapString(message, 650, 140);
            
            string game_cnt = to_string(game_count); // 총 몇 번째 게임인지 출력
            message = "Playtime : ";
            message += game_cnt + "th";
            ofDrawBitmapString(message, 650, 160);

            message = "Press R To Restart The Game!"; // 게임 재시작 안내 문구 출력
            ofDrawBitmapString(message, 650, 180);
        }
        
        ofSetColor(0, 0, 0);
        for (int i = 0; i < 15; i++) { // 랭크 리스트 출력
            string str_score = to_string(list[i].score);
            string index = to_string(list[i].count);
            message = index + "th playing"; // 몇 번째 게임의 점수인지 표시
            if (list[i].count < 10) message += "  ";
            else if (list[i].count < 100) message += " ";

            message += " | " + str_score;
            ofDrawBitmapString(message, 650, 340 + i * 20);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r') { // 미로 파일 열기
        cout << "Open Your Maze File" << endl;
        readFile();
        this->draw_flag = 1;
    }
    
    if(key == 's') { // 게임 시작
        game_count++; // 게임 횟수 1 추가
        count_move = 0; // 이동 횟수 초기화
        hint_flag = 0; // 힌트 사용 여부 초기화
        cout << "Start The Game" << endl; // 게임 시작 문구 출력
        this->start_flag = 1; // draw를 위한 시작 플래그 설정
        this->end_flag = 0; // 종료 플래그 초기화
        pos.set(20, 20); // 플레이어 시작 위치 초기화
        
        // 방문 체크 벡터 초기화
        visited.resize(HEIGHT, vector<bool>(WIDTH, false));
        visited_lines.clear();
    }
    
    if(start_flag == 1 && end_flag == 0) {
        ofPoint prev_pos = pos; // 이전 위치 저장
        bool moved = false;
        
        if(key == OF_KEY_LEFT) { // 왼쪽 키
            if(pos.x > 10 && maze[(pos.y - 10) / 10][(pos.x - 20) / 10] == ' ') {
                pos.x -= 20; // 왼쪽으로 이동
                count_move++; // 이동 횟수 증가
                moved = true;
            }
        }
        
        if(key == OF_KEY_RIGHT) { // 오른쪽 키
            if(pos.x == width * 20 && pos.y == height * 20) { // 도착 지점에 도착한 경우
                pos.x += 20; // 미로 밖으로 탈출
                count_move++; // 마지막 탈출도 이동 횟수에 추가
                moved = true;

                this->end_flag = 1; // draw를 위한 종료 플래그 설정
                int new_score = WIDTH * HEIGHT - count_move; // 점수를 랭크 리스트에 기록
                make_list(new_score, game_count);
            }
            else if(pos.x < WIDTH * 10 - 10 && maze[(pos.y - 10) / 10][(pos.x) / 10] == ' ') {
                pos.x += 20; // 플레이어가 그 외의 지점에 있을 경우, 통상적인 오른쪽 이동을 실행
                count_move++;
                moved = true;
            }
        }
        
        if(key == OF_KEY_UP) { // 위로 이동
            if(pos.y > 10 && maze[(pos.y - 20) / 10][(pos.x - 10) / 10] == ' ') {
                pos.y -= 20;
                count_move++;
                moved = true;
            }
        }
        
        if(key == OF_KEY_DOWN) { // 아래로 이동
            if(pos.y < HEIGHT * 10 - 10 && maze[(pos.y) / 10][(pos.x - 10) / 10] == ' ') {
                pos.y += 20;
                count_move++;
                moved = true;
            }
        }
        
        if (moved) { // 이동한 경로의 위치 저장
            int x_idx = (pos.x - 10) / 10;
            int y_idx = (pos.y - 10) / 10;

            if (!visited[y_idx][x_idx]) {
                visited[y_idx][x_idx] = true;
                cout << "Total move : " << count_move << endl; // 현재까지 총 이동 횟수 출력
            }

            // 이동한 경로 저장
            Line line;
            line.sX = prev_pos.x;
            line.sY = prev_pos.y;
            line.eX = pos.x;
            line.eY = pos.y;
            visited_lines.push_back(line);
        }
    }
    
    if(key == 'h') { // 힌트 사용하기
        if(this->draw_flag == 1) { // 이전 힌트 경로 초기화
            this->visit.clear();
            vector<vector<bool>>().swap(this->visit);
            this->PATH.clear();
            vector<Line>().swap(this->PATH);
            while(!b_path.empty()) b_path.pop();
            while(!r_path.empty()) r_path.pop();
            this->hint_flag = 0;
            hint_line.clear();
            
            this->hint_flag = 1; // draw를 위한 힌트 플래그 설정
            cout << "Close The Hint" << endl;
            int size_a = b_path.size();
            hint(); // 힌트 함수
        }
    }
    
    if(key == 'c') { // 힌트 지우기
        if(this->draw_flag == 1) {
            this->visit.clear(); // 현재 힌트 경로 초기화
            vector<vector<bool>>().swap(this->visit);
            this->PATH.clear();
            vector<Line>().swap(this->PATH);
            while(!b_path.empty()) b_path.pop();
            while(!r_path.empty()) r_path.pop();
            hint_line.clear();
            cout << "Close The Hint" << endl;

        }
    }

    if(key == 'q') { // 게임 종료
        cout << "Quit The Game" << endl; // 게임 종료 문구
        freememory();
        OF_EXIT_APP(0); // 프로그램 종료
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
    freememory(); // 이전 데이터 초기화

    HEIGHT = 0;
    WIDTH = 0;
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Input the maze file");

    if (openFileResult.bSuccess) {
        int ith = 0;
        string lines;

        string location = openFileResult.getPath();
        ifstream input_file, input_file2;
        input_file.open(location);
        input_file2.open(location);

        if (input_file.is_open()) {
            while (getline(input_file, lines)) {
                WIDTH = lines.length();
                width = (lines.length() - 1) / 2;
                HEIGHT++;
            }
            height = (HEIGHT - 1) / 2;

            cout << "Open Maze Size : " << height << " * " << width << endl;

            maze.resize(HEIGHT, vector<char>(WIDTH));

            string temp;

            for (int i = 0; i < HEIGHT; i++) {
                getline(input_file2, temp);
                for (int j = 0; j < WIDTH; j++) maze[i][j] = temp[j];
            }

            // 미로 선 초기화
            Line line_temp;
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    if (maze[i][j] == '-') {
                        line_temp.sX = j;
                        line_temp.sY = i + 1;
                        line_temp.eX = j + 2;
                        line_temp.eY = i + 1;
                        LINE.push_back(line_temp);
                    } else if (maze[i][j] == '|') {
                        line_temp.sX = j + 1;
                        line_temp.sY = i;
                        line_temp.eX = j + 1;
                        line_temp.eY = i + 2;
                        LINE.push_back(line_temp);
                    }
                }
            }
            return true;
        } else {
            cout << "Open File Failure." << endl;
            draw_flag = 0;
            return false;
        }
    } else {
        cout << "Input file canceled." << endl;
        return false;
    }
}

void ofApp::maze_line(Line line) { // 미로 그리기
    ofSetColor(0, 0, 0);
    ofSetLineWidth(2);
    ofDrawLine(10 * line.sX, 10 * line.sY, 10 * line.eX, 10 * line.eY);
}

void ofApp::path_line(Line line) { // 힌트 경로 그리기
    ofSetColor(255, 255, 0);
    ofSetLineWidth(2);
    ofDrawLine(10 * line.sX + 10, 10 * line.sY + 10, 10 * line.eX + 10, 10 * line.eY + 10);
}

bool ofApp::hint() { //bfs를 통해 최단 경로를 알려주는 힌트 함수
    this->draw_flag = 1;
    this->hint_flag = 1;
    visit.resize(HEIGHT, vector<bool>(WIDTH, false)); //bfs탐색 시 칸마다 방문 여부 저장
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(maze[i][j] != ' ') visit[i][j] = true;
        }
    }

    queue<Block> Q; // 큐를 사용하기
    Block start; // 시작 지점 초기화
    start.x = 1;
    start.y = 1;
    Q.push(start);
    visit[1][1] = true;

    map<pair<int, int>, pair<int, int>> parent; // parent를 이용해 시작 지점 역으로 찾기
    parent[{1, 1}] = {-1, -1};

    //방향 배열
    int x_move[4] = {0, 1, -1, 0};
    int y_move[4] = {1, 0, 0, -1};

    while(!b_path.empty()) b_path.pop(); // 최단 경로 초기화

    while(!Q.empty()) {
        Block curr = Q.front();
        Q.pop(); // 큐의 헤드 부분 지우기

        if(curr.x >= WIDTH - 3 && curr.y >= HEIGHT - 3) { // 목표지점까지 탐색이 완료된 경우
            // 경로를 추적하여 a_path에 저장
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

        Line b2_path; // 다음 경로
        
        for(int i = 0; i < 4; i++) { // 다음 경로의 이동 가능 유무 판단
            int newX = curr.x + x_move[i];
            int newY = curr.y + y_move[i];
            b2_path.eX = curr.x + x_move[i];
            b2_path.eY = curr.y + y_move[i];
            b2_path.sX = curr.x;
            b2_path.sY = curr.y;
            if(newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && !visit[newY][newX]) {
                Block next; // 이동이 가능하면 큐에 좌표 추가하기
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

void ofApp::make_list(int new_score, int count) { // 새로운 점수와 플레이 횟수를 리스트에 삽입
    for(int i = 0; i < 15; i++) {
        if(list[i].score < new_score) {
            for(int j = 14; j > i; j--) { // 리스트의 내림차순으로 정렬
                list[j] = list[j - 1];
            }
            // 새로운 점수를 삽입
            list[i].score = new_score;
            list[i].count = count;
            break; // 삽입 후 반복문 종료
        }
    }
}

void ofApp::freememory() {
    // 모든 데이터 초기화, 단 랭크 리스트는 데이터를 유지한다
    ofClear(255, 255, 255); // 화면 초기화
    maze.clear();
    vector<vector<char>>().swap(maze);
    LINE.clear();
    vector<Line>().swap(LINE);
    visit.clear();
    vector<vector<bool>>().swap(visit);
    PATH.clear();
    vector<Line>().swap(PATH);

    while (!b_path.empty()) b_path.pop();
    while (!r_path.empty()) r_path.pop();

    draw_flag = 0;
    start_flag = 0;
    hint_flag = 0;
    end_flag = 0;

    hint_line.clear();
    count_move = 0;
    
    visited.resize(HEIGHT, vector<bool>(WIDTH, false));
    visited_lines.clear();
}
