#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#define BLACK 0
#define WHITE 255

Mat convolution(Matx<uchar, 100, 100> img, Matx<float, 3, 3> filter) {
    Mat result = Mat(img.rows, img.cols, CV_8U);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++) {
            int px, py;
            float sum = 0;
            for (int k = 0; k < 3; k++) {
                for (int m = 0; m < 3; m++) {
                    py = i + k - 1;
                    px = j + m - 1;
                    if (px < 0 || py < 0 || px >= img.cols || py >= img.rows)
                        continue;
                    sum += ((float)(img(py, px)) * filter(k, m));
                }
            }
            result.at<uchar>(i, j) = (int)sum;
        }
    return result;
}

void matSetting(Matx<uchar,100,100> &m){
    
    for(int i=0;i<100;i++){
        for(int j=0;j<100;j++){
            m(i,j) = BLACK;
        }
    }
    
    for(int i=25;i<75;i++){
        for(int j=25;j<75;j++){
            m(i,j) = WHITE;
        }
    }

}
int main() {
    Matx<uchar, 100, 100> img;
    Matx<float, 3, 3> m(
        1.0 / 9, 1.0 / 9, 1.0 / 9,
        1.0 / 9, 1.0 / 9, 1.0 / 9,
        1.0 / 9, 1.0 / 9, 1.0 / 9
        );
    
    matSetting(img);
    
    Mat m2 = convolution(img, m);

    namedWindow("test", WINDOW_NORMAL);
    imshow("test", img);
    namedWindow("result", WINDOW_NORMAL);
    imshow("result", m2);
    waitKey();
    return 0;
}
