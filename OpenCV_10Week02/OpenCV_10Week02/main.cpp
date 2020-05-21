//histogram
//각 bin의 픽셀 수

#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String img_path = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/object.jpg";

Mat histogramAdd(Mat m, int nBins);

void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256,200)){
    
    hist_img = Mat(size, CV_8U,Scalar(255));
    float bin_width = (float)hist_img.cols/hist.rows; //각 bin당 너비
    normalize(hist,hist,0,hist_img.rows,NORM_MINMAX);
    
    for(int i=0;i<hist.rows;i++){   //drawing bar graph
        float start_x = i*bin_width;
        float end_x = (i+1)*bin_width;
        
        Point2f pt1(start_x,0);
        Point2f pt2(end_x,hist.at<float>(i));
        
        if(pt2.y>0){
            rectangle(hist_img, pt1, pt2, Scalar(0),-1);
            
        }
    }
    flip(hist_img,hist_img,0);
    
}

int main(){
    Mat image = imread(img_path,IMREAD_GRAYSCALE);
    Mat histMat;
    
    imshow("A",image);
    
    Mat hist = histogramAdd(image, 100);
    
    for(int i=0; i<hist.rows;i++)
        cout<<i<<":"<<hist.at<float>(i,0)<<endl;
    
    
    draw_histo(hist, histMat);
    imshow("hist",histMat);
    waitKey();
    return 0;
}

Mat histogramAdd(Mat m, int nBins){
    Mat hist;
    int channels[] = {0,};  //channel list
    int histSize[] = {nBins}; //bin 개수
    float range[] = {0,256};
    const float* ranges[] = {range};
    calcHist(&m, 1, channels, Mat(), hist, 1, histSize, ranges);
    
    return hist;
}


/*
 
 calc_Histo(const Mat& image, Mat& hist, int bins)
 
 int range_max = 256 ) { …
 
 }
 
 draw_histo(Mat hist, Mat &hist_img. Size size = Size(256j 200))
 
 hist一img = Mat(size, cy^U, Scalar(255)); float bin = (float)hist一img.cols / hist.rows; normalizeChistj hist, hist_img.rows, N0RM_MINMAX);
 
 for (int i = 0; ichist.rows; i++) {
 
 
 float start_x = i * bin; float end_x = (i + 1) * bin; Point2f ptl(start_x, 0); Point2f pt2(end_x, hist.at<float>(i));
 
 if (pt2.y > 0) rectangle(hist_imgj ptl, pt2, Scalar(0), -1);
 
 } flip(hist_imgj hist_img, 0);
 
 
 }
 
 int main()
 
 {
 
 Mat image = imread("../image/pixel_test.jpg"J IMREAD_GRAYSCALE); CV_Assert(!image.empty()); II 예외처리
 
 Mat hist, hist_img; calc_Histo(imagej hist,256); draw_histo(histj hist_img);
 
 imshow("image", image); imshowC'hist^mg'j hist一img); waitKey(); return 0;
 
 
 }
 */
