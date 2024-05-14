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

    gamePaused.LoadBitmapByString({"Resources/GamePaused.bmp"}, RGB(0, 100, 0));

    selector = 1;
    selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
    selectArrow.SetTopLeft(300, 312);
}

void menu::ShowCongrats()
{
    congratulations.SetTopLeft(340, 180);
    congratulations.ShowBitmap();
    nextLevel.SetTopLeft(400, 300);
    nextLevel.ShowBitmap();
    mainMenu.SetTopLeft(405, 366);
    mainMenu.ShowBitmap();
    
    selectArrow.ShowBitmap();
}

void menu::ShowGameOver()
{
    gameOver.SetTopLeft(355, 200);
    gameOver.ShowBitmap();
    tryAgain.SetTopLeft(411, 294);
    tryAgain.ShowBitmap();
    mainMenu.SetTopLeft(407, 360);
    mainMenu.ShowBitmap();
    
    selectArrow.ShowBitmap();
}

void menu::ShowGamePaused()
{
    gamePaused.SetTopLeft(355, 250);
    gamePaused.ShowBitmap();
}

void menu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_DOWN) // Move arrow down
    {
        if (selector < 2)
        {
            selector += 1;
            selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() + 66);
        }
    }
    else if (nChar == VK_UP) // Move arrow up
    {
        if (selector > 1)
        {
            selector -= 1;
            selectArrow.SetTopLeft(selectArrow.GetLeft(), selectArrow.GetTop() - 66);
        }
    }
}

bool menu::GameOverChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (selectArrow.GetTop() == 312 && nChar == VK_RETURN) // Try again
    {
        return false;
    }
    if (selectArrow.GetTop() == 378 && nChar == VK_RETURN) // Back to menu
    {
        return true;
    }
    return false;
}

bool menu::CongratsChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (selectArrow.GetTop() == 312 && nChar == VK_RETURN) // Go to next stage button
    {
        return false;
    }
    if (selectArrow.GetTop() == 378 && nChar == VK_RETURN) // Go to main menu button
    {
        return true;
    }
    return false;
}
