#include "stdafx.h"
#include "flyObject.h"

using namespace game_framework;

flyObject::flyObject() {}

void flyObject::init()
{
    reverseGravity = false;
    jumping = false;
    collide = false;
    cheat = false;
    gravityConst = 4;
    jumpConst = 50;
}

void flyObject::load()
{
    this->LoadBitmapByString({"Resources/Plane.bmp"}, RGB(0, 100, 0));
    this->SetTopLeft(180, 270);
}


bool flyObject::isCollide()
{
    return collide;
}

bool flyObject::isJumping()
{
    return jumping;
}

void flyObject::setCollide(bool flag)
{
    collide = flag;
}

void flyObject::setJumping(bool flag)
{
    jumping = flag;
}

void flyObject::getCurrentJump()
{
    if (reverseGravity == false)
    {
        currentJump = this->GetTop() - jumpConst;
    }
    else
    {
        currentJump = this->GetTop() + jumpConst;
    }
}

void flyObject::movement()
{
    applyGravity();
    appplyJump();
}

void flyObject::applyGravity()
{
    if (this->isJumping() == false) {
        if (reverseGravity == true) {
            if (this->GetTop() > 0) {
                this->SetTopLeft(this->GetLeft(), this->GetTop() - gravityConst);
            }
        } else {
            if (this->GetTop() < 590) {
                this->SetTopLeft(this->GetLeft(), this->GetTop() + gravityConst);
            }
        }
    }
}

void flyObject::appplyJump()
{
    if (this->isJumping() == true) {
        if (reverseGravity == true) {
            if (this->GetTop() < 600 && this->GetTop() < currentJump) {
                this->SetTopLeft(this->GetLeft(), this->GetTop() + gravityConst);
            } else {
                this->setJumping(false);
            }
        } else {
            if (this->GetTop() > maxJumpHeight && this->GetTop() > currentJump) {
                this->SetTopLeft(this->GetLeft(), this->GetTop() - gravityConst);
            } else {
                this->setJumping(false);
            }
        }
    }
}

void flyObject::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_SPACE)
    {
        CAudio::Instance()->Play(0, false);
        
        this->getCurrentJump();
        this->setJumping(true);
    }

    if (nChar == 0x43) // Press C for cheat mode
    {
        if (cheat == true)
        {
            cheat = false;
        }
        else
        {
            cheat = true;
        }
    }
}

bool flyObject::isCheat()
{
    return cheat;
}

void flyObject::setCheat(bool flag)
{
    cheat = flag;
}

void flyObject::setReverseGravity(bool flag)
{
    reverseGravity = flag;
}

bool flyObject::isReverseGravity()
{
    return reverseGravity;
}

