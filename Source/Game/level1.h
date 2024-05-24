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
        vector<game_framework::obstacle> building = vector<game_framework::obstacle>(obstacleNum);
        vector<game_framework::obstacle> cloud = vector<game_framework::obstacle>(obstacleNum);
    };
}

