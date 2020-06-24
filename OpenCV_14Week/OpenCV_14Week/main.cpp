//14주차
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string imagePath = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영처실(코드)/Git/OpenCV_14Week/Base.lproj/self.jpeg";

Mat readImage(string path);
void getZeroPaddedImage(Mat &src, Mat &dst);
void getLogMag(Mat planes[], Mat& magI, Mat& angI);
void rearRange(Mat magI);

void addNoise(Mat& img, int nNoise);
void addLines(Mat& img, int nLine);
void myFilter(Mat& magI);

int main(){
    Mat I = readImage(imagePath);
    //addNoise(I,20000);
    addLines(I,20);
    
    
    //zero 패딩
    Mat padded;
    getZeroPaddedImage(I,padded);
    
    //dft를 위한 2채널 매트릭스 생성
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F)};
    Mat complexI;
    merge(planes,2,complexI);
    
    //dft 수행하고 채널 분리 planes[0]: 실수부, planes[1]: 허수부
    dft(complexI,complexI);
    
    split(complexI,planes);
    
    //실수부와 허수부 값으로부터 크기와 각도 계산, 크기에 log를 취하여 정규화, 각도도 함께 계산
    //직교 좌표계를 극 좌표계로
    Mat magI, angI;
    getLogMag(planes, magI, angI);
    
    
    //역변환을 위한 최대 최소값 계산
    double min_mag, max_mag, min_ang, max_ang;
    minMaxLoc(magI,&min_mag,&max_mag);
    minMaxLoc(angI,&min_ang,&max_ang);
    
    normalize(magI,magI,0,1,NORM_MINMAX);
    normalize(angI,angI,0,1,NORM_MINMAX); //0~1 사이로 정규화
    
    //각 사분면의 위치를 재조정 1-3, 2-4 교환
    rearRange(magI);
    rearRange(angI);
    
    // 화면 출력
    namedWindow("src");
    namedWindow("zero padding");

    imshow("src",I);
    imshow("zero padding",padded);
    imshow("dft : real",planes[0]);
    imshow("dft : imaginary",planes[1]);
    imshow("spectrum magnitue",magI);
    imshow("phase",angI);
    
    
    //필터링
    myFilter(magI);
    imshow("filtered mag",magI);
    waitKey();
    /*====================================*/
    // IFFT를 위한 처리 시작
    //각 사분면의 위치를 재조정 1-3, 2-4 교환
    rearRange(magI);
    rearRange(angI);
    
    // 비정규화 de-normalize    , 최대 최소값을 미리 계산해 두자
    magI = magI * (max_mag - min_mag) + min_mag;
    angI = angI * (max_ang - min_ang) + min_ang;
    
    // 로그의 역변환 => 지수변환
    exp(magI, magI);
    magI -=1;
    
    //극 좌표계 => 직교 좌표계
    
    polarToCart(magI,angI,planes[0],planes[1],true);
    merge(planes,2,complexI);
    
    
    //Inverse FFT
    dft(complexI,complexI,DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
    split(complexI,planes); //1차원의 행렬로 나누어준다.
    normalize(planes[0], planes[0], 0,1,NORM_MINMAX);
    
    imshow("filtered",planes[0]);
    
    
    waitKey();
    return 0;
}

void getZeroPaddedImage(Mat &src, Mat &dst){
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);    //on the border add zero values
    copyMakeBorder(src, dst, 0, m-src.rows, 0, n-src.cols, BORDER_CONSTANT,Scalar::all(0));
}
Mat readImage(string path){
    Mat I = imread(path,IMREAD_GRAYSCALE);
    if(I.empty()){
        cout<<"Error opening image"<<endl;
        exit(EXIT_FAILURE);
    }
    return I;
}
void getLogMag(Mat planes[], Mat& magI, Mat& angI){
    cartToPolar(planes[0],planes[1],magI, angI, true);
    magI += Scalar::all(1);                             // log0을 계산할 수 없기 때문에
    log(magI,magI);
    
    //스펙트럼이미지의 크기(너비,높이)가 홀수인 경우 제거(잘라냄)
    magI = magI(Rect(0,0,magI.cols & -2, magI.rows & -2));
}
void rearRange(Mat magI){
    //rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    
    Mat q0(magI,Rect(0,0,cx,cy));   //Top-Left Create a ROI per quadrant
    Mat q1(magI,Rect(cx,0,cx,cy));  // T-R
    Mat q2(magI,Rect(0,cy,cx,cy));  // B-L
    Mat q3(magI,Rect(cx,cy,cx,cy)); // B-R
    
    Mat tmp;                        //swap tmp
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}
void addNoise(Mat& img, int nNoise){
    int nGenPoints = 0;
    while(nGenPoints < nNoise){
        int x = (int)(((float)rand() / RAND_MAX) * img.cols);
        int y = (int)(((float)rand() / RAND_MAX) * img.rows);
        
        if(x>=img.cols || y>= img.rows) continue;
        
        //너무 하얗거나 까맣게 노이즈가 생기면 안되니 변수창출
        if((float)rand() / RAND_MAX > 0.5)
            img.at<uchar>(y,x) = 0;
        else img.at<uchar>(y,x) = 255;
        nGenPoints++;
    }
    
}
void addLines(Mat& img, int nLine){
    int width = img.cols/nLine;
    for(int i=0; i<nLine; i++){
        int sx = i * width;
        int ex = (i+1) * width - 1;
        if(ex >= img.cols) ex = img.cols -1;
        Mat tmp(img, Rect(sx,0,ex-sx+1,img.rows));
        
        if( i%2 == 0)
            tmp += Scalar::all(100);
        else
            tmp -= Scalar::all(100);
        
    }
}
void myFilter(Mat& magI){
    Mat f = Mat::zeros(magI.rows, magI.cols,CV_32F);
    int cx = magI.cols /2;
    int cy = magI.rows/2;
    circle(f,Point(cx,cy),200,Scalar(1),-1);
    magI = magI.mul(f);
    
    
}
