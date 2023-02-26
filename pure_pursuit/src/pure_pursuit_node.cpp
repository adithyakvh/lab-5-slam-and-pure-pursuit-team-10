#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "ackermann_msgs/msg/ackermann_drive_stamped.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "visualization_msgs/msg/marker_array.hpp"
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>

#include <chrono>
#include <memory>
#include <functional>


//libraries for reading csv
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


/// CHECK: include needed ROS msg type headers and libraries

using namespace std;

using std::placeholders::_1;

using namespace std::chrono_literals;

class PurePursuit : public rclcpp::Node
{
    // Implement PurePursuit
    // This is just a template, you are free to implement your own node!

private:

    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr pub_marker;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_pose;
    std::vector<std::vector<float>> positions;

    rclcpp::Node::SharedPtr node;

public:
    PurePursuit() : Node("pure_pursuit_node")
    {
        // TODO: create ROS subscribers and publishers

        pub_marker = this->create_publisher<visualization_msgs::msg::MarkerArray>("marker_array", 10);
        sub_pose = this->create_subscription<nav_msgs::msg::Odometry>("/ego_racecar/odom", 100 , std::bind(&PurePursuit::pose_callback, this, _1));

        //read csv and convert to 2d float array
        std::ifstream file("interpolated_waypoints.csv");
        std::vector<std::vector<float>> positions; // 2D array of float x y positions
        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::vector<float> pos;

            std::string token;
            while (std::getline(ss, token, ','))
            {
                pos.push_back(std::stof(token));
            }

            // Add the x y position values to the 2D array
            positions.push_back({pos[0], pos[1]});
        }

        // Print out the x y positions
        // for (auto const& pos : positions)
        // {
        //     std::cout << "(" << pos[0] << ", " << pos[1] << ")\n";
        // }

        





    }

    // void pose_callback(const std::shared_ptr<const geometry_msgs::msg::PoseStamped>& pose_msg)  
    // void pose_callback(const geometry_msgs::msg::PoseStamped::ConstSharedPtr pose_msg) //stub code had &pose_msg, the & caused build errors. also said ConstPtr instead of ConstSharedPtr, which also made errors
    void pose_callback(const nav_msgs::msg::Odometry::ConstSharedPtr pose_msg) //stub code had &pose_msg, the & caused build errors. also said ConstPtr instead of ConstSharedPtr, which also made errors

    {


        double car_x = pose_msg->pose.pose.position.x;
        double car_y = pose_msg->pose.pose.position.y;
        double car_z = pose_msg->pose.pose.position.z;



        // MARKERS
        // visualization_msgs::msg::Marker marker1;
        // marker1.header.frame_id = "map";
        // // marker1.header.stamp = ros::Time::now();
        // marker1.type = visualization_msgs::msg::Marker::SPHERE;
        // marker1.action = visualization_msgs::msg::Marker::MODIFY;
        // marker1.pose.position.x = car_x;
        // marker1.pose.position.y = car_y;
        // marker1.pose.position.z = car_z + 1.0;
        // marker1.pose.orientation.x = 0.0;
        // marker1.pose.orientation.y = 0.0;
        // marker1.pose.orientation.z = 0.0;
        // marker1.pose.orientation.w = 1.0;
        // marker1.scale.x = 1.0;
        // marker1.scale.y = 1.0;
        // marker1.scale.z = 1.0;
        // marker1.color.a = 1.0;
        // marker1.color.r = 1.0;
        // marker1.color.g = 0.0;
        // marker1.color.b = 0.0;

        // pub_marker->publish(marker1); 


        RCLCPP_INFO(this->get_logger(), "before");

        //create the top level marker array
        visualization_msgs::msg::MarkerArray marker_array;
        marker_array.markers.resize(2);

        // Sphere Marker
        marker_array.markers[0].header.frame_id = "map";
        // marker_array.markers[0].header.stamp = node->now();
        marker_array.markers[0].id = 0;
        marker_array.markers[0].type = visualization_msgs::msg::Marker::SPHERE;
        marker_array.markers[0].action = visualization_msgs::msg::Marker::MODIFY;
        marker_array.markers[0].pose.position.x = 1.0;
        marker_array.markers[0].pose.position.y = 2.0;
        marker_array.markers[0].pose.position.z = 1.0;
        marker_array.markers[0].scale.x = 1.0;
        marker_array.markers[0].scale.y = 1.0;
        marker_array.markers[0].scale.z = 1.0;
        marker_array.markers[0].color.r = 1.0;
        marker_array.markers[0].color.g = 0.0;
        marker_array.markers[0].color.b = 0.0;
        marker_array.markers[0].color.a = 1.0;

        // CubeList Marker
        marker_array.markers[1].header.frame_id = "map";
        // marker_array.markers[1].header.stamp = node->now();
        marker_array.markers[1].id = 1;
        marker_array.markers[1].type = visualization_msgs::msg::Marker::CUBE_LIST;
        marker_array.markers[1].action = visualization_msgs::msg::Marker::ADD;
        marker_array.markers[1].scale.x = 1.0;
        marker_array.markers[1].scale.y = 1.0;
        marker_array.markers[1].scale.z = 1.0;
        marker_array.markers[1].color.r = 0.0;
        marker_array.markers[1].color.g = 1.0;
        marker_array.markers[1].color.b = 0.0;
        marker_array.markers[1].color.a = 1.0;


     
        geometry_msgs::msg::Point point;
        point.x = 2.0;
        point.y = 1.0;
        point.z = 0.0;

        marker_array.markers[1].points.push_back(point);

        point.x = 2.0;
        point.y = 2.0;
        point.z = 0.0;

        marker_array.markers[1].points.push_back(point);

        point.x = 2.0;
        point.y = 3.0;
        point.z = 0.0;

        marker_array.markers[1].points.push_back(point);

        // std::shared_ptr<visualization_msgs::msg::MarkerArray> marker_array_ptr = std::make_shared<visualization_msgs::msg::MarkerArray>(marker_array);


        pub_marker->publish(marker_array);

        RCLCPP_INFO(this->get_logger(), "after");



    



      

        // RCLCPP_DEBUG(this->get_logger(), "Debug message");




        // TODO: find the current waypoint to track using methods mentioned in lecture

        // TODO: transform goal point to vehicle frame of reference

        // TODO: calculate curvature/steering angle

        // TODO: publish drive message, don't forget to limit the steering angle.
    }

    ~PurePursuit() {} // destructor, which is called when the object is destroyed,
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    // rclcpp::get_logger("rclcpp").set_level(rclcpp::LoggingSeverity::Debug);
    rclcpp::spin(std::make_shared<PurePursuit>());
    rclcpp::shutdown();
    return 0;
}