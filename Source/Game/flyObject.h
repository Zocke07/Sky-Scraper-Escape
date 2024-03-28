#pragma once
#include "../Library/gameutil.h"
namespace game_framework {
	class flyObject
	{
	public:
		flyObject();		// constructor
		void init();
		int getResourceShow();

		void jump();
		
		void onMove();
		void setMoveLeft(bool flag);
		void setMoveRight(bool flag);
		void setMoveUp(bool flag);		// dev mode
		void setMoveDown(bool flag);	// dev mode
		void jumpCharge(bool flag);

		void setTopCollision(int flag);
		void setBottomCollision(int flag);
		void setLeftCollision(int flag);
		void setRightCollision(int flag);
		
		void onShow();
		int getX();
		int getYshow();		// 提供mygame_run顯示用座標
		int getYactual();	// 提供CGameMap判斷位置用座標
		int getVelocityY();
		int getVelocityX();
		int getInitialVelocity();
		int getDirection();
		bool getCharging();
		bool getMovingRight();
		bool getMovingLeft();

		void nextResource();
		void previousResource();

	protected:
		// void setState();

		/*
		// 人物狀態
		enum characterState {
			motionless,		// 在表面上無移動
			movingLeft,		// 平面上往左移動
			movingRight,	// 平面上往右移動
			jumping,		// 跳躍/上升中
			falling,		// 落下中
			fallAndHit,		// 落下途中撞擊
			charging		// 跳躍蓄力中
		};
		*/

		// bool isOnSurface;		// 無垂直移動
		bool isMovingLeft;		// 平面上往左移動
		bool isMovingRight;		// 平面上往右移動
		bool isMovingUp;		// 往上移動中 (dev mode)
		bool isMovingDown;		// 往下移動中 (dev mode)
		bool isCharging;		// 跳躍蓄力中
		bool isJumping;			// 跳躍中(含上升/下降)
		bool isFalling;			// 落下中
		bool hitWhenFalling;	// 落下途中撞擊

		int topCollision;		// 上方碰撞
		int bottomCollision;	// 下方碰撞, 0:無碰撞, 1:下方為平面, 2:下方為往右斜面, 3:下方為往左斜面
		int leftCollision;		// 左碰撞
		int rightCollision;	// 右碰撞

		// 角色面對方向
		enum direction_flag {
			LEFT, 
			RIGHT
		};

		int state;
		int direction;
		// bool isFacingRight;		// 是否面向右邊

		// 座標相關
		int X;			// X座標
		int Yactual;	// 實際Y座標
		int Yshow;		// 顯示Y座標

		const int height = 48;
		const int width = 48;

		// 移動速度相關
		int initialVelocity;	// 跳躍加速度
		int velocityY;			// 垂直速度
		int velocityX;			// 水平速度
		int velocityJumpX;		// 跳躍時的水平速度
		const int gravity = 3;		// 重力加速度

		// 顯示哪張圖
		int bitmapShow;

	};
}


