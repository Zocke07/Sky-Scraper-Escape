#include "stdafx.h"
#include "menu.h"

using namespace game_framework;

void menu::loadObject()
{
    congratulations.LoadBitmapByString({"Resources/Congrats.bmp"}, RGB(0, 100, 0));
    nextLevel.LoadBitmapByString({"Resources/NextLevel.bmp"}, RGB(0, 100, 0));
    mainMenu.LoadBitmapByString({"Resources/MainMenu.bmp"}, RGB(0, 100, 0));

    gameOver.LoadBitmapByString({"Resources/GameOver.bmp"}, RGB(0, 100, 0));
    tryAgain.LoadBitmapByString({"Resources/TryAgain.bmp"}, RGB(0, 100, 0));
}

void menu::ShowCongrats()
{
    congratulations.SetTopLeft(490, 200);
    congratulations.ShowBitmap();
    nextLevel.SetTopLeft(652, 320);
    nextLevel.ShowBitmap();
    mainMenu.SetTopLeft(655, 386);
    mainMenu.ShowBitmap();
}

void menu::ShowGameOver()
{
    gameOver.SetTopLeft(355, 200);
    gameOver.ShowBitmap();
    tryAgain.SetTopLeft(411, 294);
    tryAgain.ShowBitmap();
    mainMenu.SetTopLeft(407, 360);
    mainMenu.ShowBitmap();
}
