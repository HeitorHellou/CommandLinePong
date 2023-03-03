#include <iostream>
#include <thread>
#include <list>
#include <Random>
#include <Windows.h>

int screenWidth{ 120 };
int screenHeight{ 40 };

struct Paddle
{
	int x;
	int y;
};

int main()
{
	// Setup console window
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	for (int i{ 0 }; i < screenWidth * screenHeight; i++)
		screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Score variables
	int leftScore{ 0 };
	int rightScore{ 0 };

	// Ball variables
	int ballX{ screenWidth / 2 };
	int ballY{ screenHeight / 2 };
	int directionX{ -1 };
	int directionY{ -1 };

	// Paddles
	std::list<Paddle> paddleA = { {2, 18}, {2, 19}, {2, 20}, {2, 21}, {2, 22} };
	std::list<Paddle> paddleB = { {117, 18}, {117, 19}, {117, 20}, {117, 21}, {117, 22} };

	while (1)
	{
		// Update paddle A position
		if (GetAsyncKeyState((unsigned char)('\x057')) & 0x8000)
		{
			for (auto& p : paddleA)
			{
				p.y -= 1;
				if (p.y <= 0)
					p.y = 0;
			}
		}
		else if (GetAsyncKeyState((unsigned char)('\x053')) & 0x8000)
		{
			for (auto& p : paddleA)
			{
				p.y += 1;
				if (p.y >= screenHeight - 1)
					p.y = screenHeight - 1;
			}
		}

		// Boundarie check paddle A
		if (paddleA.front().y <= 0)
			for (auto& p : paddleA)
				p.y++;
		if (paddleA.back().y >= screenHeight - 1)
			for (auto& p : paddleA)
				p.y--;

		// Update paddle B position
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			for (auto& p : paddleB)
			{
				p.y -= 1;
				if (p.y <= 0)
					p.y = 0;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			for (auto& p : paddleB)
			{
				p.y += 1;
				if (p.y >= screenHeight - 1)
					p.y = screenHeight - 1;
			}
		}

		// Boundarie check paddle B
		if (paddleB.front().y <= 0)
			for (auto& p : paddleB)
				p.y++;
		if (paddleB.back().y >= screenHeight - 1)
			for (auto& p : paddleB)
				p.y--;

		// Check for ball collision with paddle A
		for (auto p : paddleA)
		{
			if (ballX == p.x + 1 && ballY == p.y)
				directionX *= -1;
		}

		// Check for ball collision with paddle B
		for (auto p : paddleB)
		{
			if (ballX == p.x - 1 && ballY == p.y)
				directionX *= -1;
		}

		// Clear screen 
		for (int i{ 0 }; i < screenWidth * screenHeight; i++)
		{
			screen[i] = L' ';
		}

		// Draw dashed line
		for (int y{ 0 }; y < screenHeight; y += 2)
		{
			screen[y * screenWidth + screenWidth / 2] = L'|';
		}

		// Draw ball
		screen[ballY * screenWidth + ballX] = L'•';

		// Draw score
		wsprintf(&screen[screenWidth + 55], L"%d", leftScore);
		wsprintf(&screen[screenWidth + 65], L"%d", rightScore);

		// Draw paddle
		for (auto p : paddleA)
			screen[p.y * screenWidth + p.x] = L'H';
		for (auto p : paddleB)
			screen[p.y * screenWidth + p.x] = L'H';

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);

		// Move ball
		ballX += directionX;
		ballY += directionY;

		// Bounce ball off walls
		if (ballX <= 0)
		{
			rightScore++;
			ballX = screenWidth / 2;
			ballY = screenHeight / 2;
			directionX = rand() % 2 == 0 ? -1 : 1;
			directionY = rand() % 2 == 0 ? -1 : 1;
			paddleA = { {2, 18}, {2, 19}, {2, 20}, {2, 21}, {2, 22} };
			paddleB = { {117, 18}, {117, 19}, {117, 20}, {117, 21}, {117, 22} };
		}
		else if (ballX >= screenWidth)
		{
			leftScore++;
			ballX = screenWidth / 2;
			ballY = screenHeight / 2;
			directionX = rand() % 2 == 0 ? -1 : 1;
			directionY = rand() % 2 == 0 ? -1 : 1;
			paddleA = { {2, 18}, {2, 19}, {2, 20}, {2, 21}, {2, 22} };
			paddleB = { {117, 18}, {117, 19}, {117, 20}, {117, 21}, {117, 22} };
		}
		if (ballY <= 0 || ballY >= screenHeight) {
			directionY *= -1;
		}

		// Wait 1 second to launch the ball
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}