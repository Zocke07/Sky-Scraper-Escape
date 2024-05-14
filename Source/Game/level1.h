#pragma once
#include "levelInit.h"

namespace levels
{
    class level1 : public levelInit
    {
    public:
        void OnBeginState();
        void OnShow();
        void OnMove();
        void loadObject();
        void moveObstacle();
    private:
        int pointSpeedDeficit = 0;
        int obstacleSpeed = 0;
        int obstacleDistance = 0;
        int pathDifference = 0;
        int accelerationConst = 0;
        int pathHeight = 0;
        int pathLocation = 0;
        int obstacleMovementConst = 3;
        int counter = 0;
        int obstacleNum = 1;
        int time = 0;
        vector<game_framework::CMovingBitmap> building = vector<game_framework::CMovingBitmap>(obstacleNum);
        vector<game_framework::CMovingBitmap> cloud = vector<game_framework::CMovingBitmap>(obstacleNum);
    };
}

