﻿#pragma once
#include "levelInit.h"

namespace levels
{
	class level2 : public levelInit
	{
	public:
		void OnBeginState() override;
		void OnShow() override;
		void OnMove() override;
		void loadObject();
		void moveObstacle();
	private:
		vector<game_framework::CMovingBitmap> building = vector<game_framework::CMovingBitmap>(obstacleNum);
		vector<game_framework::CMovingBitmap> cloud = vector<game_framework::CMovingBitmap>(obstacleNum);
	};
}