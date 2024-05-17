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
    };
}
