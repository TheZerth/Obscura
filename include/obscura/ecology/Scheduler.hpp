#pragma once
#include "obscura/ecology/Agent.hpp"
#include "obscura/ecology/Claim.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

namespace obscura {

    struct SchedulerConfig {
        std::size_t max_claims_per_tick = 256; // 0 means unlimited
        std::chrono::microseconds max_tick_duration = std::chrono::microseconds(0); // 0 means unlimited
        double failure_rate = 0.0; // 0.0 - 1.0
        int shuffle_retries = 1; // avoid repeating the last order when possible
    };

    class World;

    class Scheduler {
    public:
        virtual ~Scheduler() = default;
        virtual void execute(World& world,
                             std::vector<std::unique_ptr<Agent>>& agents,
                             std::vector<Claim>& out_claims) = 0;

        virtual void set_config(const SchedulerConfig& config) = 0;
        virtual const SchedulerConfig& config() const = 0;
    };

    class RandomScheduler final : public Scheduler {
    public:
        RandomScheduler();
        explicit RandomScheduler(std::uint64_t seed);

        void execute(World& world,
                     std::vector<std::unique_ptr<Agent>>& agents,
                     std::vector<Claim>& out_claims) override;

        void set_config(const SchedulerConfig& config) override { config_ = config; }
        const SchedulerConfig& config() const override { return config_; }

    private:
        std::mt19937_64 rng_;
        SchedulerConfig config_{};
        std::vector<std::size_t> order_;
        std::vector<std::size_t> last_order_;

        bool should_fail_();
        void shuffle_order_(std::size_t count);
    };

} // namespace obscura
