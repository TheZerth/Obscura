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
        const Screen& screen() const noexcept { return screen_; }
        WorldSize size() const noexcept { return WorldSize{screen_.cols(), screen_.rows()}; }

        Instrumentation& stats() noexcept { return stats_; }
        const Instrumentation& stats() const noexcept { return stats_; }

        Scheduler& scheduler() noexcept { return *scheduler_; }
        const Scheduler& scheduler() const noexcept { return *scheduler_; }
        void set_scheduler(std::unique_ptr<Scheduler> scheduler);

        AgentId add_agent(std::unique_ptr<Agent> a);
        bool remove_agent(AgentId id);
        Agent* agent_by_id(AgentId id);
        const Agent* agent_by_id(AgentId id) const;
        void tick(); // run one tick of emergence

    private:
        void emit(Claim c);

        Screen screen_;
        Instrumentation stats_;
        CellSettle settle_;
        std::unique_ptr<Scheduler> scheduler_;

        std::vector<std::unique_ptr<Agent>> agents_;
        std::uint64_t next_agent_id_ = 1;
        std::vector<AgentId> agent_ids_;

        // For bootstrap: store claims in a simple vector, then settle by cell.
        std::vector<Claim> claims_;
    };

} // namespace obscura
