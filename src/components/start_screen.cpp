#include "start_screen.h"

StartScreen::StartScreen(Game *game) : game(game) {
    title = Text {
        .pos = {game->width/2, game->height/3},
        .color = {RGB_TO_NORMALIZED(242, 181, 212), 1},
        .text = game->textRender.Create("Diagonals"),
        .scale = 7.f
    };

    author = Text {
        .pos = {game->width, game->height},
        .color = {RGB_TO_NORMALIZED(247, 214, 224), 1},
        .text = game->textRender.Create("made by MilkeeyCat"),
        .scale = 2.f
    };

    hint = Text {
        .pos = {game->width/2, game->height/2},
        .color = {RGB_TO_NORMALIZED(247, 214, 224), 1},
        .text = game->textRender.Create("press Space to start"),
        .scale = 2.f
    };

    delta = 0.025f;
}

void StartScreen::Update() {
    if(hint.color.a > 1 || hint.color.a < 0) {
        delta = -delta;
    }

    hint.color.a += delta;
}

void StartScreen::Render() {
    if(game->state == game->GAME_NOT_STARTED) {
        game->textRender.Render(title, GLT_CENTER, GLT_CENTER);
        game->textRender.Render(author, GLT_RIGHT);
        game->textRender.Render(hint, GLT_CENTER, GLT_CENTER);
    }
}
