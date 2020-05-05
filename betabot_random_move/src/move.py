#!/usr/bin/env python

import rospy
import math
import time

from geometry_msgs.msg import Twist 
from sensor_msgs.msg import LaserScan
from std_msgs.msg import Float32
from random import  choice
# import sys, select, termios, tty


class Mover():

	def __init__(self):


		self.pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)
		self.sub = rospy.Subscriber("scan", LaserScan, self.callback)

		# definiing the fields to be filled by the laser scanner
		
		self.max_range= Float32()		# the max readble range for the bot in meteres
		self.min_range= Float32()		# the minimum available range in meteres
		self.angle_min= Float32()        # start angle of the scan [rad]
		self.angle_max= Float32()        # end angle of the scan [rad]
		self.angle_increment= Float32()  # angular distance between measurements [rad]

		self.regions=dict()

		self.max_range=8.000000	# the max readble range for the bot in meteres
		self.min_range=0.100000		# the minimum available range in meteres
		self.angle_min=0.000000        # start angle of the scan [rad]
		self.angle_max=6.283120        # end angle of the scan [rad]
		self.angle_increment=0.003143  # angular distance between measurements [rad]


		self.normal_vel =0.3 # the linear velocity applied in normal conditions
		self.normal_angel_turn = -1  # the angular velocity applied in normal conditions
		self.min_vel =0.0 # the velocity applied in case of the existance of a nearby object
		self.min_angel = -0.3 # the angular velocity applied in case of  a nearby object
		# half_angel =1.57284 # the half of the angel fo
		self.move()
	def angleToIndex(self,angle):
		angle=angle*math.pi/180
		return -1+(angle-self.angle_min)//self.angle_increment

	def callback(self,data):
		
		self.regions = {
        'right':  min(min(data.ranges[999:1199]), 8),
        'fright': min(min(data.ranges[1200:1399]), 8),
        'front':  min(min(data.ranges[1399:1599]), 8),
        'fleft':  min(min(data.ranges[1599:1799]), 8),
        'left':   min(min(data.ranges[1799:1999]), 8),
		}
  
		# self.range_left.data= data.ranges[0] # range value on the left of left of the bot
		# self.range_front_left.data= data.ranges[1749] # range valuee on the front left of the bot
		# self.range_front.data= data.ranges[1499] # range value in front of the bot
		# self.range_front_right.data= data.ranges[1249] # range valuee on the front right bot
		# self.range_right.data= data.ranges[999] # range value on the right of the bot
		
		#those three can be used to know the exact index of the specific angle we want (for now i have used the numbers i found and applied it)
		self.angle_min=data.angle_min
		self.angle_max=data.angle_max
		self.angle_increment= data.angle_increment
	def move(self):
		
		rospy.init_node('move', anonymous=True)
		rate = rospy.Rate(3) # 10hz
		twist = Twist()
		thresh=.3
  		period=0.5
		self.linear_x = 0
		self.angular_z = 0
		while not rospy.is_shutdown():
	
			state_description = ''
			rospy.loginfo("left:%f ,frontleft: %f ,front:%f ,front right:%f ,right:%f",self.regions['left'],self.regions['fleft'],self.regions['front'],self.regions['fright'],self.regions['right']) # range value in front of the bot
		
			if self.regions['front'] > thresh and self.regions['fleft'] > thresh and self.regions['fright'] > thresh:
				state_description = 'case thresh - nothing'
				linear_x = self.normal_vel
				angular_z = 0
			elif self.regions['front'] < thresh and self.regions['fleft'] > thresh and self.regions['fright'] > thresh:
				state_description = 'case 2 - front'
				linear_x = 0
				angular_z = -self.normal_angel_turn * (-1)*choice([1,-1])
				time.sleep(period)
    
			elif self.regions['front'] > thresh and self.regions['fleft'] > thresh and self.regions['fright'] < thresh:
				state_description = 'case 3 - fright'
				linear_x = self.min_vel
				angular_z = -self.normal_angel_turn
				time.sleep(period*choice([0,1]))
			elif self.regions['front'] > thresh and self.regions['fleft'] < thresh and self.regions['fright'] > thresh:
				state_description = 'case 4 - fleft'
				linear_x = self.min_vel
				angular_z = self.normal_angel_turn
				time.sleep(period*choice([0,1]))
			elif self.regions['front'] < thresh and self.regions['fleft'] > thresh and self.regions['fright'] < thresh:
				state_description = 'case 5 - front and fright'
				linear_x = self.min_vel
				angular_z = -self.normal_angel_turn
				time.sleep(period*choice([0,1]))
			elif self.regions['front'] < thresh and self.regions['fleft'] < thresh and self.regions['fright'] > thresh:
				state_description = 'case 6 - front and fleft'
				linear_x = self.min_vel
				angular_z = self.normal_angel_turn
				time.sleep(period*choice([0,1]))
			elif self.regions['front'] < thresh and self.regions['fleft'] < thresh and self.regions['fright'] < thresh:
				state_description = 'case 7 - front and fleft and fright'
				linear_x = self.min_vel
				angular_z = self.normal_angel_turn * (-1)*choice([1,-1])
				time.sleep(period*choice([0,1]))
			elif self.regions['front'] > thresh and self.regions['fleft'] < thresh and self.regions['fright'] < thresh:
				state_description = 'case 8 - fleft and fright'
				linear_x = self.min_vel
				angular_z =self.normal_angel_turn * (-1)*choice([1,-1])
				time.sleep(period*choice([0,1]))
			else:
				state_description = 'unknown case'
				rospy.loginfo(self.regions)
		
			rospy.loginfo(state_description)
			twist.linear.x = linear_x
			twist.angular.z = angular_z
		
			self.pub.publish(twist)			#we can add another condition (law elrobot mazno2) but it I will need to know the exact dimensions of the robot
			rate.sleep()
			

if __name__ == '__main__':
	
	try:
		Mover()
		
	except rospy.ROSInterruptException:
		pass
	

