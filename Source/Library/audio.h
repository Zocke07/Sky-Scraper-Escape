/*
 * gamelib.h: This file stores the interface of game-related classes
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
 * History:
 *   2002-03-04 V3.1
 *		1. Add ENABLE_AUDIO flag.
 *   2004-03-02 V4.0
 *		1. Add CInteger class.
 *      2. Add CGameState abstract class and adjust CGame to apply
 *         state pattern for switching game states.
 *   2004-03-08 V4.1
 *      1. Add OPEN_AS_FULLSCREEN flag.
 *      2. The Game Engine now becomes a framework.
 *   2005-07-28
 *      1. Add GAME_ASSERT macro to display run time errors gracefully.
 *      2. Remove CDDarw::CheckSystem(). It is useless.
 *   2005-09-08
 *      1. Eliminate CSpecialEffect::Abort. It is useless now.
 *      2. Add SHOW_GAME_CYCLE_TIME flag.
 *      3. Add DEFAULT_BG_COLOR flag;
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Remove constantness of CAnimation::DELAY_COUNT.
 *      3. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      4. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      5. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap().
 *         Defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
*/
#pragma once
#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// This class will create a DirectX Audio object to provide sound effects
// You must understand the usage of each Public Interface, but you don¡¦t need to understand Implementation.
// Note: Sound effect files can be WAVE files (.wav) or MIDI files (.mid).
//           Several sound effect files can be mixed (simultaneously) played, but
//           only one MIDI file can be mixed, and two MIDI files cannot be played at the same time.
//           If two MIDI files are played at the same time, the previous MIDI file will be stopped.
/////////////////////////////////////////////////////////////////////////////

namespace game_framework {

class CAudio {
public:
	~CAudio();
	void           Close();						// Close the Direct Sound interface
	static CAudio* Instance();					// Get the instance of CAudio
	bool           Load(unsigned, char *);		// Load the sound number i, specify the MIDI file
	bool           Open();						// Open the Direct Sound interface
	void		   Pause();						// Pause all sound effects
	void           Play(unsigned, bool=false);	// Start playing the sound number i and specify whether to repeat it.
	void		   Resume();					// Resume paused sound effects
	void           SetPowerResume();			// Restore power
	void           Stop(unsigned);				// Stop playing the sound number i
private:
	class Info {
	public:
		Info() {
			repeat = isGood = false;
		}
		bool repeat, isGood;
		string fileName;
	};
	static void MCIThread(HANDLE);	// MCI thread method
	static void ExecuteMciCommand(char *); // 
	void SendMciCommand(char *);	// 
	CAudio();						// private constructor
	static CAudio		audio;		// automatic destruct
	map<int, Info>		info;
	bool				isOpened;
    HANDLE				hThread;	// MCI command thread
	HANDLE				hWriteEnd;	// Pipe write handle for thread
	const static int	MAX_MCI_COMMAND_SIZE = 400;
};

}
