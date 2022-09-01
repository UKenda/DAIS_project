#!/usr/bin/python3
# Step 3: publish an ImageMarkerArray topic to draw a box around each face.

import cv2
import rospy

from cv_bridge import CvBridge

from std_msgs.msg import ColorRGBA
from sensor_msgs.msg import Image
from foxglove_msgs.msg import ImageMarkerArray
from visualization_msgs.msg import ImageMarker
from geometry_msgs.msg import Point
from darknet_ros_msgs.msg import BoundingBoxes


cv_bridge = CvBridge()


class Node:
    pub_markers: rospy.Publisher

    def __init__(self):
        rospy.init_node("my_node")
        rospy.Subscriber("/camera/rgb/image_raw", Image, self.image_callback, queue_size=1)
        rospy.Subscriber("/darknet_ros/bounding_boxes", BoundingBoxes, self.boundig_box_callback, queue_size=1)

        self.pub_person = rospy.Publisher(
            "/yolo/persons", ImageMarkerArray, queue_size=1
        )
        self.pub_other = rospy.Publisher(
            "/yolo/other", ImageMarkerArray, queue_size=1
        )
        self.yolo_detection = None


        rospy.spin()

    def boundig_box_callback(self, msg: BoundingBoxes):
        self.yolo_detection = msg


    def image_callback(self, msg: Image):
        # Convert the ROS Image to a grayscale OpenCV image
        cv_img = cv_bridge.imgmsg_to_cv2(msg)
        grayscale_img = cv2.cvtColor(cv_img, cv2.COLOR_BGR2GRAY)

        # Display each face as a cyan rectangle
        markers_person = ImageMarkerArray()
        markers_other = ImageMarkerArray()
        if self.yolo_detection is not None :
            #print(self.yolo_detection)
            
            for box in self.yolo_detection.bounding_boxes:
                color = ColorRGBA(0, 1, 1, 0)
                if box.Class == "person":
                    markers_person.markers.append(
                        ImageMarker(
                            header=msg.header,
                            scale=1,
                            type=ImageMarker.POLYGON,
                            outline_color=ColorRGBA(0, 1, 1, 1),
                            points=[
                                Point(box.xmin, box.ymin, 0),
                                Point(box.xmax, box.ymin, 0),
                                Point(box.xmax, box.ymax, 0),
                                Point(box.xmin, box.ymax, 0),

                            ],
                        )
                    )
                else :
                    markers_other.markers.append(
                        ImageMarker(
                            header=msg.header,
                            scale=1,
                            type=ImageMarker.POLYGON,
                            outline_color=ColorRGBA(1, 0, 1, 1),
                            points=[
                                Point(box.xmin, box.ymin, 0),
                                Point(box.xmax, box.ymin, 0),
                                Point(box.xmax, box.ymax, 0),
                                Point(box.xmin, box.ymax, 0),

                            ],
                        )
                    )
            self.pub_person.publish(markers_person)
            self.pub_other.publish(markers_other)


def main():
    Node()


if __name__ == "__main__":
    main()