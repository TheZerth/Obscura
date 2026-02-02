#include "obscura/term/Terminal.hpp"
#include "obscura/term/Renderer.hpp"
#include "obscura/term/Input.hpp"
#include "obscura/ecology/World.hpp"
#include "obscura/organisms/Menu.hpp"
#include "obscura/organisms/Box.hpp"
#include "obscura/organisms/Statusline.hpp"

#include <memory>
#include <sstream>
#include <unistd.h>

int main() {
    auto term = obscura::Terminal::open();
    obscura::Renderer renderer;
    obscura::Input input(STDIN_FILENO);

    obscura::World world;
    world.resize(80, 24);

    auto box = std::make_unique<obscura::Box>();
    box->x = 1; box->y = 1; box->w = 30; box->h = 10; box->priority = 0;
    world.add_agent(std::move(box));

    auto menu = std::make_unique<obscura::Menu>();
    menu->x = 3; menu->y = 3;
    menu->items = {"Start", "Settings", "About", "Quit"};
    menu->selected = 0;
    auto* menu_ptr = menu.get();
    world.add_agent(std::move(menu));

    auto status = std::make_unique<obscura::Statusline>();
    status->text = "Obscura demo_menu_ecology (bootstrap)";
    auto* status_ptr = status.get();
    world.add_agent(std::move(status));

    bool running = true;
    while (running) {
        const auto& stats = world.stats();
        std::ostringstream oss;
        oss << "ticks " << stats.ticks
            << " | claims " << stats.claims_emitted
            << " | cells " << stats.cells_written
            << " | contention " << stats.contention_cells
            << " | q/esc to quit";
        status_ptr->text = oss.str();

        world.tick();
        renderer.draw(term, world.screen());

        auto ev = input.read_event();
        if (!ev) continue;
        if (ev->type == obscura::Event::Type::resize) {
            world.resize(ev->resize.cols, ev->resize.rows);
            renderer.invalidate();
            continue;
        }
        if (ev->type != obscura::Event::Type::key) continue;

        if (ev->key.key == obscura::Key::esc) {
            running = false;
            continue;
        }
        if (ev->key.key == obscura::Key::up) {
            if (menu_ptr->selected > 0) menu_ptr->selected--;
            continue;
        }
        if (ev->key.key == obscura::Key::down) {
            if (menu_ptr->selected + 1 < static_cast<int>(menu_ptr->items.size())) {
                menu_ptr->selected++;
            }
            continue;
        }
        if (!ev->key.text.empty()) {
            if (ev->key.text[0] == 'q') running = false;
        }
    }

    return 0;
}
