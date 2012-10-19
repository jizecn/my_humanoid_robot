humanoid_robot
==============

yarp based software

# old work, kept here as a backup #

The software is a complete system for controlling a reactive humanoid robot. 

The robot is built with a combination of different types of servos, inc dynamixel and ssc/minissc controllers, and multimodal sensors, such as tactile sensors, vision and microphone. 

Tactile sensors are based on capacitive sensors, developed by iit in the roboskin project (though i do not have the hardware anymore). Data communication is via can-bus and i2c at different layers. 

The system is highly modularised, allowing re-programming individual components, such as the behaviour selection or sensing algorithms, such as tactile interaction or vision intereaction. 

It also allows remote control from wireless controller.

Components are loosely coupled based on Yarp, which is lightweight and efficient. 