# multi_node_remap_with_params.launch.py

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_E845', 
            parameters=[{'comm_port': "/dev/emlidE845"}, {'comm_speed': 115200}],
            remappings= [('/gpsx', 'topic_emlid_E845')]
        ),
        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_6802', 
            parameters=[{'comm_port': "/dev/emlid6802"}, {'comm_speed': 115200}],
            remappings= [('/gpsx', 'topic_emlid_6802')]
        ),

        Node(
            package='gpsx',
            executable='gps_node',
            name='emlid_C959', 
            parameters=[{'comm_port': "/dev/emlidC959"}, {'comm_speed': 115200}],
            remappings= [('/gpsx', 'topic_emlid_C959')]
        ),
        
    ])
