A simulated robot that maps an unknown indoor environment on its own using frontier-based exploration.

## Requirements

- Ubuntu 24.04
- ROS 2 Jazzy ([install guide](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html), `ros-jazzy-desktop`)

## Setup

### 1. Install dependencies

```bash
sudo apt update
sudo apt install ros-jazzy-ros-gz ros-jazzy-slam-toolbox \
  ros-jazzy-navigation2 ros-jazzy-nav2-bringup \
  python3-colcon-common-extensions python3-rosdep
```

Open a **new terminal** after installing, so the Gazebo environment hooks load.

### 2. Build the TurtleBot3 simulation workspace

TurtleBot3 is built in its own workspace, separate from this repo.

```bash
mkdir -p ~/tb3_ws/src && cd ~/tb3_ws/src
git clone -b jazzy https://github.com/ROBOTIS-GIT/DynamixelSDK.git
git clone -b jazzy https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
git clone -b jazzy https://github.com/ROBOTIS-GIT/turtlebot3.git
git clone -b jazzy https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git
cd ~/tb3_ws
sudo rosdep init   # skip if already initialized
rosdep update
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
```

### 3. Configure your environment

Add to `~/.bashrc` (order matters), then open a new terminal:

```bash
source /opt/ros/jazzy/setup.bash
source ~/tb3_ws/install/setup.bash
export TURTLEBOT3_MODEL=burger
export ROS_DOMAIN_ID=30
```

### 4. Clone and build this repo

```bash
git clone git@github.com:<your-username>/<repo>.git ~/frontier_explorer_ws
cd ~/frontier_explorer_ws
colcon build --symlink-install
source install/setup.bash
```

## Usage

### Manual mapping (baseline)

Run each command in its own terminal:

```bash
ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py
ros2 launch slam_toolbox online_async_launch.py use_sim_time:=true
rviz2 -d ~/frontier_explorer_ws/rviz/explore.rviz
ros2 run turtlebot3_teleop teleop_keyboard
```

Save the map once the area is explored:

```bash
ros2 run nav2_map_server map_saver_cli -f ~/frontier_explorer_ws/maps/<map_name>
```

### Autonomous frontier exploration

Coming soon.

## Troubleshooting

- **`Unable to locate package ros-jazzy-...`**: the ROS apt source is missing. Check that `/etc/apt/sources.list.d/` contains `ros2.sources`.
- **`gz-sim8` not found during `colcon build`**: `ros-gz` was installed in a terminal that was already open. Open a new terminal, run `rm -rf build/turtlebot3_gazebo`, and rebuild.
- **`ros2 topic list` shows almost nothing while the sim runs**: the terminals have different `ROS_DOMAIN_ID` values. Check with `printenv ROS_DOMAIN_ID` in each.
