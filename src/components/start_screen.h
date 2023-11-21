#include "component.h"
#include "game.h"

class StartScreen : public Component {
public:
    StartScreen(Game *game);
    void Render();
    void Update();
    Game *game;

    Text title;
    Text author;
    Text hint;

    float delta;
};
