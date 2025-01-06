#pragma once

#include "ofMain.h"
#include <string>
#include <vector>
#include <stack>

struct Line {
    float sX, sY, eX, eY;
};

struct Direct {
    bool is_b, up, down, left, right;
};

struct Block {
    int x = 0, y = 0;
};

class ofApp : public ofBaseApp{

	public:
        int mag;
        int draw_flag;
        int dfs_flag;
        int bfs_flag;
        int draw_finish;
        vector<Line> LINE;
    
        stack<Block> m_block;
        stack<Block> m_path;
        stack<Block> a_path;
    
        int x_move[4] = {0, 0, 1, -1};
        int y_move[4] = {1, -1, 0, 0};

        int HEIGHT = 0, WIDTH = 0;
        int height, width;
    
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
        void freememory();
		
        vector<vector<char>> maze;
        vector<vector<bool>> visit;
        vector<vector<Direct>> maze_block;
};
