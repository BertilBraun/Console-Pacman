/*
License
~~~~~~~
One Lone Coder Console Game Engine  Copyright(C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.

GNU GPLv3
	https://github.com/OneLoneCoder/videos/blob/master/LICENSE
*/

#pragma once

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. Thanks! For now, I'll try enabling it for you - Javidx9
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>

enum COLOUR
{
	FG_BLACK		= 0x0000,
	FG_DARK_BLUE    = 0x0001,	
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY			= 0x0007, // Thanks MS :-/
	FG_DARK_GREY    = 0x0008,
	FG_BLUE			= 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED			= 0x000C,
	FG_MAGENTA		= 0x000D,
	FG_YELLOW		= 0x000E,
	FG_WHITE		= 0x000F,
	BG_BLACK		= 0x0000,
	BG_DARK_BLUE	= 0x0010,
	BG_DARK_GREEN	= 0x0020,
	BG_DARK_CYAN	= 0x0030,
	BG_DARK_RED		= 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW	= 0x0060,
	BG_GREY			= 0x0070,
	BG_DARK_GREY	= 0x0080,
	BG_BLUE			= 0x0090,
	BG_GREEN		= 0x00A0,
	BG_CYAN			= 0x00B0,
	BG_RED			= 0x00C0,
	BG_MAGENTA		= 0x00D0,
	BG_YELLOW		= 0x00E0,
	BG_WHITE		= 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

class Sprite {
public:
	Sprite() {
		Create(8, 8);
	}

	Sprite(int w, int h) {
		Create(w, h);
	}

	Sprite(std::wstring sFile) {
		if (!Load(sFile))
			Create(8, 8);
	}

	Sprite(Sprite* s) {	
		Create(s->Width, s->Height);

		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Height; x++) {
				SetGlyph(x, y, s->GetGlyph(x, y));
				SetColor(x, y, s->GetColor(x, y));
			}
	}

	int Width = 0;
	int Height = 0;
	int scale = 1;

private:
	wchar_t *Glyphs = nullptr;
	short *Colors = nullptr;

public:
	void Scale(float scale) {

		if (scale == 0 || scale == 1)
			return;

		Sprite s = Sprite(this);

		Create(Width * scale, Height * scale);

		for (int y = 0; y < Height; y++)
			for (int x = 0; x < Width; x++) {
				SetGlyph(x, y, s.GetGlyph(floor(x / scale), floor(y / scale)));
				SetColor(x, y, s.GetColor(floor(x / scale), floor(y / scale)));
			}
	}

	void Create(int w, int h) {
		Width = w;
		Height = h;

		if (Glyphs != nullptr)
			delete[] Glyphs;
		Glyphs = new wchar_t[w * h];
		
		if (Colors != nullptr)
			delete[] Colors;
		Colors = new short[w * h];
		
		for (int i = 0; i < w * h; i++) {
			Glyphs[i] = L' ';
			Colors[i] = FG_BLACK;
		}
	}

	void SetGlyph(int x, int y, wchar_t c) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return;
		else
			Glyphs[y * Width + x] = c;
	}

	void SetColor(int x, int y, short c) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return;
		else
			Colors[y * Width + x] = c;
	}

	wchar_t GetGlyph(int x, int y) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return L' ';
		else
			return Glyphs[y * Width + x];
	}

	short GetColor(int x, int y) {
		if (x < 0 || x >= Width || y < 0 || y >= Height)
			return FG_BLACK;
		else
			return Colors[y * Width + x];
	}

	wchar_t SampleGlyph(float x, float y) {
		int sx = (int)(x * (float)Width);
		int sy = (int)(y * (float)Height-1.0f);
		if (sx < 0 || sx >= Width || sy < 0 || sy >= Height)
			return L' ';
		else
			return Glyphs[sy * Width + sx];
	}

	short SampleColor(float x, float y) {
		int sx = (int)(x * (float)Width);
		int sy = (int)(y * (float)Height-1.0f);
		if (sx < 0 || sx >= Width || sy < 0 || sy >= Height)
			return FG_BLACK;
		else
			return Colors[sy * Width + sx];
	}

	bool Save(std::wstring sFile) {
		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"wb");
		if (f == nullptr)
			return false;

		fwrite(&Width, sizeof(int), 1, f);
		fwrite(&Height, sizeof(int), 1, f);
		fwrite(Colors, sizeof(short), Width * Height, f);
		fwrite(Glyphs, sizeof(wchar_t), Width * Height, f);

		fclose(f);

		return true;
	}

	bool Load(std::wstring sFile) {
		if (Glyphs != nullptr)
			delete[] Glyphs;
		if (Colors != nullptr)
			delete[] Colors;
		Width = 0;
		Height = 0;

		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"rb");
		if (f == nullptr)
			return false;

		fread(&Width, sizeof(int), 1, f);
		fread(&Height, sizeof(int), 1, f);

		Create(Width, Height);

		fread(Colors, sizeof(short), Width * Height, f);
		fread(Glyphs, sizeof(wchar_t), Width * Height, f);

		fclose(f);
		return true;
	}
};

class ConsoleGameEngine {
public:
	ConsoleGameEngine()
	{
		nScreenWidth = 80;
		nScreenHeight = 30;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

		memset(keyNewState, 0, 256 * sizeof(short));
		memset(keyOldState, 0, 256 * sizeof(short));
		memset(keys, 0, 256 * sizeof(sKeyState));
		mousePosX = 0;
		mousePosY = 0;

		sAppName = L"Default";
	}

	int ConstructConsole(int width, int height, int fontw, int fonth)
	{
		if (hConsole == INVALID_HANDLE_VALUE)
			return Error(L"Bad Handle");

		nScreenWidth = width;
		nScreenHeight = height;

		// Update 13/09/2017 - It seems that the console behaves differently on some systems
		// and I'm unsure why this is. It could be to do with windows default settings, or
		// screen resolutions, or system languages. Unfortunately, MSDN does not offer much
		// by way of useful information, and so the resulting sequence is the reult of experiment
		// that seems to work in multiple cases.
		//
		// The problem seems to be that the SetConsoleXXX functions are somewhat circular and 
		// fail depending on the state of the current console properties, i.e. you can't set
		// the buffer size until you set the screen size, but you can't change the screen size
		// until the buffer size is correct. This coupled with a precise ordering of calls
		// makes this procedure seem a little mystical :-P. Thanks to wowLinh for helping - Jx9

		// Change console visual size to a minimum so ScreenBuffer can shrink
		// below the actual visual size
		rectWindow = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

		// Set the size of the screen buffer
		COORD coord = { (short)nScreenWidth, (short)nScreenHeight };
		if (!SetConsoleScreenBufferSize(hConsole, coord))
			Error(L"SetConsoleScreenBufferSize");

		// Assign screen buffer to the console
		if (!SetConsoleActiveScreenBuffer(hConsole))
			return Error(L"SetConsoleActiveScreenBuffer");
		
		// Set the font size now that the screen buffer has been assigned to the console
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontw;
		cfi.dwFontSize.Y = fonth;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		/*DWORD version = GetVersion();
		DWORD major = (DWORD)(LOBYTE(LOWORD(version)));
		DWORD minor = (DWORD)(HIBYTE(LOWORD(version)));

		if ((major > 6) || ((major == 6) && (minor >= 2) && (minor < 4)))		
			wcscpy_s(cfi.FaceName, L"Raster"); // Windows 8 :(
		else*/
			wcscpy_s(cfi.FaceName, L"Lucida Console"); // Everything else :P

		//wcscpy_s(cfi.FaceName, L"Liberation Mono");
		//wcscpy_s(cfi.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(hConsole, false, &cfi))
			return Error(L"SetCurrentConsoleFontEx");

		// Get screen buffer info and check the maximum allowed window size. Return
		// error if exceeded, so user knows their dimensions/fontsize are too large
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			return Error(L"GetConsoleScreenBufferInfo");
		if (nScreenHeight > csbi.dwMaximumWindowSize.Y)
			return Error(L"Screen Height / Font Height Too Big");
		if (nScreenWidth > csbi.dwMaximumWindowSize.X)
			return Error(L"Screen Width / Font Width Too Big");

		// Set Physical Console Window Size
		rectWindow = { 0, 0, (short)nScreenWidth - 1, (short)nScreenHeight - 1 };
		if (!SetConsoleWindowInfo(hConsole, TRUE, &rectWindow))
			return Error(L"SetConsoleWindowInfo");

		// Set flags to allow mouse input		
		if (!SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			return Error(L"SetConsoleMode");

		// Allocate memory for screen buffer
		for (int i = 0; i < 2; i++) {
			bufScreen[i] = new CHAR_INFO[nScreenWidth*nScreenHeight];
			memset(bufScreen[i], 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);
		}

		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
		return 1;
	}

	virtual void Draw(int x, int y, wchar_t c = 0x2588, short col = 0x000F)
	{
		if (x >= 0 && x < nScreenWidth && y >= 0 && y < nScreenHeight)
		{
			bufScreen[SwitchState][y * nScreenWidth + x].Char.UnicodeChar = c;
			bufScreen[SwitchState][y * nScreenWidth + x].Attributes = col;
		}
	}

	void Fill(int x1, int y1, int x2, int y2, wchar_t c = 0x2588, short col = 0x000F)
	{
		Clip(x1, y1);
		Clip(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, c, col);
	}

	void DrawString(int x, int y, std::wstring c, short col = 0x000F)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			bufScreen[SwitchState][y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
			bufScreen[SwitchState][y * nScreenWidth + x + i].Attributes = col;
		}
	}

	void DrawStringAlpha(int x, int y, std::wstring c, short col = 0x000F)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			if (c[i] != L' ')
			{
				bufScreen[SwitchState][y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
				bufScreen[SwitchState][y * nScreenWidth + x + i].Attributes = col;
			}
		}
	}

	void Clip(int &x, int &y)
	{
		if (x < 0) x = 0;
		if (x >= nScreenWidth) x = nScreenWidth;
		if (y < 0) y = 0;
		if (y >= nScreenHeight) y = nScreenHeight;
	}

	void DrawLine(int x1, int y1, int x2, int y2, wchar_t c = 0x2588, short col = 0x000F) {
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1;
		dy = y2 - y1;
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;
		if (dy1 <= dx1) {
			if (dx >= 0) {
				x = x1;
				y = y1;
				xe = x2;
			}
			else {
				x = x2;
				y = y2;
				xe = x1;
			}
			Draw(x, y, c, col);
			for (i = 0; x<xe; i++) {
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else {
					if ((dx<0 && dy<0) || (dx>0 && dy>0))
						y = y + 1;
					else
						y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c, col);
			}
		}
		else {
			if (dy >= 0) {
				x = x1;
				y = y1;
				ye = y2;
			}
			else {
				x = x2;
				y = y2;
				ye = y1;
			}
			Draw(x, y, c, col);
			for (i = 0; y<ye; i++) {
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else {
					if ((dx<0 && dy<0) || (dx>0 && dy>0))
						x = x + 1;
					else
						x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, c, col);
			}
		}
	}

	void DrawCircle(int xc, int yc, int r, wchar_t c = 0x2588, short col = 0x000F)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, c, col);//upper left left
			Draw(xc - y, yc - x, c, col);//upper upper left
			Draw(xc + y, yc - x, c, col);//upper upper right
			Draw(xc + x, yc - y, c, col);//upper right right
			Draw(xc - x, yc + y, c, col);//lower left left
			Draw(xc - y, yc + x, c, col);//lower lower left
			Draw(xc + y, yc + x, c, col);//lower lower right
			Draw(xc + x, yc + y, c, col);//lower right right
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

	void FillCircle(int xc, int yc, int r, wchar_t c = 0x2588, short col = 0x000F) {
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, c, col);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	};

	void DrawSprite(int x, int y, Sprite *sprite) {
		if (sprite == nullptr)
			return;

		for (int i = 0; i < sprite->Width; i++)
			for (int j = 0; j < sprite->Height; j++)
				if (sprite->GetGlyph(i, j) != L' ')
					Draw(x + i, y + j, sprite->GetGlyph(i, j), sprite->GetColor(i, j));
	}

	void DrawPartialSprite(int x, int y, Sprite *sprite, int ox, int oy, int w, int h)
	{
		if (sprite == nullptr)
			return;

		for (int i = 0; i < w; i++)
			for (int j = 0; j < h; j++)
				if (sprite->GetGlyph(i+ox, j+oy) != L' ')
					Draw(x + i, y + j, sprite->GetGlyph(i+ox, j+oy), sprite->GetColor(i+ox, j+oy));
	}

	void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw Closed Polygon
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
				(int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
		}
	}

	~ConsoleGameEngine()
	{
		SetConsoleActiveScreenBuffer(hOriginalConsole);
	}

public:
	void Start() {

		bAtomActive = true;

		std::thread t = std::thread(&ConsoleGameEngine::GameThread, this);

		t.join();
	}
	
	int ScreenWidth() {
		return nScreenWidth;
	}

	int ScreenHeight() {
		return nScreenHeight;
	}

private:
	void SwapBuffers() {
		while (bAtomActive) {
			WriteConsoleOutput(hConsole, bufScreen[SwitchState], { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rectWindow);
			bSwitch = false;
			while (!bSwitch);
		}
	}

	void GameThread() {

		// Create user resources as part of this thread
		if (!OnUserCreate())
			bAtomActive = false;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();
		std::thread t = std::thread(&ConsoleGameEngine::SwapBuffers, this);

		while (bAtomActive) {

			// Run as fast as possible
			while (bAtomActive) {

				SwitchState = !SwitchState;

				// Handle Timing
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;
				float fElapsedTime = elapsedTime.count();

				if (fElapsedTime > 0.15)
					fElapsedTime = 0.15;

				// Handle Keyboard Input
				for (int i = 0; i < 256; i++) {

					keyNewState[i] = GetAsyncKeyState(i);

					keys[i].Pressed = false;
					keys[i].Released = false;

					if (keyNewState[i] != keyOldState[i]) {

						if (keyNewState[i] & 0x8000) {
							keys[i].Pressed = !keys[i].Held;
							keys[i].Held = true;
						}
						else {
							keys[i].Released = true;
							keys[i].Held = false;
						}
					}
					keyOldState[i] = keyNewState[i];
				}

				// Handle Mouse Input - Check for window events
				INPUT_RECORD inBuf[32];
				DWORD events = 0;
				GetNumberOfConsoleInputEvents(hConsoleIn, &events);
				if (events > 0)
					ReadConsoleInput(hConsoleIn, inBuf, events, &events);

				// Handle events - we only care about mouse clicks and movement
				// for now
				for (DWORD i = 0; i < events; i++) {
					switch (inBuf[i].EventType)	{
					case FOCUS_EVENT:
						bConsoleInFocus = inBuf[i].Event.FocusEvent.bSetFocus;
					break;
					case MOUSE_EVENT:
						switch (inBuf[i].Event.MouseEvent.dwEventFlags) {
						case MOUSE_MOVED:
							mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
							mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
						break;
						case 0:
							for (int m = 0; m < 5; m++)
								mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
						break;
						}
					break;
					}
				}

				for (int m = 0; m < 5; m++) {

					mouse[m].Pressed = false;
					mouse[m].Released = false;

					if (mouseNewState[m] != mouseOldState[m]) {

						if (mouseNewState[m]) {
							mouse[m].Pressed = true;
							mouse[m].Held = true;
						}
						else {
							mouse[m].Released = true;
							mouse[m].Held = false;
						}
					}
					mouseOldState[m] = mouseNewState[m];
				}

				// Handle Frame Update
				if (!OnUserUpdate(fElapsedTime))
					bAtomActive = false;

				Fps.push_back(fElapsedTime);
				if (Fps.size() > 50)
					Fps.pop_front();
				float FPS = 0;
				for (auto &f : Fps)
					FPS += f;
				FPS /= Fps.size();

				// Update Title & Present Screen Buffer
				wchar_t s[256];
				swprintf_s(s, 256, L"%s - FPS: %3.2f", sAppName.c_str(), 1.0f / FPS);
				SetConsoleTitle(s);
				bSwitch = true;
			}

			if (OnUserDestroy()) {
				// User has permitted destroy, so exit and clean up
				delete[] bufScreen[0];
				delete[] bufScreen[1];
				SetConsoleActiveScreenBuffer(hOriginalConsole);
				cvGameFinished.notify_one();
			}
			else {
				// User denied destroy for some reason, so continue running
				bAtomActive = true;
			}
		}
		t.join();
	}

public:
	// User MUST OVERRIDE THESE!!
	virtual bool OnUserCreate() { return false; }
	virtual bool OnUserUpdate(float fElapsedTime) { return false; }

	// Optional for clean up 
	virtual bool OnUserDestroy() {

		return true;
	}


private:
	int mousePosX;
	int mousePosY;

	struct sKeyState {
		bool Pressed;
		bool Released;
		bool Held;
	};

public:
	sKeyState GetKey(int nKeyID){ return keys[nKeyID]; }
	int GetMouseX() { return mousePosX; }
	int GetMouseY() { return mousePosY; }
	sKeyState GetMouse(int nMouseButtonID) { return mouse[nMouseButtonID]; }
	bool IsFocused() { return bConsoleInFocus; }


protected:
	int Error(const wchar_t *msg) {
		wchar_t buf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		SetConsoleActiveScreenBuffer(hOriginalConsole);
		wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
		return 0;
	}

	static BOOL CloseHandler(DWORD evt) {
		// Note this gets called in a seperate OS thread, so it must
		// only exit when the game has finished cleaning up, or else
		// the process will be killed before OnUserDestroy() has finished
		if (evt == CTRL_CLOSE_EVENT) {
			bAtomActive = false;

			// Wait for thread to be exited
			std::unique_lock<std::mutex> ul(muxGame);
			cvGameFinished.wait(ul);
		}
		return true;
	}

protected:
	std::wstring sAppName;

private:
	std::list<float> Fps;
	sKeyState keys[256], mouse[5];
	int nScreenWidth;
	int nScreenHeight;
	CHAR_INFO *bufScreen[2];
	HANDLE hOriginalConsole;
	CONSOLE_SCREEN_BUFFER_INFO OriginalConsoleInfo;
	HANDLE hConsole;
	HANDLE hConsoleIn;
	SMALL_RECT rectWindow;
	short keyOldState[256] = { 0 };
	short keyNewState[256] = { 0 };
	bool mouseOldState[5] = { 0 };
	bool mouseNewState[5] = { 0 };
	bool bConsoleInFocus = true;
	bool SwitchState = false;
	static std::atomic<bool> bAtomActive;
	static std::atomic<bool> bSwitch;
	static std::condition_variable cvGameFinished;
	static std::mutex muxGame;
};
