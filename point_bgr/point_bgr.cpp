#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Point lbp;
Mat image;

void onMouse(int event, int x, int y, int flags, void* userdata) {
  if (event == EVENT_LBUTTONDOWN) {
    lbp = Point(x, y);
    cout << "Point: " << lbp << " -> BGR: " << image.at<Vec3b>(lbp) << endl;
  }

  return;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./point_bgr <image_path>" << endl;
    return -1;
  }
  image = imread(argv[1], IMREAD_COLOR);
  if (!image.data) {
    cerr << "the image open fails" << endl;
    return -1;
  }

  namedWindow("imageshow", WINDOW_AUTOSIZE);
  setMouseCallback("imageshow", onMouse, 0);
  imshow("imageshow", image);
  waitKey(0);

  return 0;
}
