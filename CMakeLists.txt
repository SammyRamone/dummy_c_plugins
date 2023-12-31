cmake_minimum_required(VERSION 3.10.0)

# Extract package name and version from package.xml
find_package(ros_industrial_cmake_boilerplate REQUIRED)
extract_package_metadata(pkg)
project(${pkg_extracted_name} VERSION ${pkg_extracted_version} LANGUAGES CXX)

add_compile_options(-std=c++17)
set(CMAKE_CXX_FLAGS -rdynamic)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


find_package(reach REQUIRED)
find_package(boost_plugin_loader REQUIRED)
find_package(yaml-cpp REQUIRED)

set(ROS2_DEPS
    backward_ros
    tf2_eigen
    interactive_markers
    moveit_core
    moveit_msgs
    moveit_ros_planning_interface
    rclcpp
    sensor_msgs
    visualization_msgs)

find_package(ament_cmake REQUIRED)
foreach(dep ${ROS2_DEPS})
  find_package(${dep} REQUIRED)
endforeach()

add_library(${PROJECT_NAME} SHARED src/no_op.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PUBLIC reach::reach)
ament_target_dependencies(${PROJECT_NAME} PUBLIC ${ROS2_DEPS})

add_library(${PROJECT_NAME}_plugins SHARED src/plugins.cpp)
target_link_libraries(${PROJECT_NAME}_plugins PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_test src/test_lib.cpp)
target_link_libraries(${PROJECT_NAME}_test PUBLIC reach::reach yaml-cpp ${PROJECT_NAME}_plugins)
ament_target_dependencies(${PROJECT_NAME}_test PUBLIC ${ROS2_DEPS})


# Install the libraries
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_plugins EXPORT ${PROJECT_NAME}-targets DESTINATION lib)

# Install the executable(s)
install(TARGETS ${PROJECT_NAME}_test DESTINATION lib/${PROJECT_NAME})

# Install the headers
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

ament_export_targets(${PROJECT_NAME}-targets HAS_LIBRARY_TARGET)
ament_package()
