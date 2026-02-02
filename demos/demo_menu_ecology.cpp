#include "obscura/term/Terminal.hpp"
#include "obscura/term/Renderer.hpp"
#include "obscura/ecology/World.hpp"
#include "obscura/organisms/Menu.hpp"
#include "obscura/organisms/Box.hpp"
#include "obscura/organisms/Statusline.hpp"

#include <memory>

int main() {
    auto term = obscura::Terminal::open();
    obscura::Renderer renderer;

    obscura::World world;
    world.resize(80, 24);

    auto box = std::make_unique<obscura::Box>();
    box->x = 1; box->y = 1; box->w = 30; box->h = 10; box->priority = 0;
    world.add_agent(std::move(box));

    auto menu = std::make_unique<obscura::Menu>();
    menu->x = 3; menu->y = 3;
    menu->items = {"Start", "Settings", "About", "Quit"};
    menu->selected = 0;
    world.add_agent(std::move(menu));

    auto status = std::make_unique<obscura::Statusline>();
    status->text = "Obscura demo_menu_ecology (bootstrap)";
    world.add_agent(std::move(status));

    // One tick + draw (bootstrap). Next we’ll add input loop.
    world.tick();
    renderer.draw(term, world.screen());

    return 0;
}
