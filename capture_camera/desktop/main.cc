#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char* argv[]) {
  int camera_number = 0;
  if (argc > 1)
    camera_number = atoi(argv[1]);

  // Get access to the camera.
  cv::VideoCapture camera;
  camera.open(camera_number);
  
  if (!camera.isOpened()) {
    std::cerr << "ERROR: Cound not access the camera or video!" << std::endl;
    exit(1);
  }

  // Try to set the camera resolution.
  camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

  std::cout << "Camera is initiated." << std::endl;

  while (true) {
    // Grab the next camera frame.
    cv::Mat camera_frame;
    camera >> camera_frame; 

    if (camera_frame.empty()) {
      std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
      exit(1);
    }
    imshow("Original Image", camera_frame);

    // // Create a blank output image, that we will draw onto.
    // cv::Mat displayed_frame(camera_frame.size(), CV_8UC3);
    // // Run the cartoonifier filter on the camera frame.
    // cartoonifyImage(camera_frame, displayed_frame);
    // // Display the processed image onto the screen.
    // imshow("Cartoonifier", displayed_frame);
    // IMPORTANT: Wait for at least 20 milliseconds,
    // so that the image can be displayed on the screen!
    // Also checks if a key was pressed in the GUI window.
    // Note that it should be a "char" to support Linux.
    char keypress = cv::waitKey(20); // Need this to see anything!
    if (keypress == 27) { // Escape Key
      // Quit the program!
      break;
    }
  }  // end while
}
