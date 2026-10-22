#pragma once

#include <vector>
#include "rclcpp/time.hpp"
#include "visualization_msgs/msg/marker_array.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "nav_msgs/msg/map_meta_data.hpp"
#include "frontier_explorer/grid_utils.hpp"

namespace frontier_explorer{
    inline visualization_msgs::msg::MarkerArray buildFrontierMarkers(const nav_msgs::msg::MapMetaData & info, const std::vector<Cell> & cells, const rclcpp::Time & stamp){
        visualization_msgs::msg::MarkerArray array;
        visualization_msgs::msg::Marker clear;
        clear.action = visualization_msgs::msg::Marker::DELETEALL;
        array.markers.push_back(clear);

        visualization_msgs::msg::Marker m;
        m.header.frame_id = "map";
        m.header.stamp = stamp;
        m.ns = "frontier_cells";
        m.id = 0;
        m.type = visualization_msgs::msg::Marker::POINTS;
        m.action = visualization_msgs::msg::Marker::ADD;
        m.pose.orientation.w = 1.0;
        m.scale.x = info.resolution;
        m.scale.y = info.resolution;
        m.color.r = 1.0f;
        m.color.g = 0.5f;
        m.color.b = 0.0f;
        m.color.a = 1.0f;

        for(const Cell & c : cells){
            WorldPoint wp = gridToWorld(info, c);

            geometry_msgs::msg::Point p;
            p.x = wp.x;
            p.y = wp.y;
            p.z = 0.0;
            m.points.push_back(p);
        }

        



        array.markers.push_back(m);
        return array;
    }
}