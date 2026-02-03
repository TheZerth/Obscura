#pragma once
#include "obscura/ecology/World.hpp"
#include "obscura/organisms/BlinkAgent.hpp"
#include "obscura/organisms/NoiseAgent.hpp"
#include "obscura/organisms/TrailAgent.hpp"
#include <cstdint>
#include <random>

namespace obscura {

    class PopulationManager {
    public:
        explicit PopulationManager(World& world, std::uint64_t seed = 0);

        AgentId spawn_noise(const NoiseRange& range);
        AgentId spawn_blink(const BlinkRange& range);
        AgentId spawn_trail(const TrailRange& range);

        bool remove(AgentId id) { return world_.remove_agent(id); }
        std::mt19937_64& rng() { return rng_; }

    private:
        World& world_;
        std::mt19937_64 rng_;
    };

} // namespace obscura
