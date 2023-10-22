#pragma once

#include "component.h"
#include "moves.h"

class Player : public Component {
public:
    Player(Moves *moves, float thikness, int width, int height, unsigned char size);
    ~Player();

    void Render();
    void OnMouseMove(double x, double y);
    void SetIsHidden(bool newVal) { hidden = newVal; }
    void Reset();

private:
    unsigned char size;
    glm::vec2 position;
    bool hidden;
    Moves *moves;

    float thikness;
    int width;
    int height;
};
