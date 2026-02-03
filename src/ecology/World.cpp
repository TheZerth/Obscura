#include "obscura/ecology/World.hpp"
#include <unordered_map>

namespace obscura {

    World::World() : scheduler_(std::make_unique<RandomScheduler>()) {}

    World::World(std::unique_ptr<Scheduler> scheduler)
        : scheduler_(scheduler ? std::move(scheduler) : std::make_unique<RandomScheduler>()) {}

    void World::set_scheduler(std::unique_ptr<Scheduler> scheduler) {
        scheduler_ = scheduler ? std::move(scheduler) : std::make_unique<RandomScheduler>();
    }

    void World::add_agent(std::unique_ptr<Agent> a) {
        auto* raw = a.get();
        agents_.push_back(std::move(a));
        stats_.ensure_agents(agents_.size());
        std::size_t idx = agents_.size() - 1;
        auto& a_stats = stats_.agents[idx];
        a_stats.id = next_agent_id_++;
        if (raw) {
            const char* n = raw->name();
            a_stats.name = n ? n : "Agent";
        }
    }

    void World::emit(Claim c) {
        claims_.push_back(std::move(c));
        stats_.claims_generated++;
        stats_.claims_accepted++;
    }

    void World::tick() {
        stats_.reset_frame();
        stats_.ticks++;
        claims_.clear();

        // 1) Scheduler executes agents and gathers claims
        if (scheduler_) {
            scheduler_->execute(*this, agents_, claims_);
        }

        // 2) Settle claims by cell (minimal: bucket by (x,y))
        // For bootstrap we do a simple hash map bucket.
        struct Key {
            int x, y;
            bool operator==(const Key& o) const noexcept { return x == o.x && y == o.y; }
        };
        struct KeyHash {
            std::size_t operator()(const Key& k) const noexcept {
                return (static_cast<std::size_t>(k.x) << 32) ^ static_cast<std::size_t>(k.y);
            }
        };

        std::unordered_map<Key, std::vector<Claim>, KeyHash> buckets;
        buckets.reserve(claims_.size());

        for (const auto& c : claims_) {
            if (c.x < 0 || c.y < 0 || c.x >= screen_.cols() || c.y >= screen_.rows()) continue;
            buckets[{c.x, c.y}].push_back(c);
        }

        // 3) Write settled cells into screen
        screen_.clear();
        for (auto& [key, vec] : buckets) {
            if (vec.size() > 1) stats_.contention_cells++;
            int idx = settle_.choose(vec);
            if (idx >= 0) {
                screen_.put(key.x, key.y, vec[idx].glyph);
                stats_.cells_written++;
            }
        }
    }

} // namespace obscura
