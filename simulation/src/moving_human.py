#!/usr/bin/python3
import rospy
import time
from gazebo_msgs.msg import ModelState



points = []
old_point = []
x = 0
y = 0
z = 0
roll = 0
pitch = 0
yaw = 0
pi_deg = 180 #[deg]


rospy.init_node("human_node")
pub = rospy.Publisher("/gazebo/set_model_state",ModelState,queue_size=1)
points = [[0,0],[0,0],[0,900],[400,900],[400,0]] #desired points
current_point =  points[0]
model_state = ModelState()
orientation = [0,0,0] 
#continius loop
while not rospy.is_shutdown():
    i = 0
    for point in points: 
        i +=1
        print("pose ", i)
        print("trenutna tocka: ", current_point)
        print("zeljena tocka: ", point)
        while abs(point[0]-current_point[0]) > 5 or abs(point[1]-current_point[1]) > 5:
            time.sleep(0.05)
            print("x napaka: ",abs(point[0]-current_point[0])," y napaka: ",abs(point[1]-current_point[1]))
            if point[0] > current_point[0]:
                #print("x+")
                current_point[0] +=5
                orientation = [0,0,1.57]
            elif point[0] < current_point[0]:
                #print("x-")
                current_point[0] -=5
                orientation = [0,0,-1.57]
            if point[1] > current_point[1]:
                #print("y+")
                current_point[1] +=5
                orientation = [0,0,3.14]
            elif point[1] < current_point[1]:
                #print("y-")
                current_point[1] -=5 
                orientation = [0,0,0]   
            model_state.model_name = "human"
            model_state.pose.position.x = current_point[0]/100
            model_state.pose.position.y = current_point[1]/100
            model_state.pose.orientation.z = orientation[2]
            model_state.pose.orientation.w = 1
            #publishing a human 1
            pub.publish(model_state)
        current_point = point
        print("done")




        
