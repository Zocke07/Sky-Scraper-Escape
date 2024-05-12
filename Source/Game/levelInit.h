﻿#pragma once
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
        vector<writeText> getText();
        int getPoint();
        bool getPause();
        void resetPoint();
        void addPoint();
        void setPause(bool flag);
        void OnBeginState();
        void OnMove();
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        void OnShow();
        void loadObject();
        void loadBackground();
        void moveObstacle();
        bool getToInit();
    protected:
        int point = 0;
        bool isToInit = false;
        int accelerationConst = 1;
        int pathHeight = 0;
        int pathLocation = 0;
        int obstacleDistance = 0;
        int pathDifference = 0;
        int obstacleMovementConst = 3;
        int obstacleSpeed = 0;
        int counter = 0;
        int obstacleNum = 20;
        int time = 0;
        bool isPause = false;
        int pointSpeedDeficit = 0;
        int selector = 1;
        bool congrats = false;
        game_framework::CMovingBitmap background;
        game_framework::CMovingBitmap explosion;
        game_framework::CMovingBitmap selectArrow;
        game_framework::flyObject character;
        vector<game_framework::CMovingBitmap> building = vector<game_framework::CMovingBitmap>(obstacleNum);
        vector<game_framework::CMovingBitmap> cloud = vector<game_framework::CMovingBitmap>(obstacleNum);
        game_framework::menu theMenu;
    };
}