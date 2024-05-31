#pragma once
#include "levelInit.h"

namespace levels
{
	class level3 : public levelInit
	{
	public:
		void OnBeginState() override;
		void OnShow() override;
		void OnMove() override;
		void loadObject();
		void moveObstacle();
	protected:
		game_framework::CMovingBitmap night;
	private:
		vector<game_framework::CMovingBitmap> building = vector<game_framework::CMovingBitmap>(obstacleNum);
		vector<game_framework::CMovingBitmap> cloud = vector<game_framework::CMovingBitmap>(obstacleNum);
	};
}