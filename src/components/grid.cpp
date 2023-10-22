#include "grid.h"
#include "GLFW/glfw3.h"
#include "moves.h"
#include <cstdlib>

Grid::Grid(Game *game, Moves *moves, unsigned char size, const float thikness, int width, int height)
    : size(size), thikness(thikness), moves(moves), game(game), width(width), height(height) {
    const int VERTEX_BUFFER_SIZE_PER_ELEMENT = 8;
    const int INDEX_BUFFER_SIZE_PER_ELEMENT = 6;
    const int QUAD_SOMETHING = 4;

    int sizePlusOne = size + 1;

    float *grid = new float[2 * sizePlusOne * VERTEX_BUFFER_SIZE_PER_ELEMENT];

    projection = glm::ortho(0.f, (float)(size), 0.f, (float)(size), -1.f, 1.f);

    //vertical
    for(int i = 0; i < sizePlusOne; i++) {
        glm::vec2 bottomLeft = glm::vec2((float)i - thikness, -(float)sizePlusOne);
        glm::vec2 bottomRight = glm::vec2((float)i + thikness, -(float)sizePlusOne);

        glm::vec2 topLeft = glm::vec2((float)i - thikness, (float)sizePlusOne);
        glm::vec2 topRight = glm::vec2((float)i + thikness, (float)sizePlusOne);

        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+0] = bottomLeft.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+1] = bottomLeft.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+2] = bottomRight.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+3] = bottomRight.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+4] = topRight.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+5] = topRight.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+6] = topLeft.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+7] = topLeft.y;
    }

    //horizontal
    for(int i = sizePlusOne; i < sizePlusOne*2; i++) {
        glm::vec2 bottomLeft = glm::vec2(-(float)sizePlusOne, (float)i - sizePlusOne + thikness);
        glm::vec2 bottomRight = glm::vec2((float)sizePlusOne, (float)i - sizePlusOne + thikness);

        glm::vec2 topLeft = glm::vec2(-(float)sizePlusOne, i - sizePlusOne - thikness);
        glm::vec2 topRight = glm::vec2((float)sizePlusOne, i - sizePlusOne - thikness);

        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+0] = bottomLeft.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+1] = bottomLeft.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+2] = bottomRight.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+3] = bottomRight.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+4] = topRight.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+5] = topRight.y;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+6] = topLeft.x;
        grid[i*VERTEX_BUFFER_SIZE_PER_ELEMENT+7] = topLeft.y;
    }

    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(grid, sizeof(float) * sizePlusOne * VERTEX_BUFFER_SIZE_PER_ELEMENT * 2);

    VertexBufferLayout layout;
    layout.Push<float>(2);

    vertexArray->AddBuffer(*vertexBuffer, layout);

    unsigned int *indicies = new unsigned int[2 * sizePlusOne * INDEX_BUFFER_SIZE_PER_ELEMENT];

    for(int i = 0; i < sizePlusOne; i++) {
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+0] = i * QUAD_SOMETHING + 0;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+1] = i * QUAD_SOMETHING + 1;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+2] = i * QUAD_SOMETHING + 2;

        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+3] = i * QUAD_SOMETHING + 2;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+4] = i * QUAD_SOMETHING + 3;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+5] = i * QUAD_SOMETHING + 0;
    }

    for(int i = sizePlusOne; i < sizePlusOne*2; i++) {
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+0] = i * QUAD_SOMETHING + 0;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+1] = i * QUAD_SOMETHING + 1;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+2] = i * QUAD_SOMETHING + 2;

        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+3] = i * QUAD_SOMETHING + 2;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+4] = i * QUAD_SOMETHING + 3;
        indicies[i*INDEX_BUFFER_SIZE_PER_ELEMENT+5] = i * QUAD_SOMETHING + 0;
    }

    indexBuffer = new IndexBuffer(indicies, sizePlusOne * INDEX_BUFFER_SIZE_PER_ELEMENT * 2);

    shader = new Shader("../resources/shaders/grid.shader");
}

Grid::~Grid() {
    delete vertexBuffer;
    delete vertexArray;
    delete indexBuffer;
    delete shader;
}

void Grid::Render() {
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", projection);

    vertexArray->Bind();
    indexBuffer->Bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Grid::OnClick(double x, double y) {
    int intX = (int)(x) / (width / size);
    int intY = (int)(y) / (height / size);

    glm::vec2 move((float)intX, (float)(abs(size - 1 - intY)));

    if(game->state == Game::PLAYER_MOVE && moves->IsEmpty(move)) {
        moves->MakeMove(move, PlayerType::PLAYER);
        game->state = Game::ENEMY_MOVE;
    }
}
