Build and run:

cd ~/rviz_mod_1.1 && source ~/ws_tomo/install/setup.bash && catkin build && source install/setup.bash && roslaunch tomo_ui tomo_ui.launch

Debug:
gdb --args install/lib/tomo_ui/tomo_ui
valgrind --leak-check=full devel/lib/tomo_ui/tomo_ui
sudo apt-get install valgrind


Mod from Rviz 1.13.30 and Moveit 1.0.11
