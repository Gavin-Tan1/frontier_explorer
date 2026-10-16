#include "frontier_explorer/grid_utils.hpp"
#include <iostream>

int main(){
  nav_msgs::msg::MapMetaData info;

  info.resolution = 0.05;
  info.width = 80;
  info.height = 102;
  info.origin.position.x = -0.93;
  info.origin.position.y = -2.056;
  
  /*
    world to grid: wx, wy -> x, y

    x = 0.0 + 0.93 / 0.05 = 18.6 = 18
    y = 0.0 + 2.056 / 0.05 = 41.12 = 41

  */
  auto cell = frontier_explorer::worldToGrid(info, 0.0, 0.0);

  if(cell){
    std::cout << "cell:" << cell -> x << "," << cell -> y << std::endl;
  }else{
    std::cout << "outside the map" << std::endl;
  }

  /*
    grid to world: x, y -> wx, wy

    wx = -0.93 + (1 + 0.5) * 0.05 = -0.855
    wy = -2.056 + (3 + 0.5) * 0.05 = -1.881

  */
  auto point = frontier_explorer::gridToWorld(info, frontier_explorer::Cell{1,3});

  std::cout << "point:" << point.x << "," << point.y << std::endl;
  
  /*
    find the 2D index from 1D array
    
    index = 80 * 1 + 2 = 82

  */
  auto index = frontier_explorer::toIndex(info, frontier_explorer::Cell{2,1});

  std::cout << "index:" << index << std::endl;

  // should print outside
  auto outside = frontier_explorer::worldToGrid(info, -5.0, 0.0);

  if(outside){
    std::cout << "cell:" << cell -> x << "," << cell -> y << std::endl;
  }else{
    std::cout << "outside the map" << std::endl;
  }


  return 0;
}
