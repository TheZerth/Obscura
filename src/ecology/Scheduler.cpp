#include "obscura/ecology/Scheduler.hpp"
#include "obscura/ecology/LocalView.hpp"
#include "obscura/ecology/World.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>

namespace obscura {

    namespace {
        std::uint64_t seed_from_random_device() {
            std::random_device rd;
            std::uint64_t seed = static_cast<std::uint64_t>(rd());
            seed = (seed << 32) ^ static_cast<std::uint64_t>(rd());
            return seed;
        }
    } // namespace

    RandomScheduler::RandomScheduler() : rng_(seed_from_random_device()) {}

    RandomScheduler::RandomScheduler(std::uint64_t seed) : rng_(seed) {}

    bool RandomScheduler::should_fail_() {
        if (config_.failure_rate <= 0.0) return false;
        if (config_.failure_rate >= 1.0) return true;
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng_) < config_.failure_rate;
    }

    void RandomScheduler::shuffle_order_(std::size_t count) {
        order_.resize(count);
        std::iota(order_.begin(), order_.end(), 0);
        if (order_.size() < 2) return;

        std::shuffle(order_.begin(), order_.end(), rng_);
        if (config_.shuffle_retries > 0 && order_ == last_order_) {
            for (int i = 0; i < config_.shuffle_retries && order_ == last_order_; ++i) {
                std::shuffle(order_.begin(), order_.end(), rng_);
            }
        }
        last_order_ = order_;
    }

    void RandomScheduler::execute(World& world,
                                  std::vector<std::unique_ptr<Agent>>& agents,
                                  std::vector<Claim>& out_claims) {
        auto& stats = world.stats();
        stats.ensure_agents(agents.size());

        WorldSize size{world.screen().cols(), world.screen().rows()};
        shuffle_order_(agents.size());

        for (std::size_t idx : order_) {
            if (idx >= agents.size()) continue;
            auto& agent = agents[idx];
            if (!agent) continue;

            if (should_fail_()) {
                stats.agents[idx].skipped_failures++;
                stats.skipped_failures++;
                continue;
            }

            ViewSpec spec = agent->view_spec(size);
            LocalView view(world.screen(), spec.center_x, spec.center_y, spec.radius);

            std::vector<Claim> agent_claims;
            auto start = std::chrono::steady_clock::now();
            agent->tick(view, agent_claims);
            auto end = std::chrono::steady_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::uint64_t duration_us = static_cast<std::uint64_t>(duration.count());
            auto& a_stats = stats.agents[idx];
            a_stats.ticks++;
            if (duration_us > a_stats.max_tick_time_us) a_stats.max_tick_time_us = duration_us;

            bool time_over = config_.max_tick_duration.count() > 0 && duration > config_.max_tick_duration;
            if (time_over) {
                a_stats.time_budget_hits++;
                stats.time_budget_hits++;
            }

            std::size_t accepted = agent_claims.size();
            if (config_.max_claims_per_tick > 0 && accepted > config_.max_claims_per_tick) {
                accepted = config_.max_claims_per_tick;
                a_stats.claim_budget_hits++;
                stats.claim_budget_hits++;
            }

            if (time_over) {
                accepted = 0;
            }

            a_stats.claims_emitted += accepted;
            if (accepted == 0) continue;

            auto begin_it = agent_claims.begin();
            auto end_it = agent_claims.begin() + static_cast<std::ptrdiff_t>(accepted);
            out_claims.insert(out_claims.end(),
                              std::make_move_iterator(begin_it),
                              std::make_move_iterator(end_it));
        }
    }

} // namespace obscura
