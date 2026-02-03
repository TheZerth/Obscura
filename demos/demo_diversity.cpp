#include "obscura/ecology/Population.hpp"
#include "obscura/ecology/World.hpp"
#include "obscura/organisms/Statusline.hpp"
#include "obscura/term/Input.hpp"
#include "obscura/term/Renderer.hpp"
#include "obscura/term/Terminal.hpp"

#include <sstream>
#include <unistd.h>
#include <vector>

int main() {
    auto term = obscura::Terminal::open();
    obscura::Renderer renderer;
    obscura::Input input(STDIN_FILENO);

    obscura::World world;
    world.resize(80, 24);

    obscura::PopulationManager population(world);

    auto status = std::make_unique<obscura::Statusline>();
    status->priority = 50;
    auto* status_ptr = status.get();
    (void)world.add_agent(std::move(status));

    std::vector<obscura::AgentId> created;

    bool running = true;
    while (running) {
        const auto& stats = world.stats();
        int noise_count = 0;
        int blink_count = 0;
        int trail_count = 0;
        int other_count = 0;
        for (const auto& a : stats.agents) {
            if (!a.active) continue;
            if (a.name == "NoiseAgent") {
                noise_count++;
            } else if (a.name == "BlinkAgent") {
                blink_count++;
            } else if (a.name == "TrailAgent") {
                trail_count++;
            } else {
                other_count++;
            }
        }

        std::ostringstream oss;
        oss << "N " << noise_count
            << " B " << blink_count
            << " T " << trail_count
            << " O " << other_count
            << " | gen " << stats.claims_generated
            << " acc " << stats.claims_accepted
            << " drop c:" << stats.claims_dropped_claim_budget
            << " t:" << stats.claims_dropped_time_budget
            << " | hits c:" << stats.claim_budget_hits
            << " t:" << stats.time_budget_hits
            << " f:" << stats.skipped_failures
            << " | n/b/t add r remove c clear q quit";
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

        if (!ev->key.text.empty()) {
            char ch = ev->key.text[0];
            if (ch == 'q') {
                running = false;
            } else if (ch == 'n') {
                created.push_back(population.spawn_noise(obscura::NoiseRange{}));
            } else if (ch == 'b') {
                created.push_back(population.spawn_blink(obscura::BlinkRange{}));
            } else if (ch == 't') {
                created.push_back(population.spawn_trail(obscura::TrailRange{}));
            } else if (ch == 'r') {
                if (!created.empty()) {
                    population.remove(created.back());
                    created.pop_back();
                }
            } else if (ch == 'c') {
                for (auto id : created) {
                    population.remove(id);
                }
                created.clear();
            }
        }
    }

    return 0;
}
