#pragma once
#include "levelInit.h"
#include "obstacle.h"

#define obstacleNum 20

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
		int pointSpeedDeficit = 0;
		int pathDifference = 0;
		int accelerationConst = 1;
		int pathHeight[obstacleNum];
		int pathLocation[obstacleNum];
		int obstacleDistance[obstacleNum];
		vector<game_framework::obstacle> building = vector<game_framework::obstacle>(obstacleNum);
		vector<game_framework::obstacle> cloud = vector<game_framework::obstacle>(obstacleNum);
	};
}
