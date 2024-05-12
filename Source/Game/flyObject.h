#pragma once
#include "../Library/gameutil.h"

namespace object
{
    class flyObject : public game_framework::CMovingBitmap
    {
    public:
        flyObject(); // Constructor
        void init();
        void load();
        bool isCollide();
        bool isJumping();
        void setCollide(bool flag);
        void setJumping(bool flag);
        void gravity();
        void jump();
        void getCurrentJump();
    private:
        bool jumping;
        bool collide;
        int currentJump = 0;
        int gravityConst = 4;
        int maxJumpHeight = 0;
        int jumpConst = 50;
    };
}
