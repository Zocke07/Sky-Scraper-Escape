﻿#pragma once
#include <string>
#include <vector>

#include "menu.h"
#include "flyObject.h"

using namespace std;

namespace levels
{
    // Components needed to write the altitude and point
    struct theCoordinate
    {
        int x;
        int y;
    };
    struct writeText
    {
        string text;
        theCoordinate position;
        int color;
        int size;
    };

}

namespace levels
{
    class levelInit
    {
    public:
        // Imitates mygame_run functions
        virtual void OnBeginState();  
        virtual void OnMove();
        virtual void OnInit();
        virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
        virtual void OnLButtonDown(UINT nFlags, CPoint point);
        virtual void OnLButtonUp(UINT nFlags, CPoint point);
        virtual void OnMouseMove(UINT nFlags, CPoint point);
        virtual void OnRButtonDown(UINT nFlags, CPoint point);
        virtual void OnRButtonUp(UINT nFlags, CPoint point);
        virtual void OnShow();
        
        // To handle levels in the mygame_run
        bool isRetry();
        bool isPause();
        bool isToInit();
        bool isNextlevel();
        
        vector<writeText> getText(); // To write current current altitude and point

        void setToInit(bool flag); // To go from level's run state to the init state
        void setRetry(bool flag); // To refresh the level's run state
        void setNextLevel(bool flag);
        
        // Additional main functions
        void loadBackground();
        
    protected:
		int xMax = 1193;
		int yMax = 652;
		int obstacleXDimension = 103;
		int obstacleYDimension = 667;
		int obstacleMovementConst = 3;
		int obstacleSpeed = 0;
		int time = 0;
        int point = 0;  // Current level's point
        bool pause = false;     // Pausing the game
        bool congrats = false;  // If the level is finished
        bool toInit = false;    // Triggers GoToGameState(GAME_STATE_INIT)
        bool retry = false;     // Triggers GoToGameState(GAME_STATE_RUN) in the same level
        bool nextLevel = false; // Triggers GoToGameState(GAME_STATE_RUN) to the next level
        game_framework::CMovingBitmap background;   // Background image
        game_framework::CMovingBitmap explosion;    // Explosion image when character crashes
        game_framework::flyObject character;    // The player
        game_framework::menu theMenu;   // All in game menu handler
    };
}