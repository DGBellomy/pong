#include "pch.h"
#include "Pong.h"
#include "Win32App.h"

#define KEY_W 0x57
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44


Pong::Pong(int screen_width, int screen_height)
{
	left_player_score = 0;
	right_player_score = 0;
	viewport = { screen_width - 18, screen_height - 40 };
	ball.radius = 10;
	half_paddle_width = 5;
	half_paddle_height = 20;
	int padding = 5;
	left_paddle = { padding + half_paddle_width, viewport.y / 2 };
	right_paddle = { viewport.x - (padding + half_paddle_width), viewport.y / 2 };
	speed = 10;

	ball.dir = { -5, 7 };

	current_paddle = &left_paddle;

	reset(Win32App::GetHWND());
}


Pong::~Pong()
{
}

void Pong::Update()
{
	ball.pos.x += ball.dir.x;
	ball.pos.y += ball.dir.y;
	if ((ball.pos.y + ball.radius) >= viewport.y || (ball.pos.y - ball.radius) <= 0)
		ball.dir.y *= -1;
	if ((ball.pos.x - ball.radius) <= (left_paddle.x + half_paddle_width))
	{
		if (ball.pos.y < (left_paddle.y + half_paddle_height) && ball.pos.y >(left_paddle.y - half_paddle_height))
			ball.dir.x *= -1;
		else
		{
			right_player_score++;
			reset(Win32App::GetHWND());
		}
	}
	if ((ball.pos.x + ball.radius) >= (right_paddle.x - half_paddle_width))
	{
		if (ball.pos.y < (right_paddle.y + half_paddle_height) && ball.pos.y >(right_paddle.y - half_paddle_height))
			ball.dir.x *= -1;
		else
		{
			left_player_score++;
			reset(Win32App::GetHWND());
		}
	}
}

void Pong::Draw(HDC hdc)
{
	const int left_paddle_left = left_paddle.x - half_paddle_width;
	int left_paddle_top = left_paddle.y - half_paddle_height;
	const int left_paddle_right = left_paddle.x + half_paddle_width;
	int left_paddle_bottom = left_paddle.y + half_paddle_height;

	const int right_paddle_left = right_paddle.x - half_paddle_width;
	int right_paddle_top = right_paddle.y - half_paddle_height;
	const int right_paddle_right = right_paddle.x + half_paddle_width;
	int right_paddle_bottom = right_paddle.y + half_paddle_height;
	
	Ellipse(hdc, ball.pos.x - ball.radius, ball.pos.y - ball.radius, ball.pos.x + ball.radius, ball.pos.y + ball.radius);
	//Rectangle(hdc, left_paddle.x - half_paddle_width, left_paddle.y - half_paddle_height, left_paddle.x + half_paddle_width, left_paddle.y + half_paddle_height);
	Rectangle(hdc, left_paddle_left, left_paddle_top, left_paddle_right, left_paddle_bottom);
	//Rectangle(hdc, right_paddle.x - half_paddle_width, right_paddle.y - half_paddle_height, right_paddle.x + half_paddle_width, right_paddle.y + half_paddle_height);
	Rectangle(hdc, right_paddle_left, right_paddle_top, right_paddle_right, right_paddle_bottom);
}

void Pong::KeyboardInput(int key)
{
	switch (key)
	{
	case KEY_A:
		current_paddle = &left_paddle;
		break;
	case KEY_D:
		current_paddle = &right_paddle;
		break;

	case VK_UP:
		if (current_paddle->y > (half_paddle_height))
			current_paddle->y -= speed;
		break;
	case VK_DOWN:
		if (current_paddle->y < viewport.y - (half_paddle_height))
			current_paddle->y += speed;
		break;
	}
}

void Pong::reset(HWND hWnd)
{
	std::wstring title = L"P1: ";
	title += std::to_wstring(left_player_score);
	title += L"  P2: ";
	title += std::to_wstring(right_player_score);
	SetWindowText(hWnd, title.c_str());
	ball.pos = { viewport.x / 2, viewport.y / 2 };
}
