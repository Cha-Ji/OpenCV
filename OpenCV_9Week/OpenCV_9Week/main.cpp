#include<opencv2/opencv.hpp>
#include<stdio.h>

using namespace std;
using namespace cv;

string imgA_Path = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/code/OpenCV_9Week/OpenCV_9Week/Base.lproj/object.jpg";
string imgB_Path ="/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/code/OpenCV_9Week/OpenCV_9Week/Base.lproj/background.jpg";

Mat imgA,imgB;
RotatedRect drawing;

void onMouse(int event, int mx, int my, int flag, void* param){
    Mat img_temp = imgA.clone();
    Mat black_backg = Mat::zeros(imgA.rows,imgA.cols,CV_8UC3);
    
    int Click = 0;
    float start_x=0, start_y=0,end_x=0,end_y=0;
    Point2f rot_pt; Size rot_sz;
    
    switch (event) {
        case EVENT_LBUTTONDOWN:
            start_x = float(mx); start_y = float(my);
            Click = 1;
            break;
        case EVENT_MOUSEMOVE:
            if(Click == 1){
                end_x = float(mx); end_y = float(my);
                rot_pt = Point2f((start_x + end_x)/2,(start_y + end_y)/2);
                rot_sz = Size(abs(end_x - start_x),abs(end_y - start_y));
                drawing = RotatedRect(rot_pt,rot_sz,0);
                ellipse(img_temp, drawing,  Scalar(0,0,0),2);
                imshow("A",img_temp);
                break;
            }
            break;
        case EVENT_LBUTTONUP:
            end_x = float(mx); end_y = float(my);
            rot_pt = Point2f((start_x + end_x)/2,(start_y + end_y)/2);
            rot_sz = Size(abs(end_x - start_x),abs(end_y - start_y));
            drawing = RotatedRect(rot_pt,rot_sz,0);
            ellipse(black_backg, drawing,  Scalar(255,255,255),-1);

            //bit_or
                //black inside
            bitwise_and(black_backg,imgA,imgA);
                //black <=> white
            bitwise_and(Scalar(255,255,255) - black_backg,imgB,imgB);
            
            //result
            imshow("A",imgA);
            Click = 0;
            
            //merge
            bitwise_or(imgA,imgB,imgB);
            
            imshow("B",imgB);
                        
            break;
       
    }
    
    
}

int main(){
    imgA = imread(imgA_Path);
    imgB = imread(imgB_Path);
    
    //resize B, show A
    resize(imgB,imgB,Size(imgA.cols,imgA.rows));
    
    imshow("A",imgA);
    
    //drawing circle onMouse
    setMouseCallback("A", onMouse,&imgA);

    imshow("B",imgB);
    waitKey(0);
    return 0;
}

