#include "stdafx.h"
#include "mainMenu.h"

using namespace levels;

void mainMenu::OnBeginState()
{
    levelInit::OnBeginState();

    theMenu.loadObject();
}


void mainMenu::OnShow()
{
    background.ShowBitmap();
    if (MainMenu == true)
    {
        theMenu.ShowMainMenu();
    }
    else
    {
        theMenu.ShowSelectLevels();
    }
}


void mainMenu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (MainMenu == true)
    {
        theMenu.OnKeyDownVertical(nChar, nRepCnt, nFlags);

        // If the user choose to Play Game, will go to state run
        toRun = theMenu.MainMenuChoose(nChar, nRepCnt, nFlags);

        if (toRun == true)
        {
            toRun = false;
            setNextLevel(true);
        }
        // If not, will choose level
        else if (toRun == false && nChar == VK_SPACE)
        {
            MainMenu = false;
        }
    }
    else
    {
        theMenu.OnKeyDownHorizontal(nChar, nRepCnt, nFlags);

        chosenLevel = theMenu.LevelChoose(nChar, nRepCnt, nFlags);

        if (chosenLevel == 0 && nChar == VK_SPACE)
        {
            MainMenu = true; // Go to main menu
        }
        else if (chosenLevel > 0 && chosenLevel < 8 && nChar == VK_SPACE)
        {
            setChooseLevel(true);
            MainMenu = true;
        }
    }
}
