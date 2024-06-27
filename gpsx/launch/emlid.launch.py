# multi_node_remap_with_params.launch.py

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_E845', 
            parameters=[{'comm_port': "/dev/ttyACM0"}, {'comm_speed': 9600}],
            remappings= [('/gpsx', 'topic_emlid_E845')]
        ),
        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_6802', 
            parameters=[{'comm_port': "/dev/ttyACM1"}, {'comm_speed': 9600}],
            remappings= [('/gpsx', 'topic_emlid_6802')]
        ),

        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_backpack', 
            parameters=[{'comm_port': "/dev/ttyACM2"}, {'comm_speed': 9600}],
            remappings= [('/gpsx', 'topic_emlid_backpack')]
        ),
        
        Node(
            package='gpsx',
            executable='gps_reader',
        )
    ])
