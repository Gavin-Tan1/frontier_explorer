#pragma once

#include <cmath>
#include <optional>
#include "nav_msgs/msg/map_meta_data.hpp"

namespace frontier_explorer{
    struct Cell{
        int x;
        int y;

    };

    struct WorldPoint{
        double x;
        double y;
    };

    // start at origin and go x steps * resolution
    inline WorldPoint gridToWorld(const nav_msgs::msg::MapMetaData & info, const Cell & c){
        double wx = info.origin.position.x + (c.x + 0.5) * info.resolution;
        double wy = info.origin.position.y + (c.y + 0.5) * info.resolution;

        return WorldPoint{wx, wy};
    }

    inline std::optional<Cell> worldToGrid(const nav_msgs::msg::MapMetaData & info, double wx, double wy){
        int x = static_cast<int>(std::floor((wx - info.origin.position.x) / info.resolution));
        int y = static_cast<int>(std::floor((wy - info.origin.position.y) / info.resolution));

        if(x < 0 || x >= static_cast<int>(info.width)){
            return std::nullopt;
        }

        if(y < 0 || y >= static_cast<int>(info.height)){
            return std::nullopt;
        }

        return Cell{x, y};
    }

    inline int toIndex(const nav_msgs::msg::MapMetaData & info, const Cell & c){
        return static_cast<int>(info.width) * c.y + c.x;
    }
}