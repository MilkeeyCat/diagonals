#pragma once

#include <unordered_map>
#include <vector>
#include <optional>
#include "component.h"
#include "GLFW/glfw3.h"
#include "components/moves.h"
#include "components/player.h"
#include "text_renderer.h"

//#define GLT_IMPLEMENTATION
//#include "glText/gltext.h"

class Game {
public:
    Game(GLFWwindow *window, unsigned char size, const int width, const int height, const float thikness);
    ~Game();

    void Render();
    void Update();
    void OnMouseMove(double x, double y);
    void OnKeyPress(int key, int scancode, int action, int mods);
    void Reset();
    static void CursorPositionCallback(GLFWwindow *window, double x, double y);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
    GLFWwindow *window;
    std::vector<Component *> components;
    bool mousePressed;
    std::unordered_map<std::string, Text> texts;
    Moves *moves;
    Player *player;

public:
    enum GameState {
        GAME_OVER,
        GAME_NOT_STARTED,
        PLAYER_MOVE,
        ENEMY_MOVE,
        PAUSE
    };
    GameState state;
    std::optional <GameState> prevState;
    const float THIKNESS;
    const unsigned char size;
    TextRender textRender;

    int width;
    int height;
};
