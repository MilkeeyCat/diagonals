#pragma once

#include "component.h"
#include "moves.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "game.h"

class Grid : public Component {
public:
    Grid(Game *game, Moves *moves, const unsigned char size, const float thikness, int width, int height);
    ~Grid();

    void Render();
    void OnClick(double x, double y);
private:
    Game *game;
    Moves *moves;
    float thikness;
    glm::mat4 projection;
    unsigned char size;
    int width;
    int height;
};
