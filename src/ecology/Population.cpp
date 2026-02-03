#include "obscura/ecology/Population.hpp"

namespace obscura {

    namespace {
        std::uint64_t seed_from_device() {
            std::random_device rd;
            std::uint64_t seed = static_cast<std::uint64_t>(rd());
            seed = (seed << 32) ^ static_cast<std::uint64_t>(rd());
            return seed;
        }
    } // namespace

    PopulationManager::PopulationManager(World& world, std::uint64_t seed)
        : world_(world), rng_(seed == 0 ? seed_from_device() : seed) {}

    AgentId PopulationManager::spawn_noise(const NoiseRange& range) {
        auto agent = make_random_noise_agent(range, rng_, world_.size());
        return world_.add_agent(std::move(agent));
    }

    AgentId PopulationManager::spawn_blink(const BlinkRange& range) {
        auto agent = make_random_blink_agent(range, rng_, world_.size());
        return world_.add_agent(std::move(agent));
    }

    AgentId PopulationManager::spawn_trail(const TrailRange& range) {
        auto agent = make_random_trail_agent(range, rng_, world_.size());
        return world_.add_agent(std::move(agent));
    }

} // namespace obscura
