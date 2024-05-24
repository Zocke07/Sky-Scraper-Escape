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
        void ShowMainMenu();
        void ShowSelectLevels();
        void ShowMenuSelectArrow();
        void ShowLevelSelectArrow();
        
        void OnKeyDownVertical(UINT nChar, UINT nRepCnt, UINT nFlags);
        void OnKeyDownHorizontal(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool MainMenuChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool GameOverChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool CongratsChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
        int LevelChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
    private:
        CMovingBitmap background;
        
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
        CMovingBitmap playGame;
        CMovingBitmap selectLevel;

        // Level Selection
        CMovingBitmap levelSelections;
        CMovingBitmap back;
        
        // Game paused pop up
        CMovingBitmap gamePaused;

        // The selector for each pop up
        CMovingBitmap selectArrow;
        CMovingBitmap selectLevelArrow;

        // To indicate the select arrow state
        int menuSelector = 1;
        int levelSelector = 0;

    };
}
