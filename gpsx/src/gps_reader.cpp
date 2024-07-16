#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "gpsx/msg/gpsx.hpp"
#include <chrono>
#include <iostream>
//below lib for coord conversion

using namespace std::chrono_literals;
using std::placeholders::_1;

class Gps_reader : public rclcpp::Node
{
  public:
    Gps_reader()
    : Node("Gps_reader")
    {

      subscription_1 = this->create_subscription<gpsx::msg::Gpsx>(
      "topic_emlid_E845", 10, std::bind(&Gps_reader::topic_E845_callback, this, _1));

      subscription_2 = this->create_subscription<gpsx::msg::Gpsx>(
      "topic_emlid_C959", 10, std::bind(&Gps_reader::topic_C959_callback, this, _1));

      subscription_3 = this->create_subscription<gpsx::msg::Gpsx>(
      "topic_emlid_6802", 10, std::bind(&Gps_reader::topic_6802_callback, this, _1));

      publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("gps_value", 10);

     timer_ = this->create_wall_timer(
       50ms, std::bind(&Gps_reader::timer_callback, this));
    }

  private:
    void topic_E845_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_E845_msg = msg;
    }

    void topic_C959_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_C959_msg = msg;
    }

    void topic_6802_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_6802_msg = msg;
    }

    void timer_callback()
    {
     
      auto message = std_msgs::msg::Float32MultiArray();

      message.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());
      message.layout.dim[0].label = "emlid_values";
      message.layout.dim[0].size = 9;  
      message.layout.dim[0].stride = 9;
      //pas propre mais marche 
      message.data = {static_cast<float> (emlid_E845_msg.longitude), static_cast<float> (emlid_E845_msg.latitude),static_cast<float> (emlid_E845_msg.altitude),
                      static_cast<float> (emlid_C959_msg.longitude), static_cast<float> (emlid_C959_msg.latitude),static_cast<float> (emlid_C959_msg.altitude), 
                      static_cast<float> (emlid_6802_msg.longitude), static_cast<float> (emlid_6802_msg.latitude),static_cast<float> (emlid_6802_msg.altitude)};

      publisher_->publish(message);
    }

    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_1;
    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_2;
    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_3;
    rclcpp::TimerBase::SharedPtr timer_;
    gpsx::msg::Gpsx emlid_E845_msg;
    gpsx::msg::Gpsx emlid_6802_msg;
    gpsx::msg::Gpsx emlid_C959_msg;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Gps_reader>());
  rclcpp::shutdown();
  return 0;
}