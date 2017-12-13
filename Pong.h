#pragma once

struct PongBall
{
	POINT pos;
	POINT dir;
	int radius;
};

class Pong
{
public:
	Pong(int screen_width, int screen_height);
	~Pong();

	virtual void Update();
	virtual void Draw(HDC hdc);
	virtual void KeyboardInput(int key);
	void reset(HWND hWnd);

private:

	PongBall ball;

	POINT viewport;

	POINT left_paddle;
	POINT right_paddle;
	POINT* current_paddle;

	int half_paddle_width;
	int half_paddle_height;
	int padding;
	int speed;

	int left_player_score;
	int right_player_score;
};
