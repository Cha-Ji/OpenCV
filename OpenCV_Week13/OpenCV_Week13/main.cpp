#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


Mat src;    //원본 이미지
Mat disply_image;
string szTitle_src = "src";
Point2f p[4];
int nPoint = 0;
string mePath = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영처실(코드)/Git/OpenCV_Week13/Base.lproj/self.jpeg";

Mat getPerspectiveMatrix(){
    Mat m;
    if(nPoint==4){
        int L1 = sqrt(pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y,2));
        int L2 = sqrt(pow(p[2].x - p[1].x, 2) + pow(p[2].y - p[1].y,2));
        Point2f p_new[4];
        p_new[1] = p[1];
        
        p_new[0].x = p[1].x;        p_new[0].y =p[1].y - L1;
        p_new[2].x = p[1].x + L2;   p_new[2].y =p[1].y;
        p_new[3].x = p[1].x + L2;   p_new[3].y =p[1].y - L1;
        m = getPerspectiveTransform(p, p_new);
     
    }
    return m;
}

void onMouse(int event, int x, int y, int flag, void*){
    if(event == EVENT_LBUTTONDOWN){
        p[nPoint].x = x;
        p[nPoint].y = y;
        nPoint = (nPoint+1) > 4 ? 4 : (nPoint+1);
        if(nPoint == 4){
            Mat m = getPerspectiveMatrix();
            Mat dst;
            warpPerspective(src, dst, m, Size(src.cols,src.rows));
            imshow("dst",dst);
        }
    }
}

int main(){
    
    src = imread(mePath);
    namedWindow(szTitle_src, WINDOW_NORMAL);
    namedWindow("dst",WINDOW_NORMAL);
    setMouseCallback("src", onMouse, 0);    //window생성 후 마우스 콜백을 넣을 것
    
    imshow(szTitle_src,src);
    waitKey();
    return 0;
}

