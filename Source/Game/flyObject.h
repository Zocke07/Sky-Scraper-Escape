#pragma once
#include "../Library/audio.h"
#include "../Library/gameutil.h"
namespace game_framework\
{
    class flyObject : public CMovingBitmap
    {
    public:
        flyObject(); // Constructor
        void init();
        void load();
        bool isCollide();
        bool isJumping();
        void setCollide(bool flag);
        void setJumping(bool flag);
        void getCurrentJump();
        void movement();
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        bool isCheat();
        void setCheat(bool flag);
        void applyGravity();
        void appplyJump();
        void setReverseGravity(bool flag);
        bool isReverseGravity();
    private:
        bool cheat;
        bool jumping;
        bool collide;
        bool reverseGravity;
        int currentJump;
        int gravityConst = 4;
        int maxJumpHeight = 0;
        int jumpConst = 50;
    };
}
