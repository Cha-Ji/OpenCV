//trackBarEvent : brightness
//trackBarEvent : contrast (명암)
// m.at , m.ptr

#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string img_path = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_10Week/OpenCV_10Week/Base.lproj/object.jpg";
string winName = "A";
string trackName1 = "brightness";
string trackName2 = "contrast";
Mat img;

void brightValueAdd(Mat m,int v);
int brightValuePtr(Mat m);
int brightValueAt(Mat m);
void onChange_bright(int value, void*);
void onChange_contrast(int value, void*);
void multiplyContrast(Mat m, double value);

int main(){

    //create window/trackbar
    namedWindow(winName);
    createTrackbar(trackName1, winName, 0, 255,onChange_bright);
    createTrackbar(trackName2,winName,0,5,onChange_contrast);
    img = imread(img_path);                     //read img
    
    int brightness =brightValuePtr(img);             //bright to value
     
    imshow("A",img);
    setTrackbarPos(trackName1, winName, brightness);  //set bright
    
    
    waitKey(0);
    
    return 0;
}

// m.at
int brightValueAt(Mat m){
    int sum = 0;
    for(int i=0;i<m.rows;i++)
        for(int j =0;j<m.cols;j++){
            for(int k=0;k<3;k++)    //BGR
                sum+=m.at<Vec3b>(i,j)[k];
            sum/=3;
        }
    
    return sum/(m.rows*m.cols);
}

//m.ptr : Pointer
int brightValuePtr(Mat m){
    int sum = 0;
    uchar* p;
    
    for(int i=0; i<m.rows;i++){
        p = m.ptr<uchar>(i);
        for(int j=0; j<m.cols;j++)
            sum = sum +
            ((p)[j*3] +
             (p)[j*3+1] +
             (p)[j*3+2]
             )/3;
        
    }
    return sum / (m.rows*m.cols);
}

void brightValueAdd(Mat m,int v){
    uchar* p;
    
    for(int i=0; i<m.rows;i++){
        p = m.ptr<uchar>(i);
        for(int j=0; j<m.cols;j++)
            for(int k=0; k<3; k++){
                if( (p)[j*3 + k] + v > 255 )
                    (p)[j*3 + k] = 255;
                else if( (p)[j*3 + k] + v < 0 )
                    (p)[j*3 + k] = 0;
                else
                    (p)[j*3 + k] += v/3;
            }
    }
}

void onChange_bright(int value, void*){
    Mat img_temp = img.clone();
    
    int preValue = brightValueAt(img_temp);
    brightValueAdd(img_temp, value - preValue-128);
    cout<<value-preValue<<"\n";
    imshow(winName,img_temp);
}

void onChange_contrast(int value, void*){
    Mat img_temp = img.clone();
    double v = value*0.5 + 1;
    multiplyContrast(img_temp,v);
    cout<<v<<" "<<value<<"\n";
    imshow(winName,img_temp);
}

void multiplyContrast(Mat m, double value){
    m = m*value;
}

