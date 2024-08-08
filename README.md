Build and run:

cd ~/rviz_mod_1.1 && source ~/ws_tomo/install/setup.bash && catkin build

source devel/setup.bash && rosrun rviz quick_render_panel_test

cd ~/rviz_mod_1.1 && source ~/ws_tomo/install/setup.bash && catkin build && source devel/setup.bash && rosrun rviz quick_render_panel_test

cd ~/rviz_mod_1.1 && source ~/ws_tomo/install/setup.bash && catkin build && source devel/setup.bash && roslaunch tomo_ui tomo_ui.launch

Debug:
gdb --args devel/lib/rviz/quick_render_panel_test
gdb --args devel/lib/tomo_ui/tomo_ui

valgrind --leak-check=full devel/lib/rviz/quick_render_panel_test
sudo apt-get install valgrind

On xavier:
cd /usr/local
git clone https://github.com/desmond196/Qt-5.9.5.git

if [ "$(uname -m)" = "armv7l" ] || [ "$(uname -m)" = "aarch64" ]; then
    if [ -d /usr/local/Qt-5.9.5 ]; then
    echo "Directory /usr/local/Qt-5.9.5 exists. Pulling the latest changes."
    cd /usr/local/Qt-5.9.5 || error_exit "Failed to enter directory /usr/local."
    git pull origin master || error_exit "Failed to pull the latest changes."
    else
    echo "Cloning /usr/local/Qt-5.9.5"
    cd /usr/local || error_exit "Failed to enter directory /usr/local."
    git clone https://github.com/desmond196/Qt-5.9.5.git || error_exit "Failed to clone the repository."
    fi
fi


Mod from Rviz 1.13.30 and Moveit 1.0.11
