#include "stdafx.h"
#include "levelInit.h"

#include "level1.h"

using namespace levels;

void levelInit::OnBeginState()
{
    point = 0;
    pause = false;
    congrats = false;
    retry = false;
    character.init();
    loadBackground();
    character.load();
    theMenu.loadObject();
}

void levelInit::OnMove()
{
    character.movement();
}

void levelInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_ESCAPE)
    {
        if (this->isPause() == false) {
            setPause(true);
        }
        else {
            setPause(false);
        }
    }
    
    if (character.isCollide() == false)
    {
        character.OnKeyDown(nChar, nRepCnt, nFlags);
    }

    else if (character.isCollide() == true) // When character crashes
    {
        theMenu.OnKeyDown(nChar, nRepCnt, nFlags);

        // If the user choose to go to main menu, will go to init
        isToInit = theMenu.GameOverChoose(nChar, nRepCnt, nFlags);

        // If not, will retry level
        if (isToInit == false  && nChar == VK_RETURN)
        {
            setRetry(true);
        }
    }

    if (isCongrats() == true) // When character reaches target point
    {
        theMenu.OnKeyDown(nChar, nRepCnt, nFlags);

        // If the user choose to go to main menu, will go to init
        isToInit = theMenu.CongratsChoose(nChar, nRepCnt, nFlags);

        // If not, will go to next stage
        if (isToInit == false)
        {
            // Currently no implementation yet
        }
    }
}

bool levelInit::getToInit()
{
    return isToInit;
}


void levelInit::OnShow()
{
    background.ShowBitmap();
    character.ShowBitmap();
    
    if (character.isCollide() == true)
    {
        explosion.SetTopLeft(character.GetLeft() + 60, character.GetTop());
        explosion.ShowBitmap();
    }
    
    if (isPause() == true && character.isCollide() == false && isCongrats() == false)
    {
        theMenu.ShowGamePaused();
    }
    
    if (isCongrats() == true) {
        setPause(true);
        theMenu.ShowCongrats();
    }
}
    
void levelInit::loadBackground()
{
    background.LoadBitmap("Resources/Background.bmp");
    background.SetTopLeft(0, 0);

    explosion.LoadBitmapByString({"Resources/Explosion1.bmp"}, RGB(0, 100, 0));
}

std::vector<writeText> levelInit::getText()
{
    vector<writeText> texts;
    texts.push_back({"Altitude: " + to_string(670-character.GetTop()-80), {20, 50}, RGB(0, 0, 0), 20});
    texts.push_back({"Point   : " + to_string(getPoint()), {20, 70}, RGB(0, 0, 0), 20});
    return texts;
}

int levelInit::getPoint()
{
    return point;
}

bool levelInit::isPause()
{
    return pause;
}

void levelInit::setPause(bool flag)
{
    pause = flag;
}

void levelInit::resetPoint()
{
    point = 0;
}

void levelInit::addPoint()
{
    point += 1;
}

void levelInit::setToInit(bool flag)
{
    isToInit = flag;
}

void levelInit::setRetry(bool flag)
{
    retry = flag;
}

bool levelInit::isRetry()
{
    return retry;
}

bool levelInit::isCongrats()
{
    return congrats;
}

void levelInit::setCongrats(bool flag)
{
    congrats = flag;
}


