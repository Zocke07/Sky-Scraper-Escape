#include "stdafx.h"
#include "flyObject.h"

using namespace object;

flyObject::flyObject() {}

void flyObject::init()
{
    jumping = false;
    collide = false;
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

void flyObject::gravity()
{
    if (this->GetTop() < 590 && this->isJumping() == false) {
        this->SetTopLeft(this->GetLeft(), this->GetTop() + gravityConst);
    }
}

void flyObject::jump()
{
    if (this->isJumping() == true){
        if (this->GetTop() > maxJumpHeight && this->GetTop() > currentJump)
            {
                this->SetTopLeft(this->GetLeft(), this->GetTop() - gravityConst);
            }
        else
            {
                this->setJumping(false);
            }
    }
}

void flyObject::getCurrentJump()
{
    currentJump = this->GetTop() - jumpConst;
}