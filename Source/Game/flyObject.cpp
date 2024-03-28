#include "stdafx.h"
#include "flyObject.h"
#include "../Library/gameutil.h"
using namespace game_framework;

flyObject::flyObject() {

}

void flyObject::init() {
	// facing = right;
	// state = motionless;
	bitmapShow = 0;				// 顯示哪張圖
	X = 120;						// X座標
	Yactual = 240;				// 實際Y座標,580 demo:-41036
	Yshow = 240;				// 顯示用Y座標

	direction = RIGHT;			// 預設面向右
	initialVelocity = 0;
	velocityJumpX = 0;
	velocityX = 0;
	velocityY = 0;

	isMovingUp = false;			// dev mode
	isMovingDown = false;		// dev mode

	// isOnSurface = true;			// 平面上的狀態
	isMovingLeft = false;
	isMovingRight = false;
	
	isCharging = false;			//跳躍蓄力, 在平面上才可觸發 
	isJumping = false;			// 跳躍後觸發
	isFalling = false;			// 初速被重力減完後觸發
	hitWhenFalling = false;		// 落下途中遭受撞擊

	topCollision = 0;		// 上方碰撞
	bottomCollision = 0;	// 下方碰撞
	leftCollision = 0;		// 左碰撞
	rightCollision = 0;	// 右碰撞
}

int flyObject::getResourceShow() {
	// 計算Yshow
	return bitmapShow;
}

void flyObject::setMoveLeft(bool flag) {
	isMovingLeft = flag;
	if (bottomCollision == 1 && !isCharging) {
		direction = LEFT;			// 在平面上則變更為面對左
	}
	// else isMovingLeft = false;
}

void flyObject::setMoveRight(bool flag) {
	isMovingRight = flag;
	if (bottomCollision == 1 && !isCharging) {
		direction = RIGHT;			// 在平面上則變更為面向右
	}
	// else isMovingRight = false;
}

void flyObject::setMoveUp(bool flag) {
	isMovingUp = flag;
}

void flyObject::setMoveDown(bool flag) {
	isMovingDown = flag;
}

void flyObject::jumpCharge(bool flag) {
	if (bottomCollision == 1) {
		if (flag) {
			isCharging = flag;
			// isJumping = true;
		}
		else {
			jump();
			isCharging = false;
		}
	}
	else isCharging = false;		// 不在地面上則不蓄力
}

void flyObject::jump() {
	velocityY = -1*initialVelocity;					// 設置速度
	isJumping = true;
	if (isMovingLeft && !isMovingRight) {
		velocityX = -1* initialVelocity / 2;
	}
	else if (isMovingRight && !isMovingLeft) {
		velocityX = initialVelocity / 2;
	}
	initialVelocity = 0;							// 重設加速度
}


void flyObject::onShow() {

	/*	角色動畫:
		0: 預設向右圖	5: 向右跳躍圖		9: 落下途中受撞擊_右
		1: 預設向左圖	6: 向左跳躍圖		10: 落下途中受撞擊_左
		2: 向右走動畫	7: 向右落下圖		11: 從高處落地_右
		3: 向左走動畫	8: 向左落下圖		12: 從高處落地_左
		4: 跳躍蓄力
	*/

	if (isCharging) bitmapShow = 4;
	else {
		if (direction == LEFT) {		// 面向左
			if (velocityY > 0) bitmapShow = 8;
			else if (velocityY < 0) bitmapShow = 6;
			else if (velocityX < 0) bitmapShow = 3;
			else if(velocityY == 0 && bottomCollision == 1) bitmapShow = 1;
		}
		else {							// 面向右
			if (velocityY > 0) bitmapShow = 7;
			else if (velocityY < 0) bitmapShow = 5;
			else if (velocityX > 0) bitmapShow = 2;
			else if (velocityY == 0 && bottomCollision == 1) bitmapShow = 0;
		}
	}  	
}

void flyObject::onMove() {
	
	/*	角色動畫:
		0: 預設向右圖	5: 向右跳躍圖		9: 落下途中受撞擊_右
		1: 預設向左圖	6: 向右跳躍圖		10: 落下途中受撞擊_左
		2: 向右走動畫	7: 向右落下圖		11: 從高處落地_右
		3: 向左走動畫	8: 向左落下圖		12: 從高處落地_左
		4: 跳躍蓄力		
	*/
	
	const int STEP_SIZE = 7;

	// 物理狀態

	// gravity
	velocityY += gravity;
	
	// 在地面上
	if (bottomCollision == 1) {
		velocityY -= gravity;		// 地面反作用力

		if (isCharging) {			// 跳躍蓄力
			velocityX = 0;
			if (initialVelocity < 42) {
				if (initialVelocity < 10) initialVelocity = 10;
				else initialVelocity += 2;
			}
		}
		else {
			if (!isJumping) {
				if (isMovingRight) velocityX = STEP_SIZE;		// 向左移動
				else if (isMovingLeft) velocityX = -1 * STEP_SIZE;		// 向右移動
				else velocityX = 0;
			}

		}
	}
	else {		// 在空中

	}

	// 水平運動狀態
	if (velocityX > 0) {	// 向右移動
		if (rightCollision == 0) X += velocityX;
		else if (rightCollision != 1) X += (rightCollision-1);
	}
	else if (velocityX < 0) {		// 向左移動
		if (leftCollision == 0) X += velocityX;
		else if (leftCollision != 1) X -= (leftCollision - 1);
	}

	
	// 垂直運動狀態
	if (velocityY > 0) {	// 下降狀態
		if (bottomCollision == 0) Yactual += velocityY;
		else {
			// 落地前
			Yactual += (bottomCollision - 1);
			velocityY = 0;
			isJumping = false;
		}

	}
	else if (velocityY < 0) {	// 上升狀態
		if (topCollision == 0) Yactual += velocityY;
		else {
			Yactual -= (topCollision - 1);
			velocityY = 0;
		}
	}
	/*
	if (leftCollision == 1 || rightCollision == 1) velocityX = 0;

	

	
	// 向左移動
	if (isMovingLeft && leftCollision != 1) {
		if (leftCollision == 0) velocityX = -1*STEP_SIZE;
		// bitmapShow = 3;
	}

	// 向右移動
	if (isMovingRight && rightCollision != 1) {
		if (rightCollision == 0) velocityX = STEP_SIZE;
		// bitmapShow = 2;
	}

	// 向上移動
	if (isMovingUp && topCollision != 1) {		// dev
		velocityY = 0;
		if(topCollision == 0) Yactual -= STEP_SIZE;
		else Yactual -= (topCollision - 1);
	}

	// 向下移動
	if (isMovingDown && bottomCollision != 1) {		// dev
		velocityY = 0;
		if (bottomCollision == 0) Yactual += STEP_SIZE;
		else Yactual += (bottomCollision - 1);
	}

	// 重力(有上限), v = g*t
	if(velocityY < 50) velocityY += gravity;
	*/
	
	

	/*
	// 跳躍蓄力
	if (isCharging) {
		if (bottomCollision == 1) {
			if(initialVelocity < 25) initialVelocity++;
		}
	}

	// 重力
	if (isMovingUp == false) {
		
		if (velocityY < 40) {
			velocityY += gravity;		// 重力
		}

		if (bottomCollision == 0) {
			Yactual += velocityY;
		}
		else {
			Yactual += (bottomCollision - 1);
		}
		
	}

	if (bottomCollision == 1) {			// 下方碰撞
		if (velocityY > 0) velocityY = 0;		// 下降狀態則改為0
		velocityY -= gravity;			// 反作用力
	}
	*/

}

int flyObject::getX() {
	return X;
}

int flyObject::getYshow() {

	// 計算顯示座標，以左上點為基準
	if (Yactual >= 0 && Yactual <=710) {
		return Yactual;
	}
	else {
		Yshow = Yactual % 720 + 720;
		return Yshow;
	}
	
}

int flyObject::getYactual() {
	return Yactual;
}

int flyObject::getVelocityY() {
	return velocityY;
}

// dev mode 
void flyObject::nextResource() {
	if (bitmapShow < 12) bitmapShow += 1;
}

// dev mode
void flyObject::previousResource() {
	if (bitmapShow > 0) bitmapShow -= 1;
}


void flyObject::setTopCollision(int flag) {
	topCollision = flag;
}

void flyObject::setBottomCollision(int flag) {
	bottomCollision = flag;
}

void flyObject::setLeftCollision(int flag) {
	leftCollision = flag;
}

void flyObject::setRightCollision(int flag) {
	rightCollision = flag;
}

int flyObject::getVelocityX() {
	return velocityX;
}

int flyObject::getInitialVelocity() {
	return initialVelocity;
}

int flyObject::getDirection() {
	return direction;
}

bool flyObject::getCharging() {
	return isCharging;
}

bool flyObject::getMovingLeft() {
	return isMovingLeft;
}

bool flyObject::getMovingRight() {
	return isMovingRight;
}