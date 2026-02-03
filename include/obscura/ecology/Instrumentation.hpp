#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace obscura {

    struct AgentStats {
        std::uint64_t ticks = 0;
        std::uint64_t claims_emitted = 0;
        std::uint64_t claim_budget_hits = 0;
        std::uint64_t time_budget_hits = 0;
        std::uint64_t skipped_failures = 0;
        std::uint64_t max_tick_time_us = 0;
    };

    struct Instrumentation {
        std::uint64_t ticks = 0;
        std::uint64_t claims_emitted = 0;
        std::uint64_t cells_written = 0;
        std::uint64_t contention_cells = 0;
        std::uint64_t claim_budget_hits = 0;
        std::uint64_t time_budget_hits = 0;
        std::uint64_t skipped_failures = 0;

        std::vector<AgentStats> agents;

        void reset_frame() {
            claims_emitted = 0;
            cells_written = 0;
            contention_cells = 0;
            claim_budget_hits = 0;
            time_budget_hits = 0;
            skipped_failures = 0;
        }

        void ensure_agents(std::size_t count) {
            if (agents.size() < count) agents.resize(count);
        }
    };

} // namespace obscura
