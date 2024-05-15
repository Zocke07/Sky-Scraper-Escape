#pragma once
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
        
        vector<writeText> getText(); // To write current current altitude and point

        void setToInit(bool flag); // To go from level's run state to the init state
        void setRetry(bool flag); // To refresh the level's run state

        
        // Additional main functions
        void loadBackground();
        
    protected:
        int point = 0;  // Current level's point
        bool toInit = false;    // Triggers GoToGameState(GAME_STATE_INIT)
        bool pause = false;     // Pausing the game
        bool congrats = false;  // If the level is finished
        bool retry = false;     // Triggers GoToGameState(GAME_STATE_RUN)
        game_framework::CMovingBitmap background;   // Background image
        game_framework::CMovingBitmap explosion;    // Explosion image when character crashes
        game_framework::flyObject character;    // The player
        game_framework::menu theMenu;   // All in game menu handler
    };
}