#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "gpsx/msg/gpsx.hpp"
#include "gpsx/msg/xyz.hpp"
#include <chrono>

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
      "topic_emlid_backpack", 10, std::bind(&Gps_reader::topic_backpack_callback, this, _1));

      subscription_3 = this->create_subscription<gpsx::msg::Gpsx>(
      "topic_emlid_6802", 10, std::bind(&Gps_reader::topic_6802_callback, this, _1));

      publisher_ = this->create_publisher<gpsx::msg::Xyz>("pos_conversion", 10);

     timer_ = this->create_wall_timer(
       50ms, std::bind(&Gps_reader::timer_callback, this));
    }

  private:
    void topic_E845_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_E845_msg = msg;
    }

    void topic_backpack_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_backpack_msg = msg;
    }

    void topic_6802_callback(const gpsx::msg::Gpsx & msg)
    {
      emlid_6802_msg = msg;
    }

    void timer_callback()
    {
      auto message = gpsx::msg::Xyz();
      message.x = 1.4;
      message.y = 1.4;
      message.z = 1.4;
      RCLCPP_INFO(this->get_logger(), " valeur_sat_E845: '%i'", emlid_E845_msg.satellites);
      RCLCPP_INFO(this->get_logger(), "valeur_sat_6802: '%i'", emlid_6802_msg.satellites);
      RCLCPP_INFO(this->get_logger(), "valeur_sat_backpack: '%i'", emlid_backpack_msg.satellites);
      publisher_->publish(message);
    }

    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_1;
    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_2;
    rclcpp::Subscription<gpsx::msg::Gpsx>::SharedPtr subscription_3;
    rclcpp::TimerBase::SharedPtr timer_;
    gpsx::msg::Gpsx emlid_E845_msg;
    gpsx::msg::Gpsx emlid_6802_msg;
    gpsx::msg::Gpsx emlid_backpack_msg;
    rclcpp::Publisher<gpsx::msg::Xyz>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Gps_reader>());
  rclcpp::shutdown();
  return 0;
}