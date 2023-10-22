#include "game.h"

#include "GLFW/glfw3.h"
#include "components/grid.h"
#include "components/moves.h"
#include "components/player.h"
#include "imgui/backends/imgui_impl_glfw.h"

Game::Game(GLFWwindow *window, unsigned char size, const float thikness) : size(size), window(window), mousePressed(false), state(GAME_NOT_STARTED), THIKNESS(thikness) {
    texts["gameResult"] = gltCreateText();
    texts["hint"] = gltCreateText();

    gltSetText(texts["hint"], "press Space to restart the game");

    int windowWidth, windowHeight;

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    moves = new Moves(windowWidth, windowHeight, THIKNESS, size);
    Grid *grid = new Grid(this, moves, size, THIKNESS, windowWidth, windowHeight);
    player = new Player(moves, THIKNESS, windowWidth, windowHeight, size);

    components.push_back(grid);
    components.push_back(moves);
    components.push_back(player);

    if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
	}
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
        if(key == GLFW_KEY_SPACE && game->state == GAME_NOT_STARTED) {
            game->state = PLAYER_MOVE;
        }

        if(key == GLFW_KEY_SPACE && game->state == GAME_OVER) {
            //restart the game
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
        gltBeginDraw();

        gltColor(0.5f, 1.f, 1.f, 1.f);

        gltDrawText2DAligned(
                texts["gameResult"],
                300.f,
                250.f,
                10.f,
                GLT_CENTER,
                GLT_CENTER
        );

        gltColor(0.1f, .5f, 1.f, 1.f);

        gltDrawText2DAligned(texts["hint"], 300.f, 575.f, 2.f, GLT_CENTER, GLT_BOTTOM);

        gltEndDraw();
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
            gltSetText(texts["gameResult"], "You\nwon\n :D");
        } else if(gameResult == LOSE) {
            gltSetText(texts["gameResult"], "You\nlost\n:D");
        } else {
            gltSetText(texts["gameResult"], "Draw :')");
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
