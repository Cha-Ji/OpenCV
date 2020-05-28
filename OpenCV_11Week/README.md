# 화소 처리와 영역 처리

[11_컬러공간_합성곱_v4.pdf](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/849ddbf3-114e-4fe7-ad18-d593b37707ce/11___v4.pdf)

- 1교시

    > 컬러공간

    1. RGB ⇒ HSV 공간으로 변환하기

    ```cpp
    #include<opencv2/opencv.hpp>
    #include<opencv2/imgproc/types_c.h> //CV_BGR2HSV가 내장됨

    using namespace std;
    using namespace cv;

    string pathA = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_11Week/OpenCV_11Week/A.jpg";
    string pathB = "/Users/cha-ji/iCloud Drive(아카이브)/3학년 1학기/영상처리및실습(장원두)/Git/OpenCV_11Week/OpenCV_11Week/B.jpg";
    string winNameA = "A";
    string winNameB = "B";

    int main(){
        Mat BGR_img = imread(pathA);
        Mat HSV_img,hsv[3];
        cvtColor(BGR_img, HSV_img, CV_BGR2HSV);
        
        split(HSV_img,hsv);
        
        imshow("BGR",BGR_img);
        imshow("H",hsv[0]);
        imshow("S",hsv[1]);
        imshow("V",hsv[2]);
        
        
        
        waitKey();
        return 0;
    }
    ```

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/6983e919-a173-4466-94ad-23706387884a/_2020-05-28__7.48.48.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/6983e919-a173-4466-94ad-23706387884a/_2020-05-28__7.48.48.png)

- 2교시

    > YCbCr공간에서 피부검출

    ```cpp
    cvtColor(BGR_img, HSV_img, CV_BGR2YCrCb);
    ```

    피부 공식

    - Y >80
    - 85<Cb<135
    - 135<Cr<180

    ```cpp
    for(int i=0; i<skinImg.rows;i++){
            for(int j=0; j<skinImg.cols;j++){
                if(ycrcb[0].at<uchar>(i,j)<80
                   && ycrcb[2].at<uchar>(i,j)<135
                   && ycrcb[2].at<uchar>(i,j)>85
                   && ycrcb[1].at<uchar>(i,j)>135
                   && ycrcb[1].at<uchar>(i,j)<180)
                    skinImg.at<uchar>(i,j) = 255;
            }
        }
    ```

    ```cpp
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
    ```

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/8f4e1cd3-020d-4ed2-a3f6-29cceb91e155/_2020-05-28__8.34.20.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/8f4e1cd3-020d-4ed2-a3f6-29cceb91e155/_2020-05-28__8.34.20.png)

- 3교시

    > 영역 처리

    - 이미지 개선 : IMAGE ENHANCEMENT

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/c56700ab-1833-43c4-bf2b-de2e9a6023c4/_2020-05-28__8.56.17.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/c56700ab-1833-43c4-bf2b-de2e9a6023c4/_2020-05-28__8.56.17.png)

    - 여러 이미지를 합치는 기술 : IMAGE STITCHING

        ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/b5205a6c-111e-408c-b21e-ff0f2f07b949/_2020-05-28__8.57.06.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/b5205a6c-111e-408c-b21e-ff0f2f07b949/_2020-05-28__8.57.06.png)

        - DENOISING

            ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/4b55a1e4-1708-41cc-9b58-ade31c6f5b7f/_2020-05-28__8.59.14.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/4b55a1e4-1708-41cc-9b58-ade31c6f5b7f/_2020-05-28__8.59.14.png)

        - IMAGE RESTORATION

        → 이미지 복구

        - 이미지 검색 : IMAGE RETRIEVAL

            ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/122e7e21-ab92-485d-a23c-52ba4eb510a0/_2020-05-28__9.01.01.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/122e7e21-ab92-485d-a23c-52ba4eb510a0/_2020-05-28__9.01.01.png)

- 4교시
    - convolution

    → 겹치는 공간끼리 곱한 것의 합

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/968ed6b8-fe5a-435e-b7e5-8094dae69ad8/_2020-05-28__9.18.30.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/968ed6b8-fe5a-435e-b7e5-8094dae69ad8/_2020-05-28__9.18.30.png)

    ⇒ 블러처리?

    ```cpp
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
    ```

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/62686ae7-6099-4569-aee1-e2a046b87509/_2020-05-28__11.30.39.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/62686ae7-6099-4569-aee1-e2a046b87509/_2020-05-28__11.30.39.png)

- 오류

→ OpenCV 내에서 types_c의 충돌 때문에 import 과정이 필요하다.

```cpp
#include<opencv2/imgproc/types_c.h>
cvtColor(BGR_img, HSV_img, CV_BGR2HSV);
```
