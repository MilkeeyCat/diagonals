#include "player.h"

#include <glm/gtc/matrix_transform.hpp>

Player::Player(Moves *moves, float thikness, int width, int height, unsigned char size)
    : hidden(true), moves(moves), size(size), thikness(thikness), width(width), height(height) {

    position = glm::vec2(0.f, 0.f);

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    vertexArray = new VertexArray;
    vertexBuffer = new VertexBuffer(nullptr, 8 * sizeof(float), GL_DYNAMIC_DRAW);

    VertexBufferLayout layout;

    layout.Push<float>(2);

    vertexArray->AddBuffer(*vertexBuffer, layout);

    indexBuffer = new IndexBuffer(indicies, sizeof(indicies));

    glm::mat4 projection = glm::ortho(0.f, (float)size, 0.f, (float)size, -1.f, 1.f);

    shader = new Shader("../resources/shaders/player.shader");

    shader->Bind();
    shader->SetUniformMat4f("u_MVP", projection);
}

Player::~Player() {
}

void Player::Render() {
    float coords[] = {
        position.x + thikness, position.y + thikness,
        position.x + 1 - thikness, position.y + thikness,
        position.x + 1 - thikness, position.y + 1 - thikness,
        position.x + thikness, position.y + 1 - thikness,
    };

    vertexArray->Bind();
    vertexBuffer->Bind();
    vertexBuffer->UpdateData(coords, 8 * sizeof(float));
    shader->Bind();

    if(!hidden && moves->IsEmpty(position)) {
        GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
    }
}

void Player::OnMouseMove(double x, double y) {
    int intX = (int)x / (width / size);
    int intY = (int)y / (height / size);

    glm::vec2 move((float)intX, (float)(abs(size - 1 - intY)));

    position.x = move.x;
    position.y = move.y;
}

void Player::Reset() {
    position = glm::vec2(0, 0);
    hidden = true;
}
