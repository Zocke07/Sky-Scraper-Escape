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
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool GameOverChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool CongratsChoose(UINT nChar, UINT nRepCnt, UINT nFlags);
    private:
        CMovingBitmap congratulations;
        CMovingBitmap nextLevel;
        CMovingBitmap mainMenu;

        CMovingBitmap gameOver;
        CMovingBitmap tryAgain;

        CMovingBitmap gamePaused;

        CMovingBitmap selectArrow;

        int selector = 1;

    };
}
