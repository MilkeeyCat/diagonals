#pragma once

#include "component.h"

enum PlayerType {
    PLAYER,
    ENEMY,
    NEUTRAL
};

enum GameResult {
    WIN,
    LOSE,
    DRAW
};

struct Move {
    glm::vec2 position;
    PlayerType playerType;
};

class Moves : public Component {
public:
    Moves(int widht, int height, float thikness, unsigned char size);
    ~Moves();

    void Render();
    void Update();
    void MakeMove(glm::vec2 position, PlayerType playerType);
    bool IsEmpty(glm::vec2 position);
    void Make500IQMove();
    void UpdateRawVertexBuffer();
    GameResult GetGameResult();
    unsigned int EmptyMovesCount();
    void Reset();

private:
    const unsigned char size;
    int movesCount;
    Move **movesIndeed;
    float *rawVertexBuffer;
    unsigned int *rawIndexBuffer;
    Texture *playerTexture;
    Texture *enemyTexture;
    Texture *neutralTexture;
    int width;
    int height;
    float thikness;
};
