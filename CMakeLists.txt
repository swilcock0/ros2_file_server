cmake_minimum_required(VERSION 3.8)
project(ros2_file_server)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)

ament_auto_find_build_dependencies()

set(msg_files
  "srv/GetBinaryFile.srv"
  "srv/GetFile.srv"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES std_msgs
)


add_executable(file_server src/file_server.cpp)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp
)

target_link_libraries(file_server "${cpp_typesupport_target}")

ament_target_dependencies(file_server
  rclcpp
  std_msgs
)


install(TARGETS file_server
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_dependencies(rosidl_default_runtime std_msgs)

ament_package()
