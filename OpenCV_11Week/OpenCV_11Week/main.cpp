#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

string pathA = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_11Week/OpenCV_11Week/A.jpg";
string pathB = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_11Week/OpenCV_11Week/B.jpg";
string pathC = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_11Week/OpenCV_11Week/Base.lproj/woman.jpg";
string winNameA = "A";
string winNameB = "B";

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
    Mat BGR_img = imread(pathC);
    Mat HSV_img,hsv[3],YCrCb_img,ycrcb[3],
        Skin_img,bgr[3],skin_color;
    
    cvtColor(BGR_img, HSV_img, CV_BGR2HSV);
    cvtColor(BGR_img, YCrCb_img,CV_BGR2YCrCb);
    
    split(YCrCb_img,ycrcb);
    split(HSV_img,hsv);
    split(BGR_img,bgr);
    
    isSkinArea(ycrcb,Skin_img);
    
    for(int i=0;i<3;i++){   //행렬곱이 아닌 원소끼리의 곱을위해 mul을 사용
        bgr[i] = bgr[i].mul(Skin_img/255);
    }
    merge(bgr,3,skin_color);
    
    
    imshow("base",BGR_img);
    imshow("Skin",Skin_img);
    imshow("skinColor",skin_color);
    waitKey();
    return 0;
}

