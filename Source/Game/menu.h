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
    private:
        CMovingBitmap congratulations;
        CMovingBitmap nextLevel;
        CMovingBitmap mainMenu;

        CMovingBitmap gameOver;
        CMovingBitmap tryAgain;
    };
}
