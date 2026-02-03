#include "obscura/ecology/World.hpp"
#include "obscura/ecology/LocalView.hpp"
#include <iterator>
#include <unordered_map>

namespace obscura {

    void World::add_agent(std::unique_ptr<Agent> a) {
        agents_.push_back(std::move(a));
    }

    void World::emit(Claim c) {
        claims_.push_back(std::move(c));
        stats_.claims_emitted++;
    }

    void World::tick() {
        stats_.reset_frame();
        stats_.ticks++;
        claims_.clear();

        // 1) Agents propose claims
        WorldSize size{screen_.cols(), screen_.rows()};
        for (auto& a : agents_) {
            if (!a) continue;
            ViewSpec spec = a->view_spec(size);
            LocalView view(screen_, spec.center_x, spec.center_y, spec.radius);
            std::vector<Claim> agent_claims;
            a->tick(view, agent_claims);
            stats_.claims_emitted += static_cast<std::uint64_t>(agent_claims.size());
            claims_.insert(claims_.end(),
                           std::make_move_iterator(agent_claims.begin()),
                           std::make_move_iterator(agent_claims.end()));
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
