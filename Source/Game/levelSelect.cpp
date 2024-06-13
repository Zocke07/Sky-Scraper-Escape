#include "stdafx.h"
#include "levelSelect.h"

using namespace levels;

void levelSelect::OnBeginState()
{
    levelInit::OnBeginState();

    theMenu.loadObject();
}


void levelSelect::OnShow()
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


void levelSelect::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
        else if (toRun == false && nChar == VK_RETURN)
        {
            MainMenu = false;
        }
    }
    else
    {
        theMenu.OnKeyDownHorizontal(nChar, nRepCnt, nFlags);

        chosenLevel = theMenu.LevelChoose(nChar, nRepCnt, nFlags);

        if (chosenLevel == 0 && nChar == VK_RETURN)
        {
            MainMenu = true; // Go to main menu
        }
        else if (chosenLevel > 0 && chosenLevel < 8 && nChar == VK_RETURN)
        {
            setChooseLevel(true);
            MainMenu = true;
        }
    }
}
