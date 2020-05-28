#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat convolution(Matx<uchar,5,5> img, Matx<float,3,3> filter){
    Mat result = Mat(img.rows,img.cols,CV_8U);
    
    for(int i=0; i<img.rows;i++)
        for(int j=0; j<img.cols;j++){
            int px,py;
            float sum = 0;
            for(int k=0;k<3;k++){
                for(int m=0;m<3;m++){
                    py = i+k - 1;
                    px = j+m - 1;
                    if(px<0||py<0||px>=img.cols||py>=img.rows)
                        continue;
                    sum+= ((float)(img(py,px))*filter(k,m));
                }
            }
            result.at<uchar>(i,j) = (int)sum;
        }
    return result;
}


int main(){
    Matx<uchar,5,5> img(
                        0,0,255,255,255,
                        0,0,255,255,255,
                        0,0,255,255);
    Matx<float, 3, 3> m(
                        1.0 / 9, 1.0 / 9, 1.0 / 9,
                        1.0 / 9, 1.0 / 9, 1.0 / 9,
                        1.0 / 9, 1.0 / 9, 1.0 / 9
                        );
    Mat m2 = convolution(img, m);
    
    namedWindow("test",WINDOW_AUTOSIZE);
    imshow("test",img);
    namedWindow("result",WINDOW_AUTOSIZE);
    imshow("result",m2);
    waitKey();
    return 0;
}

