//#define	 INITGUID
#include "stdafx.h"
#include "../Core/Game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>
using namespace std::experimental::filesystem::v1;


namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CGame: Game Class
	// This class is the facade of the game and is the bridge between MFC and each game state.
	// If you do not increase or decrease the game state, you can ignore the interface and implementation of this class.
	/////////////////////////////////////////////////////////////////////////////

	CGame CGame::instance;

	CGame::CGame()
		: NUM_GAME_STATES(3)
	{
		running = true;
		suspended = false;
		gameStateTable[GAME_STATE_INIT] = new CGameStateInit(this);
		gameStateTable[GAME_STATE_RUN] = new CGameStateRun(this);
		gameStateTable[GAME_STATE_OVER] = new CGameStateOver(this);
		gameState = nullptr;
	}

	CGame::~CGame()
	{
		for (int i = 0; i < NUM_GAME_STATES; i++)
			delete gameStateTable[i];
	}

	CGame *CGame::Instance()
	{
		return &instance;
	}

	bool CGame::IsRunning()
	{
		return running;
	}

	void CGame::OnDraw()
	{
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);	// Blacken Back Plain
		gameState->OnDraw();					// Show every element in the game
		if (!running) {
			//
			// If in pause state, shows Ctrl-Q...
			//
			// CMovingBitmap bmp;
			// bmp.LoadBitmap(IDB_CONTINUE);
			// bmp.SetTopLeft(0, 0);
			// bmp.ShowBitmap();
		}
		CDDraw::BltBackToPrimary();				// Paste Back Plain to the screen
	}

	void  CGame::OnFilePause()
	{
		if (ENABLE_GAME_PAUSE) {
			if (running)
				CAudio::Instance()->Pause();
			else
				CAudio::Instance()->Resume();
			running = !running;
		}
		else {
			CAudio::Instance()->Resume();
			running = true;
		}
	}

	bool CGame::OnIdle()  // When modifying functions, do not modify OnIdle(), but modify OnMove() and OnShow().
	{
		if (suspended) {
			running = false;
			suspended = false;
		}
		//
		// Control whether the game is paused
		//
		if (!running)
			return false;
		//
		// The following is the main loop of the game
		//
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);	// Paint Back Plain with the preset color
		gameState->OnCycle();
		CDDraw::BltBackToPrimary();				// Paste Back Plain to the screen
		//
		// The following program controls the speed of the game. Note:
		// 1. Use Debug mode to view the time spent on each loop, and let this time be t.
		// 2. The time since leaving OnIdle() last time is set to 33ms, cannot be deleted, and the time cannot be lower than t.
		//
		if (SHOW_GAME_CYCLE_TIME)
			TRACE("Ellipse time for the %d th cycle=%d \n", CSpecialEffect::GetCurrentTimeCount(), CSpecialEffect::GetEllipseTime());
		CSpecialEffect::DelayFromSetCurrentTime(GAME_CYCLE_TIME);
		CSpecialEffect::SetCurrentTime();	// Set the time to leave OnIdle()
		return true;
	}

	void CGame::OnInit()	// OnInit() is only executed once at the beginning of the program
	{
		//
		// Start random numbers
		//
		srand((unsigned)time(NULL));
		//
		// Open the DirectX map interface
		//
		CDDraw::Init(SIZE_X, SIZE_Y);							// Set game resolution
		//
		// Open the DirectX audio interface
		//
		if (!CAudio::Instance()->Open())						// Open the audio interface
			AfxMessageBox("Audio Interface Failed (muted)");	// No audio interface
		//
		// Switch to the first state
		//
		gameState = gameStateTable[GAME_STATE_INIT];
		gameState->OnBeginState();
		CSpecialEffect::SetCurrentTime();
		running = true;
	}

	void CGame::OnInitStates()
	{
		//
		// Calls OnInitialUpdate for each state
		//
		for (int i = 0; i < NUM_GAME_STATES; i++)
			gameStateTable[i]->OnInit();
	}

	void CGame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (running)
			if ((nFlags & 0x4000) == 0) // Remove auto repeat
				gameState->OnKeyDown(nChar, nRepCnt, nFlags);
#ifdef _UNITTEST					// invike unit test if _UNITTEST is defined
		void runTest();
		if (nChar == 'T')
			runTest();
#endif
	}

	void CGame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (running)
			gameState->OnKeyUp(nChar, nRepCnt, nFlags);
	}

	void CGame::OnKillFocus()
	{
		CAudio::Instance()->Pause();
		if (ENABLE_GAME_PAUSE)
			running = false;
		else if (CDDraw::IsFullScreen())
			running = false;
	}

	void CGame::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (running) {
			gameState->OnLButtonDown(nFlags, point);
		}
	}

	void CGame::OnRButtonDown(UINT nFlags, CPoint point)
	{
		if (running)
			gameState->OnRButtonDown(nFlags, point);
	}

	void CGame::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (running)
			gameState->OnLButtonUp(nFlags, point);
	}

	void CGame::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (running)
			gameState->OnMouseMove(nFlags, point);
	}

	void CGame::OnRButtonUp(UINT nFlags, CPoint point)
	{
		if (running)
			gameState->OnRButtonUp(nFlags, point);
	}

	void CGame::OnResume()
	{
		//
		// Note: the resume message is not synchronized with the other messages
		//
	}

	void CGame::OnSetFocus()
	{
		if (!ENABLE_GAME_PAUSE) {
			CAudio::Instance()->Resume();
			running = true;
		}
	}

	void CGame::OnSuspend()
	{
		//
		// Note: the suspend message is not synchronized with the other messages
		//
		suspended = true;
		CAudio::Instance()->SetPowerResume();
	}

	void CGame::SetGameState(int state)
	{
		ASSERT(state >= 0 && state < NUM_GAME_STATES);
		gameState = gameStateTable[state];
		gameState->OnBeginState();
		OnDraw();
		CSpecialEffect::SetCurrentTime();
		running = true;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CSpecialEffect: Specail Effect functions
	// General games do not require direct manipulation of this object, so you can skip it altogether.
	/////////////////////////////////////////////////////////////////////////////

	DWORD CSpecialEffect::ctime = 0;
	int   CSpecialEffect::ctimeCount = 0;

	void CSpecialEffect::Delay(DWORD ms)
	{
		DWORD ctime = timeGetTime();
		int waitms;
		waitms = ms - (timeGetTime() - ctime);
		if (waitms > 0)
			Sleep(waitms);
	}

	void CSpecialEffect::DelayFromSetCurrentTime(DWORD ms)
	{
		int waitms;
		waitms = ms - (timeGetTime() - ctime);
		if (waitms > 0)
			Sleep(waitms);
	}

	void CSpecialEffect::SetCurrentTime()
	{
		ctime = timeGetTime();
		ctimeCount++;
	}

	DWORD CSpecialEffect::GetEllipseTime()
	{
		return timeGetTime() - ctime;
	}

	int CSpecialEffect::GetCurrentTimeCount()
	{
		return ctimeCount;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CDDraw: Direct Draw Object
	// This class will create DirectDraw objects to be used by other classes
	// All programs in this class are low-level drawing interfaces, so you can skip them all.
	/////////////////////////////////////////////////////////////////////////////

	HDC							CDDraw::hdc;
	CDC							CDDraw::cdc;
	CView						*CDDraw::pCView;
	LPDIRECTDRAW2				CDDraw::lpDD;
	LPDIRECTDRAWCLIPPER			CDDraw::lpClipperPrimary;
	LPDIRECTDRAWCLIPPER			CDDraw::lpClipperBack;
	LPDIRECTDRAWSURFACE			CDDraw::lpDDSPrimary;
	LPDIRECTDRAWSURFACE			CDDraw::lpDDSBack;
	HRESULT						CDDraw::ddrval;
	bool						CDDraw::fullscreen;
	int							CDDraw::size_x, CDDraw::size_y;
	CDDraw						CDDraw::ddraw;
	vector<int>					CDDraw::BitmapID;
	vector<string>				CDDraw::BitmapName;
	vector<CRect>				CDDraw::BitmapRect;
	vector<COLORREF>			CDDraw::BitmapColorKey;
	vector<LPDIRECTDRAWSURFACE>	CDDraw::lpDDS;

	CDDraw::CDDraw()
	{
		pCView = nullptr;
		lpClipperPrimary = lpClipperBack = nullptr;
		lpDDSPrimary = lpDDSBack = nullptr;
		fullscreen = OPEN_AS_FULLSCREEN;
		lpDD = nullptr;
	}

	CDDraw::~CDDraw()
	{
		ReleaseSurface();
		if (lpDD)
			lpDD->Release();
		pCView = nullptr; lpDD = nullptr;
		lpClipperPrimary = lpClipperBack = nullptr;
		lpDDSPrimary = lpDDSBack = nullptr;
		TRACE("~CDDraw()\n");
	}

	void CDDraw::BltBackColor(DWORD color)
	{
		if (lpDDSBack) {
			if (lpDDSBack->IsLost())
				RestoreSurface();
			DDBLTFX ddbltfx;
			ddbltfx.dwSize = sizeof(ddbltfx);
			ddbltfx.dwFillColor = MatchColorKey(lpDDSBack, color);
			ddrval = lpDDSBack->Blt(nullptr, nullptr, nullptr, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
			CheckDDFail("BltBackColor: blitting failed");
		}
	}

	void CDDraw::BltBackToPrimary()
	{
		if (!pCView || !lpDDSPrimary || !lpDDSBack)
			return;
		if (fullscreen) {
			CRect ClientRect;
			GetClientRect(ClientRect);
			if (lpDDSBack->IsLost())
				RestoreSurface();
			if (lpDDSPrimary->IsLost())
				RestoreSurface();
			ddrval = lpDDSPrimary->Blt(ClientRect, lpDDSBack, ClientRect, DDBLTFAST_WAIT, nullptr);
			CheckDDFail("Blt Back to primary failed");
		}
		else {
			if (lpDDSBack->IsLost())
				RestoreSurface();
			if (lpDDSPrimary->IsLost())
				RestoreSurface();
			CRect ClientRect;
			GetClientRect(ClientRect);
			CRect WindowRect;
			pCView->GetWindowRect(&WindowRect);
			WindowRect.right = WindowRect.left + size_x;
			WindowRect.bottom = WindowRect.top + size_y;
			ddrval = lpDDSPrimary->Blt(WindowRect, lpDDSBack, ClientRect, DDBLTFAST_WAIT, nullptr);
			CheckDDFail("Blt Back to primary failed");
		}
	}

	void CDDraw::BltBitmapToBack(unsigned SurfaceID, int x, int y)
	{
		x = CDDraw::IsFullScreen() ? x + (RESOLUTION_X - SIZE_X) / 2 : x;
		y = CDDraw::IsFullScreen() ? y + (RESOLUTION_Y - SIZE_Y) / 2 : y;
		GAME_ASSERT(lpDDSBack && (SurfaceID < lpDDS.size()) && lpDDS[SurfaceID], "Internal Error: Incorrect SurfaceID in BltBitmapToBack");
		CRect TargetRect;
		TargetRect.left = x;
		TargetRect.top = y;
		TargetRect.right = x + BitmapRect[SurfaceID].right - BitmapRect[SurfaceID].left;
		TargetRect.bottom = y + BitmapRect[SurfaceID].bottom - BitmapRect[SurfaceID].top;
		int blt_flag;
		if (BitmapColorKey[SurfaceID] != CLR_INVALID)
			blt_flag = DDBLT_WAIT | DDBLT_KEYSRC;
		else
			blt_flag = DDBLT_WAIT;
		if (lpDDSBack->IsLost())
			RestoreSurface();
		if (lpDDS[SurfaceID]->IsLost())
			RestoreSurface();

		ddrval = lpDDSBack->Blt(TargetRect, lpDDS[SurfaceID], nullptr, blt_flag, nullptr);
		CheckDDFail("Blt Bitmap to Back Failed");
	}

	void CDDraw::BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor)
	{
		GAME_ASSERT(lpDDSBack && (SurfaceID < lpDDS.size()) && lpDDS[SurfaceID], "Internal Error: Incorrect SurfaceID in BltBitmapToBack");
		CRect TargetRect;
		TargetRect.left = x;
		TargetRect.top = y;
		TargetRect.right = x + (int)((BitmapRect[SurfaceID].right - BitmapRect[SurfaceID].left)*factor);
		TargetRect.bottom = y + (int)((BitmapRect[SurfaceID].bottom - BitmapRect[SurfaceID].top)*factor);


		if (factor == 0) {
			return;
		}

		int blt_flag;
		if (BitmapColorKey[SurfaceID] != CLR_INVALID)
			blt_flag = DDBLT_WAIT | DDBLT_KEYSRC;
		else
			blt_flag = DDBLT_WAIT;
		if (lpDDSBack->IsLost())
			RestoreSurface();
		if (lpDDS[SurfaceID]->IsLost())
			RestoreSurface();
		ddrval = lpDDSBack->Blt(TargetRect, lpDDS[SurfaceID], nullptr, blt_flag, nullptr);
		CheckDDFail("Blt Bitmap to Back Failed");
	}

	void CDDraw::BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y)
	{
		GAME_ASSERT((SourceID < lpDDS.size()) && (TargetID < lpDDS.size()) && (SourceID != TargetID), "Internal Error: Incorrect SourceID in BltBitmapToBitmap");
		int blt_flag;
		if (BitmapColorKey[SourceID] != CLR_INVALID)
			blt_flag = DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY;
		else
			blt_flag = DDBLTFAST_WAIT;
		if (lpDDS[SourceID]->IsLost())
			RestoreSurface();
		if (lpDDS[TargetID]->IsLost())
			RestoreSurface();
		ddrval = lpDDS[TargetID]->BltFast(x, y, lpDDS[SourceID], nullptr, blt_flag);
		CheckDDFail("Blt Bitmap to Back Failed");
	}

	bool CDDraw::CreateSurface()
	{
		//
		// Since all surfaces are created and loaded, this operation can be very slow.
		//
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		ReleaseSurface();
		if (fullscreen) {
			if (!CreateSurfaceFullScreen())
				return false;
		}
		else {
			if (!CreateSurfaceWindowed())
				return false;
		}
		for (unsigned i = 0; i < lpDDS.size(); i++) {
			if (BitmapID[i] != -1) // from resource
				LoadBitmap(i, BitmapID[i]);
			else
				LoadBitmap(i, (char *)BitmapName[i].c_str()); // from file
			SetColorKey(i, BitmapColorKey[i]);
		}

		return true;
	}

	bool CDDraw::CreateSurfaceFullScreen()
	{

		ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		CheckDDFail("Can not SetCooperativeLevel Exclusive");

		ddrval = lpDD->SetDisplayMode(RESOLUTION_X, RESOLUTION_Y, 32, 0, 0);

		if (ddrval != DD_OK) {
			ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_NORMAL);
			CheckDDFail("Can not SetCooperativeLevel Normal");
			return false;
		}
		CheckDDFail("SetDisplayMode FAILED");

		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, nullptr);
		CheckDDFail("Create Primary Surface failed");

		// Create clippers for the primary and back surfaces
		// ddrval = lpDD->CreateClipper(0, &lpClipperPrimary, NULL);
		// CheckDDFail("Create Primay Surface Clipper FAILED"); 
		ddrval = lpDD->CreateClipper(0, &lpClipperBack, nullptr);
		CheckDDFail("Create Back Surface Clipper FAILED");

		typedef struct {
			RGNDATAHEADER hdr;
			RECT rgndata[4];
		} CLIPLIST, *LPCLIPLIST;
		CLIPLIST    ClipList;
		RECT rc;
		SetRect(&rc, 0, 0, size_x, size_y);
		ClipList.hdr.dwSize = sizeof(RGNDATAHEADER);
		ClipList.hdr.iType = RDH_RECTANGLES;
		ClipList.hdr.nCount = 1;
		ClipList.hdr.nRgnSize = 0;
		memcpy(&ClipList.hdr.rcBound, &rc, sizeof(RECT));
		memcpy(&ClipList.rgndata, &rc, sizeof(RECT));
		ddrval = lpClipperBack->SetClipList((LPRGNDATA)&ClipList, 0);
		CheckDDFail("SetHWnd FAILED");

		// Create Back (Secondary) Surface
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwHeight = size_y; ddsd.dwWidth = size_x;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSBack, nullptr);
		CheckDDFail("Can not create back plain");

		// In fullscreen mode, the clipper for primary surafce is fixed to screen size
		ddrval = lpDDSPrimary->SetClipper(lpClipperBack);
		CheckDDFail("SetClipper FAILED");
		ddrval = lpDDSBack->SetClipper(lpClipperBack);
		CheckDDFail("SetClipper FAILED");
		lpClipperBack->Release();

		BltBackColor(RGB(0, 0, 0));
		BltBackToPrimary();

		return true;
	}

	bool CDDraw::CreateSurfaceWindowed()
	{

		ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_NORMAL);
		CheckDDFail("Can not SetCooperativeLevel ");

		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, nullptr);
		CheckDDFail("Create Primary Surface failed");

		// Create clippers for the primary and back surfaces
		ddrval = lpDD->CreateClipper(0, &lpClipperPrimary, nullptr);
		CheckDDFail("Create Primay Surface Clipper FAILED");
		ddrval = lpDD->CreateClipper(0, &lpClipperBack, nullptr);
		CheckDDFail("Create Back Surface Clipper FAILED");

		ddrval = lpClipperPrimary->SetHWnd(0, AfxGetMainWnd()->m_hWnd);
		CheckDDFail("Primary Surface SetHWnd FAILED");

		typedef struct {
			RGNDATAHEADER hdr;
			RECT rgndata[4];
		} CLIPLIST, *LPCLIPLIST;
		CLIPLIST    ClipList;
		RECT rc;
		SetRect(&rc, 0, 0, size_x, size_y);
		ClipList.hdr.dwSize = sizeof(RGNDATAHEADER);
		ClipList.hdr.iType = RDH_RECTANGLES;
		ClipList.hdr.nCount = 1;
		ClipList.hdr.nRgnSize = 0;
		memcpy(&ClipList.hdr.rcBound, &rc, sizeof(RECT));
		memcpy(&ClipList.rgndata, &rc, sizeof(RECT));
		ddrval = lpClipperBack->SetClipList((LPRGNDATA)&ClipList, 0);
		CheckDDFail("SetHWnd FAILED");

		// Create Back (Secondary) Surface
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwHeight = size_y; ddsd.dwWidth = size_x;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDSBack, nullptr);
		CheckDDFail("Can not create back plain");

		ddrval = lpDDSPrimary->SetClipper(lpClipperPrimary);
		CheckDDFail("SetClipper FAILED");
		lpClipperPrimary->Release();
		ddrval = lpDDSBack->SetClipper(lpClipperBack);
		CheckDDFail("SetClipper FAILED");
		lpClipperBack->Release();

		BltBackColor(RGB(0, 0, 0));

		SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOP, 0, 0, size_x, size_y, SWP_NOZORDER);

		return true;
	}

	void CDDraw::GetClientRect(CRect &r)
	{
		r = CRect(0, 0, size_x, size_y);
	}

	void CDDraw::Init(int sx, int sy)
	{
		// set target screen size
		size_x = RESOLUTION_X, size_y = RESOLUTION_Y;
		// init lpDD
		LPDIRECTDRAW lpDD0;
		ddrval = DirectDrawCreate(nullptr, &lpDD0, nullptr);
		CheckDDFail("DDraw create failed");
		ddrval = lpDD0->QueryInterface(IID_IDirectDraw2, (void **)&lpDD);
		CheckDDFail("DDraw surface 2 create failed");
		// init pCView
		POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
		CDocTemplate *doc_t = AfxGetApp()->GetNextDocTemplate(pos);
		pos = doc_t->GetFirstDocPosition();
		CDocument *doc = doc_t->GetNextDoc(pos);
		pos = doc->GetFirstViewPosition();
		pCView = doc->GetNextView(pos);
		// init surfaces
		SetFullScreen(fullscreen);
	}

	bool CDDraw::IsFullScreen()
	{
		return fullscreen;
	}

	void CDDraw::LoadBitmap(int i, int IDB_BITMAP)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP);
		CDC mDC;
		mDC.CreateCompatibleDC(nullptr);
		CBitmap* pOldBitmap = mDC.SelectObject(&bitmap);
		BITMAP bitmapSize;
		bitmap.GetBitmap(&bitmapSize);
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		BitmapRect[i].bottom = ddsd.dwHeight = bitmapSize.bmHeight;
		BitmapRect[i].right = ddsd.dwWidth = bitmapSize.bmWidth;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDS[i], nullptr);
		CheckDDFail("Create Bitmap Surface Failed");
		HDC hdc;
		ddrval = lpDDS[i]->GetDC(&hdc);
		CheckDDFail("Get surface HDC failed");
		CDC cdc;
		cdc.Attach(hdc);
		cdc.BitBlt(0, 0, bitmapSize.bmWidth, bitmapSize.bmHeight, &mDC, 0, 0, SRCCOPY);
		cdc.Detach();
		lpDDS[i]->ReleaseDC(hdc);
		// avoid memory leak
		// According to spec, mDC should delete itself automatically.  However,
		// it appears that we have to do it explictly.
		mDC.SelectObject(&pOldBitmap);
		mDC.DeleteDC();
		bitmap.DeleteObject();
	}

	void CDDraw::LoadBitmap(int i, char *filename)
	{

		HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GAME_ASSERT(hbitmap != NULL, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // will be deleted automatically
		CDC mDC;
		mDC.CreateCompatibleDC(nullptr);
		CBitmap* pOldBitmap = mDC.SelectObject(bmp);
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		BitmapRect[i].bottom = ddsd.dwHeight = bitmapSize.bmHeight;
		BitmapRect[i].right = ddsd.dwWidth = bitmapSize.bmWidth;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDS[i], nullptr);
		CheckDDFail("Create Bitmap Surface Failed");
		HDC hdc;
		ddrval = lpDDS[i]->GetDC(&hdc);
		CheckDDFail("Get surface HDC failed");
		CDC cdc;
		cdc.Attach(hdc);
		cdc.BitBlt(0, 0, bitmapSize.bmWidth, bitmapSize.bmHeight, &mDC, 0, 0, SRCCOPY);
		cdc.Detach();
		lpDDS[i]->ReleaseDC(hdc);
		// avoid memory leak
		// According to spec, mDC should delete itself automatically.  However,
		// it appears that we have to do it explictly.
		mDC.SelectObject(&pOldBitmap);
		mDC.DeleteDC();
		bmp->DeleteObject();
	}

	void CDDraw::LoadBitmapFromExistHBITMAP(int i, HBITMAP hbitmap)
	{

		GAME_ASSERT(hbitmap != NULL, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // will be deleted automatically
		CDC mDC;
		mDC.CreateCompatibleDC(nullptr);
		CBitmap* pOldBitmap = mDC.SelectObject(bmp);
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		BitmapRect[i].bottom = ddsd.dwHeight = bitmapSize.bmHeight;
		BitmapRect[i].right = ddsd.dwWidth = bitmapSize.bmWidth;
		ddrval = lpDD->CreateSurface(&ddsd, &lpDDS[i], nullptr);
		CheckDDFail("Create Bitmap Surface Failed");
		HDC hdc;
		ddrval = lpDDS[i]->GetDC(&hdc);
		CheckDDFail("Get surface HDC failed");
		CDC cdc;
		cdc.Attach(hdc);
		cdc.BitBlt(0, 0, bitmapSize.bmWidth, bitmapSize.bmHeight, &mDC, 0, 0, SRCCOPY);
		cdc.Detach();
		lpDDS[i]->ReleaseDC(hdc);
		// avoid memory leak
		// According to spec, mDC should delete itself automatically.  However,
		// it appears that we have to do it explictly.
		mDC.SelectObject(&pOldBitmap);
		mDC.DeleteDC();
		bmp->DeleteObject();
	}

	DWORD CDDraw::MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color)
	{
		DDSURFACEDESC ddsd;
		HDC hdc;
		HRESULT hres;
		COLORREF rgbT = CLR_INVALID;
		DWORD dw = CLR_INVALID, mask = (DWORD)~0;
		if (lpDDSurface && color != CLR_INVALID) {
			if (lpDDSurface->GetDC(&hdc) == DD_OK) {
				rgbT = GetPixel(hdc, 0, 0);             // save (0,0) pixel value
				SetPixel(hdc, 0, 0, color);             // set our value
				lpDDSurface->ReleaseDC(hdc);
			}
			ddsd.dwSize = sizeof(ddsd);
			while ((hres = lpDDSurface->Lock(nullptr, &ddsd, 0, nullptr)) == DDERR_WASSTILLDRAWING);
			if (hres == DD_OK) {
				dw = *(DWORD *)ddsd.lpSurface;          // get (0,0) data
				if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
					mask = (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;
				dw &= mask;								// mask it to bpp
				lpDDSurface->Unlock(nullptr);
			}
			if (lpDDSurface->GetDC(&hdc) == DD_OK) {
				SetPixel(hdc, 0, 0, rgbT);				// restore (0,0) pixel value
				lpDDSurface->ReleaseDC(hdc);
			}
		}
		return dw;
	}

	CDC* CDDraw::GetBackCDC()
	{
		if (lpDDSBack->IsLost())
			RestoreSurface();
		ddrval = lpDDSBack->GetDC(&hdc);
		CheckDDFail("Get back surface HDC failed");
		cdc.Attach(hdc);
		return &cdc;
	}

	int CDDraw::RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey)
	{
		unsigned i;
		for (i = 0; i < lpDDS.size(); i++)
			if (BitmapID[i] == IDB_BITMAP)
				return i;
		//
		// Enlarge the size of vectors
		//
		BitmapID.push_back(IDB_BITMAP);
		BitmapName.push_back("");
		BitmapColorKey.push_back(ColorKey);
		BitmapRect.push_back(CRect(0, 0, 0, 0));
		lpDDS.push_back(nullptr);
		LoadBitmap(i, IDB_BITMAP);
		SetColorKey(i, ColorKey);
		return i;
	}

	int CDDraw::RegisterBitmap(char *filename, COLORREF ColorKey)
	{
		unsigned i;
		for (i = 0; i < lpDDS.size(); i++)
			if (BitmapName[i].compare(filename) == 0)
				return i;
		//
		// Enlarge the size of vectors
		//
		BitmapID.push_back(-1);
		BitmapName.push_back(filename);
		BitmapColorKey.push_back(ColorKey);
		BitmapRect.push_back(CRect(0, 0, 0, 0));
		lpDDS.push_back(nullptr);
		LoadBitmap(i, filename);
		SetColorKey(i, ColorKey);
		return i;
	}

	int CDDraw::RegisterBitmapWithHBITMAP(HBITMAP bitmap)
	{
		unsigned i = BitmapName.size();
		//
		// Enlarge the size of vectors
		//
		BitmapID.push_back(-1);
		BitmapName.push_back("");
		BitmapColorKey.push_back(CLR_INVALID);
		BitmapRect.push_back(CRect(0, 0, 0, 0));
		lpDDS.push_back(nullptr);
		LoadBitmapFromExistHBITMAP(i, bitmap);
		SetColorKey(i, CLR_INVALID);
		return i;
	}

	void CDDraw::ReleaseBackCDC()
	{
		cdc.Detach();
		ddrval = lpDDSBack->ReleaseDC(hdc);
		CheckDDFail("Release back HDC failed");
	}

	void CDDraw::ReleaseSurface()
	{
		if (lpDD)
		{
			for (unsigned i = 0; i < lpDDS.size(); i++)
				if (lpDDS[i]) {
					lpDDS[i]->Release();
					lpDDS[i] = nullptr;
				}
			if (lpDDSBack)
			{
				lpDDSBack->Release();
				lpDDSBack = nullptr;
			}
			if (lpDDSPrimary)
			{
				lpDDSPrimary->Release();
				lpDDSPrimary = nullptr;
			}
		}
	}

	void CDDraw::RestoreSurface()
	{
		//
		// Since all surfaces are restored and reloaded, this operation can be very slow.
		//
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		if (lpDD != nullptr)
		{
			CreateSurface();
			while (lpDDSBack->IsLost() || lpDDSPrimary->IsLost()) {
				Sleep(100);
				CreateSurface();
			}
		}
	}

	void CDDraw::SetColorKey(unsigned SurfaceID, COLORREF color)
	{
		if (color != CLR_INVALID) {
			DDCOLORKEY ddck;
			ddck.dwColorSpaceLowValue = MatchColorKey(lpDDS[SurfaceID], color);
			ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
			ddrval = lpDDS[SurfaceID]->SetColorKey(DDCKEY_SRCBLT, &ddck);
			CheckDDFail("Can not Set Color Key");
		}
	}

	bool CDDraw::SetFullScreen(bool isFullScreen)
	{
		fullscreen = isFullScreen;
		return CreateSurface();
	}

	void CDDraw::CheckDDFail(char *s)
	{
		if (ddrval != DD_OK) {
			TRACE("Error Code: %d (%s)\n", ddrval, s);
			//
			// For some unknown reason, in Win98, ddrval can be changed
			//	 after AfxMessageBox() is called, resulting an unknown
			//	 error message.
			//
			AfxMessageBox(s);
			static int ErrorCode[] = {
				DDERR_ALREADYINITIALIZED          ,DDERR_BLTFASTCANTCLIP             ,DDERR_CANNOTATTACHSURFACE         ,DDERR_CANNOTDETACHSURFACE         ,
				DDERR_CANTCREATEDC                ,DDERR_CANTDUPLICATE               ,DDERR_CANTLOCKSURFACE             ,DDERR_CANTPAGELOCK                ,
				DDERR_CANTPAGEUNLOCK              ,DDERR_CLIPPERISUSINGHWND          ,DDERR_COLORKEYNOTSET              ,DDERR_CURRENTLYNOTAVAIL           ,
				DDERR_DCALREADYCREATED            ,DDERR_DIRECTDRAWALREADYCREATED    ,DDERR_EXCEPTION                   ,DDERR_EXCLUSIVEMODEALREADYSET     ,
				DDERR_GENERIC                     ,DDERR_HEIGHTALIGN                 ,DDERR_HWNDALREADYSET              ,DDERR_HWNDSUBCLASSED              ,
				DDERR_IMPLICITLYCREATED           ,DDERR_INCOMPATIBLEPRIMARY         ,DDERR_INVALIDCAPS                 ,DDERR_INVALIDCLIPLIST             ,
				DDERR_INVALIDDIRECTDRAWGUID       ,DDERR_INVALIDMODE                 ,DDERR_INVALIDOBJECT               ,DDERR_INVALIDPARAMS               ,
				DDERR_INVALIDPIXELFORMAT          ,DDERR_INVALIDPOSITION             ,DDERR_INVALIDRECT                 ,DDERR_INVALIDSURFACETYPE          ,
				DDERR_LOCKEDSURFACES              ,DDERR_NO3D                        ,DDERR_NOALPHAHW                   ,DDERR_NOBLTHW                     ,
				DDERR_NOCLIPLIST                  ,DDERR_NOCLIPPERATTACHED           ,DDERR_NOCOLORCONVHW               ,DDERR_NOCOLORKEY                  ,
				DDERR_NOCOLORKEYHW                ,DDERR_NOCOOPERATIVELEVELSET       ,DDERR_NODC                        ,DDERR_NODDROPSHW                  ,
				DDERR_NODIRECTDRAWHW              ,DDERR_NODIRECTDRAWSUPPORT         ,DDERR_NOEMULATION                 ,DDERR_NOEXCLUSIVEMODE             ,
				DDERR_NOFLIPHW                    ,DDERR_NOGDI                       ,DDERR_NOHWND                      ,DDERR_NOMIPMAPHW                  ,
				DDERR_NOMIRRORHW                  ,DDERR_NOOVERLAYDEST               ,DDERR_NOOVERLAYHW                 ,DDERR_NOPALETTEATTACHED           ,
				DDERR_NOPALETTEHW                 ,DDERR_NORASTEROPHW                ,DDERR_NOROTATIONHW                ,DDERR_NOSTRETCHHW                 ,
				DDERR_NOT4BITCOLOR                ,DDERR_NOT4BITCOLORINDEX           ,DDERR_NOT8BITCOLOR                ,DDERR_NOTAOVERLAYSURFACE          ,
				DDERR_NOTEXTUREHW                 ,DDERR_NOTFLIPPABLE                ,DDERR_NOTFOUND                    ,DDERR_NOTINITIALIZED              ,
				DDERR_NOTLOCKED                   ,DDERR_NOTPAGELOCKED               ,DDERR_NOTPALETTIZED               ,DDERR_NOVSYNCHW                   ,
				DDERR_NOZBUFFERHW                 ,DDERR_NOZOVERLAYHW                ,DDERR_OUTOFCAPS                   ,DDERR_OUTOFMEMORY                 ,
				DDERR_OUTOFVIDEOMEMORY            ,DDERR_OVERLAYCANTCLIP             ,DDERR_OVERLAYCOLORKEYONLYONEACTIVE,DDERR_OVERLAYNOTVISIBLE           ,
				DDERR_PALETTEBUSY                 ,DDERR_PRIMARYSURFACEALREADYEXISTS ,DDERR_REGIONTOOSMALL              ,DDERR_SURFACEALREADYATTACHED      ,
				DDERR_SURFACEALREADYDEPENDENT     ,DDERR_SURFACEBUSY                 ,DDERR_SURFACEISOBSCURED           ,DDERR_SURFACELOST                 ,
				DDERR_SURFACENOTATTACHED          ,DDERR_TOOBIGHEIGHT                ,DDERR_TOOBIGSIZE                  ,DDERR_TOOBIGWIDTH                 ,
				DDERR_UNSUPPORTED                 ,DDERR_UNSUPPORTEDFORMAT           ,DDERR_UNSUPPORTEDMASK             ,DDERR_UNSUPPORTEDMODE             ,
				DDERR_VERTICALBLANKINPROGRESS     ,DDERR_WASSTILLDRAWING             ,DDERR_WRONGMODE                   ,DDERR_XALIGN
			};
			static char *ErrorMsg[] = {
				"DDERR_ALREADYINITIALIZED          ","DDERR_BLTFASTCANTCLIP             ","DDERR_CANNOTATTACHSURFACE         ","DDERR_CANNOTDETACHSURFACE         ",
				"DDERR_CANTCREATEDC                ","DDERR_CANTDUPLICATE               ","DDERR_CANTLOCKSURFACE             ","DDERR_CANTPAGELOCK                ",
				"DDERR_CANTPAGEUNLOCK              ","DDERR_CLIPPERISUSINGHWND          ","DDERR_COLORKEYNOTSET              ","DDERR_CURRENTLYNOTAVAIL           ",
				"DDERR_DCALREADYCREATED            ","DDERR_DIRECTDRAWALREADYCREATED    ","DDERR_EXCEPTION                   ","DDERR_EXCLUSIVEMODEALREADYSET     ",
				"DDERR_GENERIC                     ","DDERR_HEIGHTALIGN                 ","DDERR_HWNDALREADYSET              ","DDERR_HWNDSUBCLASSED              ",
				"DDERR_IMPLICITLYCREATED           ","DDERR_INCOMPATIBLEPRIMARY         ","DDERR_INVALIDCAPS                 ","DDERR_INVALIDCLIPLIST             ",
				"DDERR_INVALIDDIRECTDRAWGUID       ","DDERR_INVALIDMODE                 ","DDERR_INVALIDOBJECT               ","DDERR_INVALIDPARAMS               ",
				"DDERR_INVALIDPIXELFORMAT          ","DDERR_INVALIDPOSITION             ","DDERR_INVALIDRECT                 ","DDERR_INVALIDSURFACETYPE          ",
				"DDERR_LOCKEDSURFACES              ","DDERR_NO3D                        ","DDERR_NOALPHAHW                   ","DDERR_NOBLTHW                     ",
				"DDERR_NOCLIPLIST                  ","DDERR_NOCLIPPERATTACHED           ","DDERR_NOCOLORCONVHW               ","DDERR_NOCOLORKEY                  ",
				"DDERR_NOCOLORKEYHW                ","DDERR_NOCOOPERATIVELEVELSET       ","DDERR_NODC                        ","DDERR_NODDROPSHW                  ",
				"DDERR_NODIRECTDRAWHW              ","DDERR_NODIRECTDRAWSUPPORT         ","DDERR_NOEMULATION                 ","DDERR_NOEXCLUSIVEMODE             ",
				"DDERR_NOFLIPHW                    ","DDERR_NOGDI                       ","DDERR_NOHWND                      ","DDERR_NOMIPMAPHW                  ",
				"DDERR_NOMIRRORHW                  ","DDERR_NOOVERLAYDEST               ","DDERR_NOOVERLAYHW                 ","DDERR_NOPALETTEATTACHED           ",
				"DDERR_NOPALETTEHW                 ","DDERR_NORASTEROPHW                ","DDERR_NOROTATIONHW                ","DDERR_NOSTRETCHHW                 ",
				"DDERR_NOT4BITCOLOR                ","DDERR_NOT4BITCOLORINDEX           ","DDERR_NOT8BITCOLOR                ","DDERR_NOTAOVERLAYSURFACE          ",
				"DDERR_NOTEXTUREHW                 ","DDERR_NOTFLIPPABLE                ","DDERR_NOTFOUND                    ","DDERR_NOTINITIALIZED              ",
				"DDERR_NOTLOCKED                   ","DDERR_NOTPAGELOCKED               ","DDERR_NOTPALETTIZED               ","DDERR_NOVSYNCHW                   ",
				"DDERR_NOZBUFFERHW                 ","DDERR_NOZOVERLAYHW                ","DDERR_OUTOFCAPS                   ","DDERR_OUTOFMEMORY                 ",
				"DDERR_OUTOFVIDEOMEMORY            ","DDERR_OVERLAYCANTCLIP             ","DDERR_OVERLAYCOLORKEYONLYONEACTIVE","DDERR_OVERLAYNOTVISIBLE           ",
				"DDERR_PALETTEBUSY                 ","DDERR_PRIMARYSURFACEALREADYEXISTS ","DDERR_REGIONTOOSMALL              ","DDERR_SURFACEALREADYATTACHED      ",
				"DDERR_SURFACEALREADYDEPENDENT     ","DDERR_SURFACEBUSY                 ","DDERR_SURFACEISOBSCURED           ","DDERR_SURFACELOST                 ",
				"DDERR_SURFACENOTATTACHED          ","DDERR_TOOBIGHEIGHT                ","DDERR_TOOBIGSIZE                  ","DDERR_TOOBIGWIDTH                 ",
				"DDERR_UNSUPPORTED                 ","DDERR_UNSUPPORTEDFORMAT           ","DDERR_UNSUPPORTEDMASK             ","DDERR_UNSUPPORTEDMODE             ",
				"DDERR_VERTICALBLANKINPROGRESS     ","DDERR_WASSTILLDRAWING             ","DDERR_WRONGMODE                   ","DDERR_XALIGN                      "
			};
			for (int i = 0; i < sizeof(ErrorCode) / sizeof(int); i++)
				if (ddrval == ErrorCode[i])
					GAME_ASSERT(0, ErrorMsg[i]);
			GAME_ASSERT(0, "Direct Draw Failed due to unknown error code !!!");
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// This class is the Base class for the various states of the game (an abstract class)
	/////////////////////////////////////////////////////////////////////////////

	CGameState::CGameState(CGame *g)
	{
		game = g; 	// Set the game pointer
	}

	void CGameState::GotoGameState(int state)
	{
		game->SetGameState(state);
	}

	void CGameState::ShowInitProgress(int percent, string message)
	{
		if (!SHOW_LOAD_PROGRESS)
			return;
		const int bar_width = SIZE_X * 2 / 3;
		const int bar_height = SIZE_Y / 20;
		const int x1 = (SIZE_X - bar_width) / 2;
		const int x2 = x1 + bar_width;
		const int y1 = (SIZE_Y - bar_height) / 2;
		const int y2 = y1 + bar_height;
		const int pen_width = bar_height / 8;
		const int progress_x1 = x1 + pen_width;
		const int progress_x2 = progress_x1 + percent * (bar_width - 2 * pen_width) / 100;
		const int progress_x2_end = x2 - pen_width;
		const int progress_y1 = y1 + pen_width;
		const int progress_y2 = y2 - pen_width;

		CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// Paint Back Plain with the preset color

		// CMovingBitmap loading;						// Paste the loading icon
		// loading.LoadBitmap({ "RES/loading.bmp" });
		// loading.SetTopLeft(0, 0);
		// loading.ShowBitmap();

		//
		// 以下為CDC的用法
		//
		CDC *pDC = CDDraw::GetBackCDC();			// Get CDC for Back Plain
		CPen *pp, p(PS_NULL, 0, RGB(0, 0, 0));		// clear pen
		pp = pDC->SelectObject(&p);

		CBrush *pb, b(RGB(155, 155, 155));				// Draw a green progress box
		pb = pDC->SelectObject(&b);
		pDC->Rectangle(x1, y1, x2, y2);

		CBrush b1(DEFAULT_BG_COLOR);				// Draw black progress center/core
		pDC->SelectObject(&b1);
		pDC->Rectangle(progress_x1, progress_y1, progress_x2_end, progress_y2);

		CBrush b2(RGB(255, 255, 255));					// Draw yellow progress of the progress
		pDC->SelectObject(&b2);
		pDC->Rectangle(progress_x1, progress_y1, progress_x2, progress_y2);

		pDC->SelectObject(pp);						// Release pen
		pDC->SelectObject(pb);						// Release brush


		CTextDraw::ChangeFontLog(pDC, 30, "微軟正黑體", RGB(255, 255, 255));

		CTextDraw::Print(pDC, x1, (int)(SIZE_Y * 0.40), message.c_str());

		CDDraw::ReleaseBackCDC();					// Let go of Back Plain's CDC.
		//
		// If CDC is used elsewhere, do not copy the following line, otherwise the screen will flicker
		//
		CDDraw::BltBackToPrimary();					// Paste Back Plain to the screen
	}

	void CGameState::OnDraw() // Template Method
	{
		OnShow();
	}

	void CGameState::OnCycle() // Template Method
	{
		OnMove();
		OnShow();
	}

}
