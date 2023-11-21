#include "game.h"

#include "GLFW/glfw3.h"
#include "components/grid.h"
#include "components/moves.h"
#include "components/player.h"
#include "components/start_screen.h"
#include "imgui/backends/imgui_impl_glfw.h"

Game::Game(GLFWwindow *window, unsigned char size, const int width, const int height, const float thikness)
    : size(size), window(window), mousePressed(false),
      state(GAME_NOT_STARTED), THIKNESS(thikness),
      width(width), height(height), prevState(std::nullopt) {
    textRender = TextRender();

    texts["gameResult"] = {
        .pos = {300.f, 250.f},
        .color = {RGB_TO_NORMALIZED(242, 181, 212), 1},
        .text = textRender.Create(""),
        .scale = 10.f,
    };

    texts["hint"] = {
        .pos = {300.f, 575.f},
        .color = {RGB_TO_NORMALIZED(247, 214, 224), 1},
        .text = textRender.Create("press Space to restart the game"),
        .scale = 2.f,
    };

    int windowWidth, windowHeight;

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    moves = new Moves(windowWidth, windowHeight, THIKNESS, size);
    Grid *grid = new Grid(this, moves, size, THIKNESS, windowWidth, windowHeight);
    player = new Player(moves, THIKNESS, windowWidth, windowHeight, size);
    auto start_screen = new StartScreen(this);

    components.push_back(grid);
    components.push_back(moves);
    components.push_back(player);
    components.push_back(start_screen);
}

Game::~Game() {
    gltTerminate();

    for(Component *component : components) {
        if(component != nullptr) {
            delete component;
        }
    }
}

void Game::CursorPositionCallback(GLFWwindow *window, double x, double y) {
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    ImGui_ImplGlfw_CursorPosCallback(window, x, y);

    game->OnMouseMove(x, y);
}

void Game::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if(action == GLFW_PRESS) {
        if(game->state != game->PAUSE) {
            game->prevState = game->state;
        }

        if(key == GLFW_KEY_SPACE && game->state == GAME_NOT_STARTED) {
            game->state = PLAYER_MOVE;
        }

        if(key == GLFW_KEY_ESCAPE && game->state != PAUSE) {
            game->state = game->PAUSE;
        } else if(key == GLFW_KEY_ESCAPE && game->state == PAUSE) {
            game->state = game->prevState.value();
        }

        if(key == GLFW_KEY_SPACE && game->state == GAME_OVER) {
            //restart the game
            game->Reset();
        }

        if(key == GLFW_KEY_R) {
            game->Reset();
        }

        game->OnKeyPress(key, scancode, action, mods);
    }
}

void Game::Reset() {
    for(auto component : components) {
        component->Reset();
    }

    state = PLAYER_MOVE;
}

void Game::Render() {
    for(auto *component : components) {
        component->Render();
    }

    if(state == GAME_OVER) {
        for(auto [_, el] : texts) {
            textRender.Render(el, GLT_CENTER, GLT_CENTER);
        }
    }
}

void Game::Update() {
    for(auto component : components) {
        component->Update();
    }

    int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if(mouseState == GLFW_RELEASE && mousePressed) {
        mousePressed = false;
    }

    if(mouseState == GLFW_PRESS && !mousePressed) {
        mousePressed = true;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        for(auto component : components) {
            component->OnClick(x, y);
        }
    }

    if(state == PLAYER_MOVE) {
        player->SetIsHidden(false);
    } else if(state == ENEMY_MOVE) {
        player->SetIsHidden(true);
    }

    if(!moves->EmptyMovesCount()) {
        state = GAME_OVER;
        GameResult gameResult = moves->GetGameResult();

        if(gameResult == WIN) {
            gltSetText(texts["gameResult"].text, "You\nwon\n :D");
        } else if(gameResult == LOSE) {
            gltSetText(texts["gameResult"].text, "You\nlost\n:D");
        } else {
            gltSetText(texts["gameResult"].text, "Draw :')");
        }
    }

    if(state == ENEMY_MOVE) {
        moves->Make500IQMove();
        state = PLAYER_MOVE;
    }
}

void Game::OnMouseMove(double x, double y) {
    for(auto component : components) {
        component->OnMouseMove(x, y);
    }
}

void Game::OnKeyPress(int key, int scancode, int action, int mods) {
    for(auto component : components) {
        component->OnKeyPress(key, scancode, action, mods);
    }
}
