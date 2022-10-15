#pragma once
#include "eigen/Dense"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>

using namespace Eigen;

namespace SegEigenOp{
    
class EigenOp{
public:
    EigenOp() {};
    ~EigenOp() {};

    static MatrixXf relu(MatrixXf& output);
};

}
