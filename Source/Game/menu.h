#pragma once
#include "../Library/gameutil.h"

namespace game_framework
{
    class menu : public CMovingBitmap
    {
    public:
        void loadObject();
        void ShowCongrats();
        void ShowGameOver();
        void ShowGamePaused();
        void ShowMenuSelectArrow();
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool GameOverChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool CongratsChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
    private:
        // Congratulations pop up
        CMovingBitmap congratulations;
        CMovingBitmap nextLevel;
        CMovingBitmap mainMenu;

        // Game over pop up
        CMovingBitmap gameOver;
        CMovingBitmap tryAgain;
        /* For the main-menu, re-use from above */

        // Init menu
        CMovingBitmap title;
        
        // Game paused pop up
        CMovingBitmap gamePaused;

        // The selector for each pop up
        CMovingBitmap selectArrow;

        // To indicate the select arrow state
        int menuSelector = 1;

    };
}
