#pragma once

#include "Game.h"

struct PongBall
{
    POINT pos;
    POINT dir;
    int radius;
};

class Pong : public Game
{
public:
    Pong(PWSTR title, int screen_width, int screen_height);
    ~Pong();

    void Init() final;
    void Update() final;
    void LateUpdate() final;
    void Draw(HDC hdc) final;
    void KeyboardInput(int key) final;
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
