#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/LaserScan.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <string>

using namespace std;



darknet_ros_msgs::BoundingBoxes boxes;
sensor_msgs::LaserScan scan;
void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
  scan = *msg;
}
void yoloCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg){
  boxes = *msg;
  /*
  cout<<"Bouding Boxes (header):" << msg->header <<endl;
	cout<<"Bouding Boxes (image_header):" << msg->image_header <<endl;
	cout<<"Bouding Boxes (Class):" << msg->bounding_boxes[0].Class <<endl;
	cout<<"Bouding Boxes (xmin):" << msg->bounding_boxes[0].xmin <<endl;
	cout<<"Bouding Boxes (xmax):" << msg->bounding_boxes[0].xmax <<endl;
	cout<<"Bouding Boxes (ymin):" << msg->bounding_boxes[0].ymin <<endl;
	cout<<"Bouding Boxes (ymax):" << msg->bounding_boxes[0].ymax <<endl;
	cout<<"Bouding Boxes (id):" << msg->bounding_boxes[0].id <<endl;
	cout << "\033[2J\033[1;1H";     // clear terminal*/



}

int main(int argc, char** argv) {
  ros::init(argc, argv, "topic_subscriber");
  ros::NodeHandle nh;
  ros::Subscriber lidarsub = nh.subscribe("/scan", 1, lidarCallback);
  ros::Subscriber yolosub = nh.subscribe("/darknet_ros/bounding_boxes", 1, yoloCallback);
  ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //ros::spin();
  //return 0;

  uint32_t shape = visualization_msgs::Marker::CUBE;

  //!!!!!!!!!!!!1tukaj dalje!!!!!!!!!!!!!


  float Vfov = 1.0466, Hfov = 1.396, Vres = 480, Hres = 640, pi = 3.14159, obj_x[100], obj_y[100];
  int boxes_seq = -1;     //da ne preber dokler
  boxes.header.seq = -1;  //ni novega sporocila/msg
  ros::Rate r(1);
  string person = "person";
  while (ros::ok())
  {

    if (boxes.header.seq != boxes_seq){//ce je nov podatek/nov msg
      for(int i = 0; i < boxes.bounding_boxes.size();i++){
        if(boxes.bounding_boxes[i].Class == "person"){

          float obj_center_x = (boxes.bounding_boxes[i].xmax + boxes.bounding_boxes[i].xmin)/2; // v piklsih
          float obj_center_y = (boxes.bounding_boxes[i].ymax + boxes.bounding_boxes[i].ymin)/2;
          float calculated_angle = -0.1356*obj_center_x-0.0474*obj_center_y+145.586; //deg
          //cout<<"range_max: " << scan.ranges.size() <<endl; // deg*/
          float sestevek = 0; //povprecna razdaja objekta 
          int stevec = 0; // pretvorba v deg

          for(int l = (calculated_angle)*2-1; l <= (calculated_angle)*2+1; l++) //deg podatek
          {
              if(scan.ranges[l] < 10 && scan.ranges[l] > 0) //ce smo v kateri koli meritvi kaj zaznali
              {
                  sestevek +=  sin((l/2)*pi/180) * scan.ranges[l];
                  stevec++;
                  cout<<"l: " << l << " kar je : "<< l/2 << "°"<<endl;
                  cout<<"stevec: " << stevec <<endl;
                  cout<<"z: " << sin((l/2)*pi/180) * scan.ranges[l] <<endl;
                  cout<<" " <<endl;//*/
              }
          }
          obj_x[i] = sestevek/stevec + 0.25; // razdalja od objekta v [m]
          cout<<"obj_x: " << obj_x[i] <<endl;
          obj_y[i] = -obj_x[i]/tan(calculated_angle*pi/180);
          cout<<"obj_y: " << obj_y[i] <<endl;
  ///////////////////////////////////////////

  visualization_msgs::Marker marker;
      // Set the frame ID and timestamp.  See the TF tutorials for information on these.
      marker.header.frame_id = "human_marker";
      marker.header.stamp = ros::Time::now();

      // Set the namespace and id for this marker.  This serves to create a unique ID
      // Any marker sent with the same namespace and id will overwrite the old one
      marker.ns = "human";
      marker.id = 0;

      // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
      marker.type = shape;

      // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
      marker.action = visualization_msgs::Marker::ADD;

      // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
      marker.pose.position.x = obj_x[i];
      marker.pose.position.y = obj_y[i];
      marker.pose.position.z = 1;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w = 1.0;

      // Set the scale of the marker -- 1x1x1 here means 1m on a side
      marker.scale.x = 0.5;
      marker.scale.y = 0.5;
      marker.scale.z = 2;

      // Set the color -- be sure to set alpha to something non-zero!
      marker.color.r = 1.5f;
      marker.color.g = 1.0f;
      marker.color.b = 1.25f;
      marker.color.a = 0.50;

      marker.lifetime = ros::Duration();



      static tf::TransformBroadcaster br;
      tf::Transform transform;
      transform.setOrigin( tf::Vector3(0, 0, 0) );
      tf::Quaternion q;
      q.setRPY(0, 0, 0);
      transform.setRotation(q);
      br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", marker.header.frame_id));

      marker_pub.publish(marker);

      }
      }


    }





    ros::spinOnce();
    r.sleep();
  }
}
  /*
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;






  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "platform_marker_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "cube";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = -0.35;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0.1;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.4;
    marker.scale.y = 0.9;
    marker.scale.z = 0.4;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.25f;
    marker.color.a = 0.50;

    marker.lifetime = ros::Duration();



    static tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(0, 0, 0) );
    tf::Quaternion q;
    q.setRPY(0, 0, 0);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", marker.header.frame_id));

    marker_pub.publish(marker);

    r.sleep();
  }
} */