#pragma once
#include "levelInit.h"

namespace levels
{
    class level5 : public levelInit
    {
    public:
        void OnBeginState() override;
        void OnShow() override;
        void OnMove() override;
        void loadObject();
        void moveObstacle();
        void flashObstacle();
        std::vector<writeText> getText(int r, int g, int b) override;
    private:
        vector<game_framework::CMovingBitmap> building = vector<game_framework::CMovingBitmap>(obstacleNum);
        vector<game_framework::CMovingBitmap> cloud = vector<game_framework::CMovingBitmap>(obstacleNum);
        int obstacleTimer = 120;
        bool showObstacle = true;
    };
}
