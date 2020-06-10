#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

string mePath = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영처실(코드)/Git/OpenCV_12Week/OpenCV_12Week/Base.lproj/me.jpeg";

void isSkinArea(Mat ycrcb[],Mat &skinImg){
    //resize
    skinImg = Mat(ycrcb[0].rows,ycrcb[0].cols,CV_8U);
    
    for(int i=0; i<skinImg.rows;i++){
        for(int j=0; j<skinImg.cols;j++){
            if(ycrcb[0].at<uchar>(i,j)>80
               && ycrcb[2].at<uchar>(i,j)<135
               && ycrcb[2].at<uchar>(i,j)>85
               && ycrcb[1].at<uchar>(i,j)>135
               && ycrcb[1].at<uchar>(i,j)<180)
                skinImg.at<uchar>(i,j) = 255;
        }
    }
    
}
int main(){
    Mat BGR_img = imread(mePath);
    Mat HSV_img,hsv[3],
    YCrCb_img,ycrcb[3],
    Skin_img,bgr[3],skin_color;
    
    cvtColor(BGR_img, YCrCb_img,CV_BGR2YCrCb);
    
    split(YCrCb_img,ycrcb);
    split(BGR_img,bgr);
    
    isSkinArea(ycrcb,Skin_img);
    for(int i=0;i<3;i++){   //행렬곱이 아닌 원소끼리의 곱을위해 mul을 사용
        bgr[i] = bgr[i].mul(Skin_img/255);
    }
    merge(bgr,3,skin_color);
    imshow("Skin(before)", Skin_img);
    
    // mophology
    Matx<uchar, 10, 10> mask;
    mask<< 0,1,1,1,1,1,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1,1,1,
            0,1,1,1,1,1,1,1,1,0;
    
    morphologyEx(Skin_img, Skin_img, MORPH_OPEN, mask);
    imshow("Skin (after)",Skin_img);
    
    for(int i=0;i<3;i++){   //행렬곱이 아닌 원소끼리의 곱을위해 mul을 사용
        bgr[i] = bgr[i].mul(Skin_img/255);
    }
    merge(bgr,3,skin_color);
    
    
    imshow("base",BGR_img);
    
    imshow("skinColor",skin_color);
    waitKey();
    return 0;
}
