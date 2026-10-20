#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "frontier_explorer/grid_utils.hpp"
#include <vector>

/*
cell mapping:
  unknown: -1
  free: 1
  occupied: 100
*/

namespace frontier_explorer{
  class FrontierExplorerNode : public rclcpp::Node{
    public:
      FrontierExplorerNode() : Node("frontier_explorer"){
        auto qos = rclcpp::QoS(1).transient_local().reliable();
        map_subscription_ = create_subscription<nav_msgs::msg::OccupancyGrid>("/map", qos, [this](const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
          onMap(msg);
        });

        RCLCPP_INFO(get_logger(), "Started, waiting for /map.");
      }
    private:
      
      bool isFrontierCell(const nav_msgs::msg::OccupancyGrid & map, const Cell & c) const{
        // left, right, up, down of a known cell
        constexpr int dx[4] = {1, -1, 0, 0};
        constexpr int dy[4] = {0, 0, 1, -1};

        if(map.data[toIndex(map.info, c)] != 0){
          return false;
        }

        for(int i = 0; i <= 3; i++){
          Cell neighbour{c.x + dx[i], c.y + dy[i]};
          if(neighbour == -1) return true;

          if(neighbour != inBounds(map.info, neighbour)) i++;
        }
        return false;
      }


      std::vector<Cell> findFrontierCells(const nav_msgs::msg::OccupancyGrid & map) const{
        std::vector<Cell> cells;




        return cells;
      }

      void onMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
        int unknown_cells = 0;
        int free_cells = 0;
        int occupied_cells = 0;


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
      }

      rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_subscription_;  
  };
}

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<frontier_explorer::FrontierExplorerNode>());
  rclcpp::shutdown();
  return 0;
}
