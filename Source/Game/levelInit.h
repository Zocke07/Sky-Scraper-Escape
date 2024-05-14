#pragma once
#include <string>
#include <vector>

#include "menu.h"
#include "flyObject.h"

using namespace std;

namespace levels
{
    struct theCoordinate
    {
        int x;
        int y;
    };
    struct writeText
    {
        string text;
        theCoordinate position;
        int color;
        int size;
    };

}

namespace levels
{
    class levelInit
    {
    public:
        // Getter
        int getPoint();
        bool isRetry();
        bool isPause();
        bool getToInit();
        bool isCongrats();
        vector<writeText> getText();

        // Setter
        void resetPoint();
        void addPoint();
        void setPause(bool flag);
        void setToInit(bool flag);
        void setRetry(bool flag);
        void setCongrats(bool flag);

        void OnBeginState();
        void OnMove();
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        void OnShow();
        void loadBackground();
    protected:
        int point = 0;
        bool isToInit = false;
        bool pause = false;
        bool congrats = false;
        bool retry = false;
        game_framework::CMovingBitmap background;
        game_framework::CMovingBitmap explosion;
        game_framework::flyObject character;
        game_framework::menu theMenu;
    };
}