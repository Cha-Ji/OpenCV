#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace cv;
using namespace std;

string imgPath = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영처실(코드)/Git/OpenCV_Project/OpenCV_Project/self.jpeg";
string imgTitle = "ChaJi_PhotoShop";

bool grayVer = true;   //grayscale : True
Mat I,menu,result;

//===========menu button========
Rect rMenu1(20, 20, 300, 50);
Rect rMenu2(20, 90, 300, 50);
Rect rMenu3(20, 160, 300, 50);
Rect rMenu4(20, 230, 300, 50);
Rect rMenu6(20, 300, 300, 50);
Rect rMenu7(20, 370, 300, 50);

Rect rMenu5(20, 800, 300, 50);  //back button
//===============================

//==========================function list===========
void init();
void concatImg(Mat& src);   //imshow menu + src
void createMenu(Mat& menu,Rect rMenu,string title);
Mat readImage();
void addNoise(Mat& img, int nNoise);
void addLines(Mat& img, int nLine);
void onMouse(int event, int mx, int my, int flag, void* param); //menu click event
bool bInsideRect(int x, int y, Rect rect);
void isSkinArea(Mat ycrcb[],Mat &skinImg);
void detectSkin(Mat& BGR_img);
void ROI();
void blur();
int brightValue(Mat m);
void brightValueAdd(Mat m,int v);
void onChange_bright(int value, void*);
void multiplyContrast(Mat m, double value);
void onChange_contrast(int value, void*);
//==================================================


//============fourier===============================
void getZeroPaddedImage(Mat &src, Mat &dst);
void rearRange(Mat magI);
void getLogMag(Mat planes[], Mat& magI, Mat& angI);
void myFilter(Mat& magI);
void fourier();
//==================================================

int main(){
    init();
    return 0;
}

//mouse event handler
void onMouse(int event, int mx, int my, int flag, void* param) {
    switch (event) {
        case EVENT_LBUTTONDOWN:
            //initialize
            if (bInsideRect(mx, my, rMenu1)) {
                I = readImage();
                concatImg(I);
            }
            //addNoise
            else if (bInsideRect(mx, my, rMenu2)) {
                addNoise(I, 1000);
                concatImg(I);
            }
            //addLines
            else if (bInsideRect(mx, my, rMenu3)){
                addLines(I,20);
                concatImg(I);
            }
            //gray ? myFilter : detectSkin
            else if (bInsideRect(mx, my, rMenu4)){
                if(grayVer)fourier();
                else {
                    detectSkin(I);
                    concatImg(I);
                }
            }
            //back to start
            else if (bInsideRect(mx, my, rMenu5)){
                destroyWindow(imgTitle);
                init();
            }
            //ROI
            else if (bInsideRect(mx, my, rMenu6)){
                ROI();
            }
            //blur
            else if (bInsideRect(mx, my, rMenu7)){
                if(grayVer)blur();
            }
            break;
    }
}

Mat readImage() {
    if(grayVer)
        I = imread(imgPath,IMREAD_GRAYSCALE);
    else I = imread(imgPath);
    if (I.empty()) {
        cout << "Error opening image" << endl;
        exit(EXIT_FAILURE);
    }
    resize(I, I, Size(900,900));
    return I;
}

//point inside rect : 1 outside rect : 0
bool bInsideRect(int x, int y, Rect rect) {
    return (x >= rect.x && x < rect.x+ rect.width &&
            y >= rect.y && y < rect.y + rect.height);
}

void addNoise(Mat& img, int nNoise) {
    int nGenPoints = 0;
    while (nGenPoints < nNoise) {
        int x = (int)(((float)rand() / RAND_MAX) * img.cols);
        int y = (int)(((float)rand() / RAND_MAX) * img.rows);
        if (x >= img.cols || y >= img.rows)
            continue;
        if ((float)rand() / RAND_MAX > 0.5)
            img.at<uchar>(y, x) = 0;
        else
            img.at<uchar>(y, x) = 255;
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

//create Menu button
void createMenu(Mat& menu,Rect rMenu,string title){
    rectangle(menu, rMenu, Scalar(255), 1);
    putText(menu, title,Point(rMenu.x+15, rMenu.y+30),FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255), 2);
}


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
void detectSkin(Mat& BGR_img){
    Mat YCrCb_img,ycrcb[3],
    Skin_img,bgr[3],skin_color;
    
    cvtColor(BGR_img, YCrCb_img,CV_BGR2YCrCb);
    
    split(YCrCb_img,ycrcb);
    split(BGR_img,bgr);
    
    isSkinArea(ycrcb,Skin_img);
    
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
    
    
    for(int i=0;i<3;i++){   //not mul Matrix
        bgr[i] = bgr[i].mul(Skin_img/255);
    }
    merge(bgr,3,skin_color);
    BGR_img = skin_color.clone();
    
    
}


//init program
void init(){
    //=============================================
    // start image
    
    Mat init = imread(imgPath,IMREAD_ANYCOLOR);
    resize(init,init,Size(1300,900));
    
    namedWindow("init");
    putText(init, "press '1' key : Gray",Point(730, 200),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0), 2);
    putText(init, "press other key : Color",Point(730, 400),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0), 2);
    putText(init, "press 'q' key : quit",Point(730, 300),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0), 2);
    
    imshow("init",init);
    char k = waitKey();
    destroyWindow("init");
    grayVer = (k=='1');
    if(k=='q')exit(1);

    //================================================
    
    I = readImage();
    
    //menu image
    menu = Mat::zeros(I.rows, 400,grayVer ? IMREAD_GRAYSCALE : CV_8UC3); //CV_8UC3 for concat
    
    //crate menu button
    createMenu(menu, rMenu1, "Initialize");
    createMenu(menu, rMenu2, "Add Noise");
    createMenu(menu, rMenu3, "Add Lines");
    createMenu(menu, rMenu4, grayVer ? "My Filter" : "Detect Skin");
    createMenu(menu, rMenu5, "Back");
    createMenu(menu, rMenu6, "ROI");
    if(grayVer)createMenu(menu, rMenu7, "Blur");
    
    //create menu & set mouseEvent
    namedWindow(imgTitle);
    setMouseCallback(imgTitle, onMouse, &menu);
    
    //create trackbar
    createTrackbar("brightness", imgTitle, 0, 255,onChange_bright);
    createTrackbar("contrast",imgTitle,0,5,onChange_contrast);
    int brightness =brightValue(I);                                 //bright to value
    setTrackbarPos("brightness", imgTitle, brightness);             //set bright
    
    //show image
    concatImg(I);
    waitKey();
    
}

//concat Img + menu
void concatImg(Mat& src){
    hconcat(menu, src, result);
    
    namedWindow(imgTitle,WINDOW_AUTOSIZE);
    imshow(imgTitle,result);
}

//======fourier filter============================
void getZeroPaddedImage(Mat &src, Mat &dst){
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);    //on the border add zero values
    copyMakeBorder(src, dst, 0, m-src.rows, 0, n-src.cols, BORDER_CONSTANT,Scalar::all(0));
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
void getLogMag(Mat planes[], Mat& magI, Mat& angI){
    cartToPolar(planes[0],planes[1],magI, angI, true);
    magI += Scalar::all(1);                  // log0을 계산할 수 없기 때문에
    log(magI,magI);
    
    //스펙트럼이미지의 크기(너비,높이)가 홀수인 경우 제거(잘라냄)
    magI = magI(Rect(0,0,magI.cols & -2, magI.rows & -2));
}
void fourier(){
    Mat padded;
    getZeroPaddedImage(I, padded);
    
    //for dft
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
    Mat complexI;
    
    merge(planes, 2, complexI);
    
    //planes[0]: real, planes[1]: imaginary
    dft(complexI, complexI);
    split(complexI, planes);
    
   
    Mat magI, angI;
    getLogMag(planes, magI, angI);
    
    
    //min&max values for de-normalize
    double min_mag, max_mag, min_ang, max_ang;
    minMaxLoc(magI, &min_mag, &max_mag);
    minMaxLoc(angI, &min_ang, &max_ang);
    
    normalize(magI, magI, 0, 1, NORM_MINMAX);
    normalize(angI, angI, 0, 1, NORM_MINMAX); //0~1
    
    //1<->3, 2<->4
    rearRange(magI);
    rearRange(angI);
    
    myFilter(magI);
    
    // ============IFFT==================
    rearRange(magI);
    rearRange(angI);
    
    // de-normalize
    magI = magI * (max_mag - min_mag) + min_mag;
    angI = angI * (max_ang - min_ang) + min_ang;
    
    // log => exp
    exp(magI, magI);
    magI -= 1;
    
    //Polar to Cart
    polarToCart(magI, angI, planes[0], planes[1], true);
    merge(planes, 2, complexI);
    
    Mat test;
    //Inverse FFT
    dft(complexI, complexI, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
    split(complexI, planes);
    normalize(planes[0], planes[0], 0, 1, NORM_MINMAX);
    
    
    //show image
    hconcat(Mat_<float>(menu), planes[0], result);
    
    namedWindow(imgTitle,WINDOW_AUTOSIZE);
    imshow(imgTitle,result);
    I = readImage();
    
}
void myFilter(Mat& magI) {
    Mat f = Mat::ones(magI.rows, magI.cols, CV_32F);
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    circle(f, Point(cx+11, cy), 3, Scalar(0), -1);
    circle(f, Point(cx + 31, cy), 3, Scalar(0), -1);
    circle(f, Point(cx + 51, cy), 3, Scalar(0), -1);
    magI = magI.mul(f);
    
    
}

//================================================

//==brightness trackbar==============
void onChange_bright(int value, void*){
    Mat img_temp = I.clone();
    
    int preValue = brightValue(img_temp);
    brightValueAdd(img_temp, value - preValue-50);
    concatImg(img_temp);
}
int brightValue(Mat m){
    int sum=0;
    if(!grayVer){
        for(int i=0;i<m.rows;i++)
            for(int j =0;j<m.cols;j++){
                for(int k=0;k<3;k++)    //BGR
                    sum+=m.at<Vec3b>(i,j)[k];
                sum/=3;
            }
    }
    else{
        for(int i=0;i<m.rows;i++)
            for(int j =0;j<m.cols;j++){
                for(int k=0;k<2;k++)    //BGR
                    sum+=m.at<Vec2b>(i,j)[k];
                sum/=2;
            }
    }
    return sum/(m.rows*m.cols);
    
}
void brightValueAdd(Mat m,int v){
    uchar* p;
    if(!grayVer)
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
    else
        for(int i=0; i<m.rows;i++){
            p = m.ptr<uchar>(i);
            for(int j=0; j<m.cols;j++)
                for(int k=0; k<2; k++){
                    if( (p)[j*2 + k] + v > 255 )
                        (p)[j*2 + k] = 255;
                    else if( (p)[j*2 + k] + v < 0 )
                        (p)[j*2 + k] = 0;
                    else
                        (p)[j*2 + k] += v/2;
                }
        }
}
//===================================

//contrast trackbar
void onChange_contrast(int value, void*){
    Mat img_temp = I.clone();
    double v = value*0.5 + 0.5;
    img_temp*=v;
    concatImg(img_temp);
}

void ROI(){
    Range r_row(300, 600), r_col(300, 800);
    Mat roi = I(r_row, r_col);
    roi.adjustROI(-0,100,-0,0);
    if(grayVer)roi.adjustROI(0, 0, 0, -300);
    
    for (int i = 0; i < roi.rows; i++) {
        for (int j = 0; j< roi.cols; j++) {
            if (roi.at<uchar>(i, j) >= 205)
                roi.at<uchar>(i, j) = 255;
            else
                roi.at<uchar>(i, j) +=50;
        }
    }
    concatImg(I);
}

void blur() {
    Mat mask = (Mat_<float>(3, 3) << 1.0 / 9, 1.0 / 9, 1.0 / 9,
                1.0 / 9, 1.0 / 9, 1.0 / 9,
                1.0 / 9, 1.0 / 9, 1.0 / 9
                );
    
    for (int i = 0; i < I.rows; i++)
        for (int j = 0; j < I.cols; j++) {
            int px, py;
            float sum = 0;
            for (int k = 0; k < mask.rows; k++) {
                for (int m = 0; m < mask.cols; m++) {
                    py = i + k - 1;
                    px = j + m - 1;
                    if (px < 0 || py < 0 || px >= I.cols || py >= I.rows)
                        continue;
                    sum += float(I.at<uchar>(py, px)) * mask.at<float>(k, m);
                }
            }
            I.at<uchar>(i, j) = (int)sum;
        }
    
    concatImg(I);
}

