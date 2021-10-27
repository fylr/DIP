#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 结构体有对齐问题
typedef struct bitmap_file_header {
  char bfType[2];
  int bfSize;
  char bf1[2];
  char bf2[2];
  int bfOffBits;
} t_bfh;

typedef struct bitmap_info_header {
  int biSize;
  int biWidth;
  int biHeight;
  short biPlanes;
  short biBitCount;
  int biCompression;
  int biSizeimage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  int biClrUsed;
  int biClrImportant;
} t_bih;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Usage: ./read_bmp <bmp_image_path>" << endl;
    return -1;
  }

  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    cerr << "Open File Failed!" << endl;
    return -1;
  }

  // parser t_bfh
  t_bfh bfh;
  fread(bfh.bfType, sizeof(char), 2, fp);
  fread(&bfh.bfSize, sizeof(char), 12, fp);
  if (bfh.bfType[0] != 'B' || bfh.bfType[1] != 'M') {
    cerr << "File is not bmp image!" << endl;
    return -1;
  }

  // parser t_bih
  t_bih bih;
  fread(&bih, sizeof(t_bih), 1, fp);
  if (bih.biCompression != 0) {
    cerr << "Not support compression in bmp image!" << endl;
    return -1;
  }
  if (bih.biBitCount != 8 && bih.biBitCount != 24) {
    cerr << "Not support biBitCount in bmp image!" << endl;
    return -1;
  }

  // parser bitmap data
  fseek(fp, bfh.bfOffBits, SEEK_SET);
  int data_num = bfh.bfSize - bfh.bfOffBits;
  uchar* b_data = new uchar[data_num];
  fread(b_data, sizeof(uchar), data_num, fp);
  fclose(fp);

  int step = bih.biBitCount / 8, padding;
  Mat img = Mat::zeros(bih.biHeight, bih.biWidth, CV_MAKETYPE(CV_8U, step));
  // padding = (step * bih.biWidth + 3) / 4 * 4 - step * bih.biWidth;
  padding = data_num / bih.biHeight - bih.biWidth * step;
  step = 0;
  for (int i = bih.biHeight - 1; i >= 0; i--) {
    for (int j = 0; j < bih.biWidth; j++) {
      if (bih.biBitCount == 8) {
        img.at<uchar>(i, j) = b_data[step++];
      } else if (bih.biBitCount == 24) {
        img.at<Vec3b>(i, j)[0] = b_data[step++];
        img.at<Vec3b>(i, j)[1] = b_data[step++];
        img.at<Vec3b>(i, j)[2] = b_data[step++];
      }
    }
    step += padding;
  }

  imshow("BMP Image", img);
  waitKey(0);

  return 0;
}