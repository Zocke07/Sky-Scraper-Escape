#pragma once

/////////////////////////////////////////////////////////////////////////////
// Define the three state constants used by CGame and CGameState
/////////////////////////////////////////////////////////////////////////////

enum GAME_STATES {
	GAME_STATE_INIT,
	GAME_STATE_RUN,
	GAME_STATE_OVER
};

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Macro for reporting program errors
// Note: Macro is used here to preserve the file name and line number, which is convenient for debugging.
/////////////////////////////////////////////////////////////////////////////

#define GAME_ASSERT(boolexp,str)											\
		if (!(boolexp)) {													\
			int id;															\
			char s[300]="";													\
			sprintf(s,"Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"		\
				"\n\n(Press Retry to debug the application, "				\
				"if it is executed in debug mode.)"							\
				"\n(Press Cancel otherwise.)",								\
				 str , __FILE__,__LINE__);									\
			id = AfxMessageBox(s, MB_RETRYCANCEL);							\
																			\
			if (id == IDCANCEL)												\
				exit(1);													\
			AfxDebugBreak();												\
		}

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// This class provides time, error, etc., control
	// General games do not need to directly operate this object, so you can ignore the usage of this class.
	/////////////////////////////////////////////////////////////////////////////

	class CSpecialEffect {
	public:
		static void  SetCurrentTime();					// Store the current time in ctime
		static DWORD GetEllipseTime();					// Read the current time - ctime
		static int   GetCurrentTimeCount();				// Read the number of times ctime is stored
		static void  Delay(DWORD ms);					// Delay x ms
		static void  DelayFromSetCurrentTime(DWORD ms);	// Delay x ms since ctime
	private:
		static DWORD ctime;
		static int	 ctimeCount;
	};

	/////////////////////////////////////////////////////////////////////////////
	// This class will create DirectDraw objects to be used by other classes
	// General games do not need to directly operate this object, so you can ignore the usage of this class.
	/////////////////////////////////////////////////////////////////////////////

	class CDDraw {
		friend class CMovingBitmap;
	public:
		~CDDraw();
		static void  BltBackColor(DWORD);		// Color all Back plain with the specified color
		static void  BltBackToPrimary();		// Paste Back plain to Primary plain
		static CDC*  GetBackCDC();				// Get the DC (device context) of Back Plain
		static void  GetClientRect(CRect &r);	// Get the set resolution
		static void  Init(int, int);			// Initialize direct draw
		static void  ReleaseBackCDC();			// Release the DC (device context) of Back Plain
		static bool  SetFullScreen(bool);		// Set to full screen mode/windowed mode
		static bool  IsFullScreen();			// Answer whether it is full screen mode/windowed mode
	private:
		CDDraw();								// private constructor
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y);
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor);
		static void  BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y);
		static void	 CheckDDFail(char *s);
		static bool  CreateSurface();
		static bool  CreateSurfaceFullScreen();
		static bool  CreateSurfaceWindowed();
		static void  LoadBitmap(int i, int IDB_BITMAP);
		static void  LoadBitmap(int i, char *filename);
		static void  LoadBitmapFromExistHBITMAP(int i, HBITMAP bitmap);
		static DWORD MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color);
		static int   RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey);
		static int   RegisterBitmap(char *filename, COLORREF ColorKey);
		static int   RegisterBitmapWithHBITMAP(HBITMAP hbitmap);
		static void  ReleaseSurface();
		static void  RestoreSurface();
		static void  SetColorKey(unsigned SurfaceID, COLORREF color);
		static HDC					hdc;
		static CDC					cdc;
		static CView				*pCView;
		static LPDIRECTDRAW2		lpDD;
		static LPDIRECTDRAWCLIPPER	lpClipperPrimary;
		static LPDIRECTDRAWCLIPPER	lpClipperBack;
		static LPDIRECTDRAWSURFACE	lpDDSPrimary;
		static LPDIRECTDRAWSURFACE	lpDDSBack;
		static vector<LPDIRECTDRAWSURFACE>	lpDDS;
		static HRESULT				ddrval;
		static vector<int>			BitmapID;
		static vector<string>		BitmapName;
		static vector<CRect>		BitmapRect;
		static vector<COLORREF>		BitmapColorKey;
		static bool					fullscreen;
		static CDDraw				ddraw;
		static int					size_x, size_y;
	};

	/////////////////////////////////////////////////////////////////////////////
	// Declare an undefined class
	/////////////////////////////////////////////////////////////////////////////

	class CGame;
	class CGameStateInit;
	class CGameStateRun;
	class CGameStateOver;

	/////////////////////////////////////////////////////////////////////////////
	// This class is the Base class for the various states of the game (an abstract class)
	// You must understand the usage of each Public Interface, but you don¡¦t need to understand Implementation.
	/////////////////////////////////////////////////////////////////////////////

	class CGameState {
	public:
		CGameState(CGame *g);
		void OnDraw();			// Template Method
		void OnCycle();			// Template Method
		//
		// virtual functions, implementation provided by inheritors
		//
		virtual ~CGameState() {}								// virtual destructor
		virtual void OnBeginState() {}							// Set the initial value required each time you enter this state
		virtual void OnInit() {}								// Initial value of state and graphic settings
		virtual void OnKeyDown(UINT, UINT, UINT) {}				// Handle keyboard Down action
		virtual void OnKeyUp(UINT, UINT, UINT) {}				// Handle keyboard Up action
		virtual void OnLButtonDown(UINT nFlags, CPoint point) {}// Handling mouse movements
		virtual void OnLButtonUp(UINT nFlags, CPoint point) {}	// Handling mouse movements
		virtual void OnMouseMove(UINT nFlags, CPoint point) {}  // Handling mouse movements 
		virtual void OnRButtonDown(UINT nFlags, CPoint point) {}// Handling mouse movements
		virtual void OnRButtonUp(UINT nFlags, CPoint point) {}	// Handling mouse movements
	protected:
		void GotoGameState(int state);							// Jump to the specified state
		void ShowInitProgress(int percent, string message);						// Show initialization progress
		//
		// virtual functions, implementation provided by inheritors
		//
		virtual void OnMove() {}								// Move the game element in this state
		virtual void OnShow() = 0;								// Shows this state's game screen
		CGame *game;
		CMovingBitmap loadingBitmap;
	};

	/////////////////////////////////////////////////////////////////////////////
	// This class is the core of the game and controls the progress of the game.
	// General games do not need to directly operate this object, so you can ignore the usage of this class.
	/////////////////////////////////////////////////////////////////////////////

	class CGame {
	public:
		CGame();										// Constructor
		~CGame();										// Destructor
		bool IsRunning();								// Read whether the game is in progress
		void OnDraw();									// Corresponds to OnDraw() of CGameView
		void OnFilePause();								// Game paused
		void OnInit();									// Initialization of game graphics and sound effects
		void OnInitStates();							// Initial values and graphics settings for each game state
		bool OnIdle();									// The main loop of the game
		void OnKeyDown(UINT, UINT, UINT);				// Handle keyboard Down action
		void OnKeyUp(UINT, UINT, UINT);					// Handle keyboard Up action
		void OnKillFocus();								// The game is paused by force
		void OnLButtonDown(UINT nFlags, CPoint point);	// Handling mouse movements
		void OnLButtonUp(UINT nFlags, CPoint point);	// Handling mouse movements
		void OnMouseMove(UINT nFlags, CPoint point);    // Handling mouse movements
		void OnRButtonDown(UINT nFlags, CPoint point);	// Handling mouse movements
		void OnRButtonUp(UINT nFlags, CPoint point);	// Handling mouse movements
		void OnResume();								// Handles actions to restore from "Standby"
		void OnSetFocus();								// Handles Focus
		void OnSuspend();								// Handle "Standby" actions
		void SetGameState(int);
		static CGame *Instance();
	private:
		bool			running;			// Whether the game is in progress (not Paused)
		bool            suspended;			// Whether the game is suspended
		const int		NUM_GAME_STATES;	// Number of game states (3 states)
		CGameState		*gameState;			// pointer points to the current game state
		CGameState		*gameStateTable[3];	// The pointer of the game state object
		static CGame	instance;			// The only instance of the game
	};
}
