#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "frontier_explorer/grid_utils.hpp"
#include "frontier_explorer/frontier_markers_rviz.hpp"
#include <vector>

/*
cell mapping:
  unknown: -1
  free: 0
  occupied: 100
*/

namespace frontier_explorer{
  class FrontierExplorerNode : public rclcpp::Node{
    public:
      FrontierExplorerNode() : Node("frontier_explorer"){
        marker_pub_ = create_publisher<visualization_msgs::msg::MarkerArray>("frontier_markers", 10);
        auto qos = rclcpp::QoS(1).transient_local().reliable();
        map_subscription_ = create_subscription<nav_msgs::msg::OccupancyGrid>("/map", qos, [this](const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
          onMap(msg);
        });

        RCLCPP_INFO(get_logger(), "Started, waiting for /map.");
      }
    private:
      
      bool isFrontierCell(const nav_msgs::msg::OccupancyGrid & map, const Cell & c) const{
        // right, left, up, down of a known cell
        constexpr int dx[4] = {1, -1, 0, 0};
        constexpr int dy[4] = {0, 0, 1, -1};

        if(map.data[toIndex(map.info, c)] != 0){
          return false;
        }

        for(int i = 0; i <= 3; i++){
          Cell neighbour{c.x + dx[i], c.y + dy[i]};
          if(!inBounds(map.info, neighbour)) continue;
          if(map.data[toIndex(map.info, neighbour)] == -1) return true;
        }
        return false;
      }


      std::vector<Cell> findFrontierCells(const nav_msgs::msg::OccupancyGrid & map) const{
        std::vector<Cell> cells;

        for(int x = 0; x < static_cast<int>(map.info.width); x++){
          for(int y = 0; y < static_cast<int>(map.info.height); y++){
            Cell c{x, y};
            if(isFrontierCell(map, c)){
              cells.push_back(c);
            }
          }
        }
        return cells;
      }

      
      

      void onMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
        
        int unknown_cells = 0;
        int free_cells = 0;
        int occupied_cells = 0;

        auto frontier_cells = findFrontierCells(*msg);
        marker_pub_->publish(buildFrontierMarkers(msg->info, frontier_cells, now()));


        for(int8_t cell_value : msg->data){
          if(cell_value == -1){
            unknown_cells += 1;
          }else if(cell_value == 0){
            free_cells += 1;
          }else if(cell_value == 100){
            occupied_cells += 1;
          }
        }

        RCLCPP_INFO(get_logger(), "map %ux%u  unknown = %d  free = %d  occupied = %d", msg->info.width, msg->info.height, unknown_cells, free_cells, occupied_cells);
        RCLCPP_INFO(get_logger(), "frontier cells: %zu", frontier_cells.size());
      }

      rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_subscription_;  
      rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
  };
}

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<frontier_explorer::FrontierExplorerNode>());
  rclcpp::shutdown();
  return 0;
}
