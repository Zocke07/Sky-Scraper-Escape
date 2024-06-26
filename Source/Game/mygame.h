/*
 * mygame.h: This file stores the interface of the game's own class
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#pragma once
#include "flyObject.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"
#include "level6.h"
#include "level7.h"
#include "levelSelect.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// Numbers that define various sound effects
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// This class is the game opening screen object of the game.
	// You must understand the Implementation of each Member function
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit() override;  								// Game initial values and graphics settings
		void OnBeginState() override;							// Set variables required for each replay
		void OnKeyUp(UINT, UINT, UINT) override; 				// Handle keyboard Up action
		void OnLButtonDown(UINT nFlags, CPoint point) override;  // Handling mouse movements
	protected:
		void OnShow() override;									// Shows this state of the game's screen
	private:
		CMovingBitmap logo;								// The logo of csie
		CMovingBitmap background;
		menu theMenu;
		bool toRun = false;
		bool MainMenu = true;
	};

	/////////////////////////////////////////////////////////////////////////////
	// This class is the game execution object of the game. The main game programs are all here.
	// You must understand the Implementation of each Member function
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun() override;
		void OnBeginState() override;							// Set variables required for each replay
		void OnInit() override;  								// Game initial values and graphics settings
		void OnKeyDown(UINT, UINT, UINT) override;
		void OnKeyUp(UINT, UINT, UINT) override;
		void OnLButtonDown(UINT nFlags, CPoint point) override;  // Handling mouse movements
		void OnLButtonUp(UINT nFlags, CPoint point) override;	// Handling mouse movements
		void OnMouseMove(UINT nFlags, CPoint point) override;	// Handling mouse movements 
		void OnRButtonDown(UINT nFlags, CPoint point) override;  // Handling mouse movements
		void OnRButtonUp(UINT nFlags, CPoint point) override;	// Handling mouse movements
	protected:
		void OnMove() override;									// Moving game element
		void OnShow() override;									// Shows this state of the game's screen
	private:
		levels::levelSelect theMainMenu;
		levels::level1 theLevel1;
		levels::level2 theLevel2;
		levels::level3 theLevel3;
		levels::level4 theLevel4;
		levels::level5 theLevel5;
		levels::level6 theLevel6;
		levels::level7 theLevel7;
		std::vector<levels::levelInit*> allLevels = {&theMainMenu, &theLevel1, &theLevel2, &theLevel3, &theLevel4, &theLevel5, &theLevel6, &theLevel7};

		levels::levelInit* current = allLevels[currentLevel];
		bool musicPlayed = false;
	};

	/////////////////////////////////////////////////////////////////////////////
	// This class is the end state of the game (Game Over)
	// You must understand the Implementation of each Member function
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);

		void OnBeginState() override;							// Set variables required for each replay
		void OnInit() override;
	protected:

		void OnMove() override;									// Moving game element
		void OnShow() override;									// Shows this state of the game's screen
	private:
		int counter;	// Countdown counter
	};

}
