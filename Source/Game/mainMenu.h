#pragma once
#include "levelInit.h"

namespace levels
{
    class mainMenu : public levelInit
    {
    public:
        void OnBeginState() override;
        void OnShow() override;
        void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;
    private:
        game_framework::menu theMenu;
    };
}
