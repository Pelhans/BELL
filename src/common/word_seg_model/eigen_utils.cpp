#include <map>
#include "eigen_utils.h"

using namespace Eigen;
using namespace SegEigenOp;

MatrixXf EigenOp::relu(MatrixXf& output) {
    for (int i=0; i<output.rows(); ++i) {
        for (int j=0; j<output.cols(); ++j) {
            if (output(i, j) < 0) {
                output(i, j) = 0;
            }
        }
    }
}