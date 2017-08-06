#include <opencv2/opencv.hpp>

namespace cartoon {

enum class CartoonMode {
  SKETCH,
  EVIL,
};

void CartoonifyImage(const cv::Mat& input, CartoonMode mode, cv::Mat* output);

}  // namespace cartoon
