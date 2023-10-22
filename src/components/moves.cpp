#include "moves.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cstddef>
#include <cstring>

#include <random>

// definitely not stolen from stack overflow
int randInRange(int lower, int upper) { // [lower, upper]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(lower, upper);

    return distr(gen);
}

Moves::Moves(int width, int height, float thikness, unsigned char size)
    : size(size), movesCount(0), width(width), height(height), thikness(thikness) {
    movesIndeed = new Move*[size*size] {nullptr};

    rawVertexBuffer = new float[size * size * 20] {0};
    rawIndexBuffer = new unsigned int[size * size * 6] {0};

    vertexArray = new VertexArray;
    vertexBuffer = new VertexBuffer(nullptr, size * size * 20 * sizeof(float), GL_DYNAMIC_DRAW);
    indexBuffer = new IndexBuffer(nullptr, size * size * 6, GL_DYNAMIC_DRAW);

    VertexBufferLayout layout;

    //positon
    layout.Push<float>(2);
    //uv coordinates
    layout.Push<float>(2);
    //texture id
    layout.Push<float>(1);

    vertexArray->AddBuffer(*vertexBuffer, layout);

    shader = new Shader("../resources/shaders/moves.shader");

    playerTexture = new Texture("../resources/textures/player.png");
    enemyTexture = new Texture("../resources/textures/enemy.png");
    neutralTexture = new Texture("../resources/textures/neutral.png");
}

Moves::~Moves() {
    delete vertexBuffer;
    delete vertexArray;
    delete indexBuffer;
    delete shader;

    delete playerTexture;
    delete enemyTexture;

    delete[] rawVertexBuffer;
    delete[] rawIndexBuffer;

    delete[] movesIndeed;
}

void Moves::Render() {
    glBindTextureUnit(0, playerTexture->GetRendererId());
    glBindTextureUnit(1, enemyTexture->GetRendererId());
    glBindTextureUnit(2, neutralTexture->GetRendererId());

    glm::mat4 projection = glm::ortho(0.f, (float)size, 0.f, (float)size, -1.f, 1.f);

    vertexArray->Bind();
    indexBuffer->Bind();

    shader->Bind();
    shader->SetUniformMat4f("u_MVP", projection);

    int samplers[] = {0, 1, 2};

    shader->SetUniform1iv("u_Textures", 3, samplers);

    vertexBuffer->Bind();
    vertexBuffer->UpdateData(rawVertexBuffer, movesCount * 20 * sizeof(float));

    indexBuffer->UpdateData(rawIndexBuffer, movesCount * 6 * sizeof(unsigned int));

    GL_CALL(glDrawElements(GL_TRIANGLES, movesCount * 6, GL_UNSIGNED_INT, nullptr));
}

void Moves::Update() {
    for(int i = 1; i < size-1; i++) {
        for(int j = 1; j < size-1; j++) {
            Move *f = movesIndeed[i * size + j];

            Move *topLeft = movesIndeed[(i-1) * size + j - 1];
            Move *topRight = movesIndeed[(i-1) * size + j + 1];
            Move *bottomLeft = movesIndeed[(i+1) * size + j - 1];
            Move *bottomRight = movesIndeed[(i+1) * size + j + 1];

            if(!f) {
                continue;
            }

            PlayerType pt = f->playerType;

            PlayerType opt;

            if(pt == PLAYER) {
                opt = ENEMY;
            } else {
                opt = PLAYER;
            }

            if(topLeft && topRight && bottomLeft && bottomRight) {
                if(pt != NEUTRAL && topLeft->playerType == bottomRight->playerType && topRight->playerType == bottomLeft->playerType && topLeft->playerType != topRight->playerType) {
                    movesIndeed[i * size + j]->playerType = NEUTRAL;
                    UpdateRawVertexBuffer();
                    continue;
                }
            }

            if(pt == NEUTRAL) {
                continue;
            }

            if(topLeft && bottomRight && topLeft->playerType == opt && bottomRight->playerType == opt) {
                f->playerType = opt;
                UpdateRawVertexBuffer();
                continue;
            }

            if(topRight && bottomLeft && topRight->playerType == opt && bottomLeft->playerType == opt) {
                f->playerType = opt;
                UpdateRawVertexBuffer();
                continue;
            }
        }
    }
}

unsigned int Moves::EmptyMovesCount() {
    int result = 0;

    for(int i = 0; i < size*size; i++) {
        if(movesIndeed[i] == nullptr) {
            result++;
        }
    }

    return result;
}

void Moves::UpdateRawVertexBuffer() {
    int j = 0;

    for(int i = 0; i < size*size; i++) {
        if(!movesIndeed[i]) {
            continue;
        }

        glm::vec2 position = movesIndeed[i]->position;
        PlayerType playerType = movesIndeed[i]->playerType;

        float vertices[] = {
            position.x + thikness, position.y + thikness, 0.f, 0.f, (float)playerType,
            position.x + 1 - thikness, position.y + thikness, 1.f, 0.f, (float)playerType,
            position.x + 1 - thikness, position.y + 1 - thikness, 1.f, 1.f, (float)playerType,
            position.x + thikness, position.y + 1 - thikness, 0.f, 1.f, (float)playerType,
        };

        int offset = (j * sizeof(vertices)) / sizeof(float);

        std::memcpy(rawVertexBuffer + offset, vertices, sizeof(vertices));

        j++;
    }
}

GameResult Moves::GetGameResult() {
    int player = 0;
    int enemy = 0;

    for(int i = 0; i < size*size; i++) {
        if(movesIndeed[i]->playerType == PLAYER) {
            player++;
        } else if(movesIndeed[i]->playerType == ENEMY) {
            enemy++;
        }
    }

    if(player > enemy) {
        return WIN;
    } else if(enemy > player) {
        return LOSE;
    }

    return DRAW;
}

void Moves::MakeMove(glm::vec2 position, PlayerType playerType) {
    movesIndeed[(int)(position.y* size + position.x)] = new Move {
            .position = position,
            .playerType = playerType,
    };

    float vertices[] = {
        position.x + thikness, position.y + thikness, 0.f, 0.f, (float)playerType,
        position.x + 1 - thikness, position.y + thikness, 1.f, 0.f, (float)playerType,
        position.x + 1 - thikness, position.y + 1 - thikness, 1.f, 1.f, (float)playerType,
        position.x + thikness, position.y + 1 - thikness, 0.f, 1.f, (float)playerType,
    };

    unsigned int indicies[] = {
        (unsigned int)movesCount*4+0,
        (unsigned int)movesCount*4+1,
        (unsigned int)movesCount*4+2,
        (unsigned int)movesCount*4+2,
        (unsigned int)movesCount*4+3,
        (unsigned int)movesCount*4+0,
    };

    int offset = (movesCount * sizeof(vertices)) / sizeof(float);
    int offset2 = (movesCount * sizeof(indicies)) / sizeof(unsigned int);

    std::memcpy(rawVertexBuffer + offset, vertices, sizeof(vertices));
    std::memcpy(rawIndexBuffer + offset2, indicies, sizeof(indicies));

    movesCount++;
}

bool Moves::IsEmpty(glm::vec2 position) {
    for(int i = 0; i < size*size; i++) {
        Move *move = movesIndeed[i];

        if(move != nullptr && position.x == move->position.x && position.y == move->position.y) {
            return false;
        }
    }

    return true;
}

void Moves::Make500IQMove() {
    glm::vec2 newPos = glm::vec2(
            randInRange(0, size-1),
            randInRange(0, size-1)
    );

    Move *move = movesIndeed[(int)(newPos.y * size + newPos.x)];

    if(move != nullptr) {
        Make500IQMove();
    } else {
        MakeMove(newPos, PlayerType::ENEMY);
    }
}

void Moves::Reset() {
    for(int i = 0; i < size*size; i++) {
        if(movesIndeed[i] != nullptr) {
            delete movesIndeed[i];
            movesIndeed[i] = nullptr;
        }
    }

    for(int i = 0; i < size * size * 20; i++) {
        rawVertexBuffer[i] = 0;
    }

    for(int i = 0; i < size * size * 6; i++) {
        rawIndexBuffer[i] = 0;
    }

    movesCount = 0;
}
