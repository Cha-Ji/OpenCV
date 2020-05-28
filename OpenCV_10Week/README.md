# 화소처리

- 트랙바를 사용하는 밝기변환
    1. namedWindow
    2. create TrackBar
    3. imread
    4. imshow
    5. setTrackBar

    ```cpp
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
    ```

    ```cpp
    void onChange_bright(int value, void*){
        Mat img_temp = img.clone();
        
        int preValue = brightValueAt(img_temp);
        brightValueAdd(img_temp, value - preValue-128);
        cout<<value-preValue<<"\n";
        imshow(winName,img_temp);
    }
    ```

    - clone의 밝기를 변환시키지 않으면 원본이 훼손된다.
    - ex) 이미 밝기가 255가 되었다면 255로 밝기가 고정된다.

    - m.at()을 이용한 밝기 반환

    ```cpp
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
    ```

    - m.ptr()을 이용한 밝기 반환

    ```cpp
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
    ```

- 트랙바를 사용하는 명암대비

    ```cpp
    void onChange_contrast(int value, void*){
        Mat img_temp = img.clone();
        double v = value*0.5 + 1;
        multiplyContrast(img_temp,v);
        cout<<v<<" "<<value<<"\n";
        imshow(winName,img_temp);
    }
    ```

    ```cpp
    void multiplyContrast(Mat m, double value){
        m = m*value;
    }
    ```

- histogram
- 과제 : 히스토그램 평활화

- feedback
    - 오류 : 밝기를 변환해도 갱신이 되지 않는다.

    ⇒ onChange 함수 내에서 imshow를 통한 갱신이 필요하다.

    ⇒ 밝기가 0~255 범위를 벗어날 수 있다.

    - 오류 : 자료형이 맞지 않아 계산이 되지 않는다.

    ⇒ pointer 형과 int 형의 덧셈에 오류가 발생할 수 있다.

    ```cpp
    (p + j*3)[k]
    (p)[j*3 + k]
    ```

    - imwrite() : 처리된 영상을 저장한다.
