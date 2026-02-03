## Obscura

**Obscura** is an experimental terminal UI system built around *emergent programming* rather than traditional widget or layout frameworks.

Instead of centralized rendering, Obscura treats the terminal as a living field where many small, local agents propose changes to individual cells. Simple settlement rules resolve conflicts, and coherent interfaces—menus, boxes, status lines, and interaction flows—*emerge* through iteration and constraint.

The project is intentionally minimal and TTY-first. It assumes a basic ANSI-style terminal, avoids heavyweight abstractions, and emphasizes:

* locality over global layout
* composition over inheritance
* constraints as a source of structure and beauty
* robustness through decentralization
* instrumentation and observation as first-class design tools

Obscura is designed as a laboratory as much as a toolkit: a place to explore how complex, legible, and expressive terminal interfaces can arise from small rules, limited resources, and deliberate constraints.

It favors durability, hackability, and long-term clarity over completeness or trend alignment.

### Scheduling and budgets

World updates are run through a `Scheduler` (default: `RandomScheduler`) that shuffles agent order each tick and enforces resource limits. This removes implicit update priority and lets you study how local agents adapt under constraints.

Configuration example:

```cpp
obscura::World world;
auto scheduler = std::make_unique<obscura::RandomScheduler>();
obscura::SchedulerConfig cfg;
cfg.max_claims_per_tick = 128; // 0 = unlimited
cfg.max_tick_duration = std::chrono::microseconds(500); // 0 = unlimited
cfg.failure_rate = 0.02; // random per-agent skip probability
scheduler->set_config(cfg);
world.set_scheduler(std::move(scheduler));
```

Instrumentation tracks claim/time budget hits, skips, and per-agent totals (including longest tick time) so you can observe how constraints reshape emergent behavior.
