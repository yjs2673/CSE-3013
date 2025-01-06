#pragma once

#include "ofMain.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>

struct Line {
    float sX = 0, sY = 0, eX = 0, eY = 0;
};

struct Direct {
    bool is_b, up, down, left, right;
};

struct Block {
    int x = 0, y = 0;
};

struct score_rank{
    int score;
    int count;
};

class ofApp : public ofBaseApp{

	public:
        int mag;
        int draw_flag;
        int start_flag;
        int hint_flag;
        int end_flag;
        int game_count = 0;
        int draw_finish;
        vector<Line> LINE;
        vector<Line> PATH;

        score_rank list[15];
    
        stack<Block> m_path;
        queue<Block> b_path;
        queue<Block> r_path;

        int HEIGHT = 0, WIDTH = 0;
        int height, width;
        int count_move = 0;
    
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        bool readFile();
        void maze_line(Line line);
        void path_line(Line line);
        void freememory();
        bool hint();
    
        void make_list(int new_score, int count);
		
        vector<vector<char>> maze;
        vector<vector<bool>> visit;
};
