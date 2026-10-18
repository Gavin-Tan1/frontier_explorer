#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "frontier_explorer/grid_utils.hpp"

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
      void onMap(const nav_msgs::msg::OccupancyGrid::SharedPtr msg){
        RCLCPP_INFO(get_logger(), "Recieved map %u x %u", msg->info.width, msg->info.height);
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
