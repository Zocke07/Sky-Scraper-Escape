﻿#pragma once
#include "levelInit.h"

namespace levels
{
    class level7 : public levelInit
    {
    public:
        void OnBeginState() override;
        void OnShow() override;
        void OnMove() override;
    };
}