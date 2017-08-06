#include "cartoon.h"

namespace cartoon {

using namespace cv;

Mat PopulateMaskForSketchMode(const Mat& input);
Mat PopulateMaskForEvilMode(const Mat& input);

void CartoonifyImage(const cv::Mat& input, CartoonMode mode, cv::Mat* output) {
  Mat mask;
  switch (mode) {
    case CartoonMode::SKETCH:
      mask = PopulateMaskForSketchMode(input);
      break;
    case CartoonMode::EVIL:
      mask = PopulateMaskForEvilMode(input);
      break;
  }

  // Reduce the image resolution because the next filter to be used (bilateral
  // filter) is slow.
  const Size size = input.size();
  Size small_size;
  small_size.width = size.width / 2;
  small_size.height = size.height / 2;
  Mat small_img = Mat(small_size, CV_8UC3);
  resize(input, small_img, small_size, 0, 0, INTER_LINEAR);

  // Applies bilateral filter.
  Mat tmp = Mat(small_size, CV_8UC3);
  int repetitions = 7;  // Repetitions for strong cartoon effect.
  for (int i = 0; i < repetitions; i++) {
    int ksize = 9;  // Filter size. Has a large effect on speed.
    double sigma_color = 9;  // Filter color strength.
    double sigma_space = 7; // Spatial strength. Affects speed.
    bilateralFilter(small_img, tmp, ksize, sigma_color, sigma_space);
    bilateralFilter(tmp, small_img, ksize, sigma_color, sigma_space);
  }

  Mat big_img;
  resize(small_img, big_img, size, 0,0, INTER_LINEAR);
  output->setTo(0);
  big_img.copyTo(*output, mask);
}

Mat PopulateMaskForSketchMode(const Mat& input) {
  // Convert from BGR color to Grayscle.
  Mat gray;
  cvtColor(input, gray, CV_BGR2GRAY);

  // Remove the pixel noise with a good Median filter, before we start detecting
  // edges.
  const int MEDIAN_BLUR_FILTER_SIZE = 7;
  medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

  // Detect edge.
  Mat edges;
  const int LAPLACIAN_FILTER_SIZE = 5;
  Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

  // Make edge white. Default is black color.
  Mat mask;
  const int EDGES_THRESHOLD = 80;
  threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);

  return mask;
}

Mat PopulateMaskForEvilMode(const Mat& input) {
  // Convert from BGR color to Grayscle.
  Mat gray;
  cvtColor(input, gray, CV_BGR2GRAY);

  // Remove the pixel noise with a good Median filter, before we start detecting
  // edges.
  const int MEDIAN_BLUR_FILTER_SIZE = 7;
  medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
  
  Mat edges, edges2;
  Scharr(gray, edges, CV_8U, 1, 0);
  Scharr(gray, edges2, CV_8U, 1, 0, -1);
  edges += edges2; // Combine the x & y edges together.

  Mat mask;
  const int EVIL_EDGE_THRESHOLD = 12;
  threshold(edges, mask, EVIL_EDGE_THRESHOLD, 255, THRESH_BINARY_INV);
  medianBlur(mask, mask, 3);

  return mask;
}

}  // namespace cartoon
