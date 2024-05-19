#include "stdafx.h"
#include "menu.h"

using namespace game_framework;

void menu::loadObject()
{
    title.LoadBitmapByString({"Resources/Title.bmp"}, RGB(0, 100, 0));
    playGame.LoadBitmapByString({"Resources/PlayGame.bmp"}, RGB(0, 100, 0));
    selectLevel.LoadBitmapByString({"Resources/SelectLevel.bmp"}, RGB(0, 100, 0));

    levelSelections.LoadBitmapByString({"Resources/ChooseLevels.bmp"}, RGB(0, 100, 0));
    
    congratulations.LoadBitmapByString({"Resources/Congrats.bmp"}, RGB(0, 100, 0));
    nextLevel.LoadBitmapByString({"Resources/NextLevel.bmp"}, RGB(0, 100, 0));
    mainMenu.LoadBitmapByString({"Resources/MainMenu.bmp"}, RGB(0, 100, 0));

    gameOver.LoadBitmapByString({"Resources/GameOver.bmp"}, RGB(0, 100, 0));
    tryAgain.LoadBitmapByString({"Resources/TryAgain.bmp"}, RGB(0, 100, 0));

    gamePaused.LoadBitmapByString({"Resources/GamePaused.bmp"}, RGB(0, 100, 0));

    selectArrow.LoadBitmapByString({"Resources/SelectionArrow.bmp"}, RGB(0, 100, 0));
}

void menu::ShowCongrats()
{
    congratulations.SetTopLeft(340, 180);
    congratulations.ShowBitmap();
    nextLevel.SetTopLeft(400, 300);
    nextLevel.ShowBitmap();
    mainMenu.SetTopLeft(405, 366);
    mainMenu.ShowBitmap();
    
    ShowMenuSelectArrow();
}

void menu::ShowGameOver()
{
    gameOver.SetTopLeft(355, 200);
    gameOver.ShowBitmap();
    tryAgain.SetTopLeft(411, 294);
    tryAgain.ShowBitmap();
    mainMenu.SetTopLeft(407, 360);
    mainMenu.ShowBitmap();
    
    ShowMenuSelectArrow();
}

void menu::ShowGamePaused()
{
    gamePaused.SetTopLeft(355, 250);
    gamePaused.ShowBitmap();
}

void menu::ShowMainMenu()
{
    title.SetTopLeft(340, 190);
    title.ShowBitmap();
    playGame.SetTopLeft(411, 300);
    playGame.ShowBitmap();
    selectLevel.SetTopLeft(411, 361);
    selectLevel.ShowBitmap();

    ShowMenuSelectArrow();
}

void menu::ShowSelectLevels()
{
    levelSelections.SetTopLeft(340, 300);
    levelSelections.ShowBitmap();
}


void menu::ShowMenuSelectArrow()
{
    /*
     Arrow position:
     original (x, y) = (300, 312)
     When moving down, y + 66
     When moving up, y - 66
    */
    
    if (menuSelector == 1)
    {
        selectArrow.SetTopLeft(300, 312);
        selectArrow.ShowBitmap();
    }
    else if (menuSelector == 2)
    {
        selectArrow.SetTopLeft(300, 378);
        selectArrow.ShowBitmap();
    }
}

bool menu::LevelChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return true;
}



void menu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_DOWN) // Move arrow down
    {
        if (menuSelector < 2)
        {
            menuSelector += 1;
        }
    }
    else if (nChar == VK_UP) // Move arrow up
    {
        if (menuSelector > 1)
        {
            menuSelector -= 1;
        }
    }
}

bool menu::MainMenuChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (selectArrow.GetTop() == 312 && nChar == VK_SPACE) // Play Game
    {
        menuSelector = 1;   // Reset arrow position
        return true;
    }
    if (selectArrow.GetTop() == 378 && nChar == VK_SPACE) // Select Level
    {
        menuSelector = 1;   // Reset arrow position
        return false;
    }
    return false;
}


bool menu::GameOverChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (selectArrow.GetTop() == 312 && nChar == VK_RETURN) // Try again
    {
        menuSelector = 1;   // Reset arrow position
        return false;
    }
    if (selectArrow.GetTop() == 378 && nChar == VK_RETURN) // Back to menu
    {
        menuSelector = 1;   // Reset arrow position
        return true;
    }
    return false;
}

bool menu::CongratsChoose(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (selectArrow.GetTop() == 312 && nChar == VK_RETURN) // Go to next stage button
    {
        menuSelector = 1;   // Reset arrow position
        return false;
    }
    if (selectArrow.GetTop() == 378 && nChar == VK_RETURN) // Go to main menu button
    {
        menuSelector = 1;   // Reset arrow position
        return true;
    }
    return false;
}
