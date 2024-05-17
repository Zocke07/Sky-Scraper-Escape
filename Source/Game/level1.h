#pragma once
#include "levelInit.h"
#include "obstacle.h"


namespace levels
{
    class level1 : public levelInit
    {
    public:
        void OnBeginState() override;
        void OnShow() override;
        void OnMove() override;
        void loadObject();
        void moveObstacle();
    private:
        int pointSpeedDeficit = 0;
        int obstacleDistance = 0;
        int pathDifference = 0;
        int accelerationConst = 0;
        int pathHeight = 0;
        int pathLocation = 0;
        int counter = 0;
        int obstacleNum = 1;
        vector<game_framework::obstacle> building = vector<game_framework::obstacle>(obstacleNum);
        vector<game_framework::obstacle> cloud = vector<game_framework::obstacle>(obstacleNum);
    };
}

