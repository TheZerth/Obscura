#pragma once
#include "obscura/ecology/Claim.hpp"
#include "obscura/ecology/CellSettle.hpp"
#include "obscura/ecology/Instrumentation.hpp"
#include "obscura/ecology/Scheduler.hpp"
#include "obscura/term/Screen.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace obscura {

    class World {
    public:
        World();
        explicit World(std::unique_ptr<Scheduler> scheduler);

        void resize(int cols, int rows) { screen_.resize(cols, rows); }
        Screen& screen() noexcept { return screen_; }
        const Screen& screen() const noexcept { return screen_; }

        Instrumentation& stats() noexcept { return stats_; }
        const Instrumentation& stats() const noexcept { return stats_; }

        Scheduler& scheduler() noexcept { return *scheduler_; }
        const Scheduler& scheduler() const noexcept { return *scheduler_; }
        void set_scheduler(std::unique_ptr<Scheduler> scheduler);

        void add_agent(std::unique_ptr<Agent> a);
        void tick(); // run one tick of emergence

        // Optional: external systems may propose cell content.
        void emit(Claim c);

    private:
        Screen screen_;
        Instrumentation stats_;
        CellSettle settle_;
        std::unique_ptr<Scheduler> scheduler_;

        std::vector<std::unique_ptr<Agent>> agents_;
        std::uint64_t next_agent_id_ = 1;

        // For bootstrap: store claims in a simple vector, then settle by cell.
        std::vector<Claim> claims_;
    };

} // namespace obscura
