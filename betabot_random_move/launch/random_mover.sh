#!/bin/bash


# Launch the nodes
xterm -geometry 50x15+500+250  -e "source devel/setup.bash; roslaunch robert  roslaunch betabot_random_move random_move.launch" &
sleep 5

xterm -geometry 50x15+805+488  -e "source devel/setup.bash; rosrun betabot_random_move move.pyrosrun betabot_random_move move.py"


