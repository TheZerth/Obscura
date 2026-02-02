#pragma once
namespace obscura {

    class World;

    struct Agent {
        virtual ~Agent() = default;
        virtual void tick(World& world) = 0;
    };

} // namespace obscura