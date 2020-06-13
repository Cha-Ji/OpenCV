- 1교시

    > 회전변환 실습

    - 이미지 확대와 보간

    ```cpp

    ```

- 2교시

    > 아핀변환 및 실습

    - 평행성을 유지한 상태에서 점을 찌그러뜨린다.

    - 회전 변환

    ```cpp
    //회전변환
    Point2f center(img.cols / 2,img.rows / 2); 
    Mat m1 = getRotationMatrix2D(center, 30 ,1);    //30도 회전
    Matx13d mat_add(0,0,1);
    m1.push_back((Mat)mat_add);

    //이동 매트릭스 계산
    int tx, ty;  //값에 따라 마지막 결과물이 이동된다.
    Matx33d m2(1,0,tx,0,1,ty,0,0,0);

    //resize 매트릭스 계산
    Matx33d m3(0.5,0,0,0,0.5,0,0,0,1);

    //회전 후 이동 후 resize 매트릭스 계산
    Mat m = (Mat)m3 * (Mat)m2 * m1;
    m.pop_back(); //추가한 한줄 mat_add를 다시 제거해준다.

    Mat dst1;
    warfAffine(img,dst1,m,Size(img.cols,img.rows));

    ```

- 3교시

    > 아핀변환 응용 프로그램

    - 클릭하는 곳에 사각형 띄우기 (맥시멈 3개)

    ```cpp
    #include<opencv2/opencv.hpp>

    using namespace std;
    using namespace cv;

    Mat src;    //원본 이미지
    Mat disply_image;
    string szTitle_src = "src";
    Point2d p[3];
    int nPoint = 0;

    void drawSelectedPoints(){
        int size_srect = 30;
        disply_image = src.clone();
        
        for(int i=0; i<nPoint; i++){
            Rect rect(p[i].x - size_srect / 2, p[i].y - size_srect /2 , size_srect,size_srect);
            rectangle(disply_image, rect, Scalar(0,0,0),8);
        }
        imshow(szTitle_src,disply_image);
    }

    void onMouse(int event, int x, int y, int flag, void*){
        if(event == EVENT_LBUTTONDOWN){
            p[nPoint].x = x;
            p[nPoint].y = y;
            nPoint = (nPoint+1) > 3 ? 3 : (nPoint+1);
            drawSelectedPoints();
        }
    }

    int main(){
        
        src = imread("F.jpg");
        namedWindow(szTitle_src, WINDOW_NORMAL);
        setMouseCallback("src", onMouse, 0);    //window생성 후 마우스 콜백을 넣을 것
        
        imshow(szTitle_src,src);
        waitKey();
        return 0;
    }
    ```

- 4교시

    > 아핀변환 + 투영변환

    > Perspective transform : 원근 변환

    ![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/69df5736-adde-487a-bebd-3958791f8d38/_2020-06-14__1.42.12.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/69df5736-adde-487a-bebd-3958791f8d38/_2020-06-14__1.42.12.png)

    ```cpp

    ```
