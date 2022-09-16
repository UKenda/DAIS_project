#!/usr/bin/python3
# Potrebno dati robota na obmocje kjer lahko zazna le en objekt.... nato se rocno izpisejo podatki za nadlajni preracun

from curses import KEY_OPTIONS
import rospy


from sensor_msgs.msg import LaserScan
from darknet_ros_msgs.msg import BoundingBoxes





class Node:
    pub_markers: rospy.Publisher

    def __init__(self):
        rospy.init_node("lidar_angle")
        rospy.Subscriber("/darknet_ros/bounding_boxes", BoundingBoxes, self.boundig_box_callback, queue_size=1)
        rospy.Subscriber("/scan", LaserScan, self.scan_callback, queue_size=1)

        self.yolo_detection = None


        rospy.spin()
    def scan_callback(self, msg: LaserScan):
        self.scan=msg

    def boundig_box_callback(self, msg: BoundingBoxes):
        self.yolo_detection = msg
        if self.yolo_detection is not None :
            #print(self.yolo_detection)
            
            for box in self.yolo_detection.bounding_boxes:
               
                if box.Class == "person":
                    center_x = (box.xmax + box.xmin)/2
                    center_y = (box.ymax + box.ymin)/2
                    angle = 0
                    distance = 0
                    points = 0
                    l=0
                    while l < 359:
                        l +=1
                        if self.scan.ranges[l] > 0.2 and self.scan.ranges[l]<20:
                            distance += self.scan.ranges[l]
                            points += 1
                            angle += l

                    if points > 0:
                        distance = distance / points
                        angle = (angle/points)/2
                        print("Camera ",center_x," ",center_y," LiDAR ",distance, " ",angle)
                        print("predvidevanje angle: ", -0.1356*center_x-0.0474*center_y+145.586)
                        print("")
                        
                    


def main():
    Node()


if __name__ == "__main__":
    main()