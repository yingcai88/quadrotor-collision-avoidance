#include "trajectory_selector.h"

void TrajectorySelector::Test() {

  std::cout << "Printing from inside TrajectorySelector " << std::endl;  
  trajectory_library.TestLibrary();
  trajectory_evaluator.TestEvaluator();

}