#include <dummy_c_plugins/no_op.h>

namespace dummy_c_plugins
{

std::vector<std::vector<double>> NoOpIKSolverTest::solveIK(const Eigen::Isometry3d&,
                                                       const std::map<std::string, double>&) const
{
  return { { 0.0 } };
}

reach::IKSolver::ConstPtr NoOpIKSolverTestFactory::create(const YAML::Node&) const
{
  return std::make_shared<NoOpIKSolverTest>();
}

}  // namespace dummy_c_plugins
