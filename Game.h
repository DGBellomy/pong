#pragma once

class Game
{
public:

    Game(PWSTR title, int screen_width, int screen_height);
    ~Game();

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void Draw(HDC hdc) = 0;
    virtual void KeyboardInput(int key) = 0;

private:

    PWSTR m_Title;
    int m_Width;
    int m_Height;

};
