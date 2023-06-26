#include <reach/reach_study.h>
#include <reach/utils.h>
#include <reach/plugin_utils.h>
#include <reach/interfaces/ik_solver.h>

#include <boost_plugin_loader/plugin_loader.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <yaml-cpp/yaml.h>

#include <rclcpp/rclcpp.hpp>

namespace reach_ros
{
    namespace utils
    {
        // we need to do this since the node is specified as "extern" in the shared library
        rclcpp::Node::SharedPtr node;
    } // namespace utils
}

int main(int argc, char **argv)
{
    std::cout << "start" << std::endl;

    rclcpp::init(argc, argv);
    rclcpp::executors::MultiThreadedExecutor executor = rclcpp::executors::MultiThreadedExecutor();
    std::thread executor_thread(std::bind(&rclcpp::executors::MultiThreadedExecutor::spin, &executor));
    reach_ros::utils::node = std::make_shared<rclcpp::Node>(
        "reach_study_node",
        rclcpp::NodeOptions().allow_undeclared_parameters(true).automatically_declare_parameters_from_overrides(true));
    executor.add_node(reach_ros::utils::node);

    const YAML::Node &config = YAML::LoadFile("/home/best_mr/ws_reach/src/reach/test/reach_study.yaml");
    const YAML::Node &ik_config = config["ik_solver"];
    // reach::runReachStudy(config, "test_foo", "/tmp", true);

    boost_plugin_loader::PluginLoader loader;
    std::vector<std::string> plugin_libraries;
    boost::split(loader.search_libraries, "reach_plugins", boost::is_any_of(":"), boost::token_compress_on);
    loader.search_libraries_env = "REACH_PLUGINS";
    auto factory = loader.createInstance<reach::IKSolverFactory>("MoveItIKSolver");
    reach::IKSolver::ConstPtr ik_solver = factory->create(ik_config);

    std::cout << "end" << std::endl;
    return 0;
}