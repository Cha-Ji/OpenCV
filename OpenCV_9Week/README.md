# 컬러 영상과 채널

- 1교시

    ## BGR 세 개의 컬러로 나누기

    ```cpp

    	Vector<Mat> img_bgr_src;
    	Mat img_src = imread(.jpg);
     
    	split(img_src,img_bgr_src); // 3개의 채널을 3개의 Mat으로 분리
    ```

    img_bgr_src[0..2] : BGR



    ⇒ 백색에 다시 색을 입혀줘야 한다.

- 2교시

    색을 입혀주는 과정

    색깔별로 분할하고 다른 색의 Scalar를 0으로 만들어 다시 merge한다. 

    ```cpp
    img_black = zeros(img_src.rows,img_src.cols,CV_8UC1);

    img_tmp.push_back(img_bgr_src[0]);
    img_tmp.push_back(img_black); //Scalar(0)
    img_tmp.push_back(img_black);
    merge(img_tmp, img_bgr_split[0]);
    ```

   
    - mixChannel : 알아서 실습

    ## 행렬 간 연산

    원소 간 산술 연산

    - substract

    벡터 연산

    논리 연산

    - bitwise_and

- 3교시

    ## 행렬 간 연산

    크기가 다른 행렬 간의 연산은 resize가 필요하다.

    ```cpp
    Mat img1
    Mat img2

    resize(img2,img2,Size(img1.cols,img1.rows));
    ```

    add 함수를 통해 더하면 밝게 겹쳐진다.

    ⇒ 명도를 유지하려면 addWeighted(A*a + B*b + c)

    - 논리연산

    검은 바탕에 흰 문양이 있는 이미지와 and 연산을 취하면
    흰 문양이 있는 부분은 그대로 남고, 검은 바탕은 모두 검게 변한다.

    검은 바탕에 흰 문양이 있는 이미지와 or 연산을 취하면
    흰 문양이 있는 부분은 희게 변하고, 검은 바탕에 있는 부분만 그대로 남는다.

    ellipse : 부채꼴, 각도


    360도 : 원

    bitwise_and

- 4교시

    사진 합성(액자형)

    검은 바탕에 흰 화면을 통해

    bitwise_and : 특정 부분만 보이기
    bitwise_or : 특정 부분만 가리기

    ## 실습

    사진 A에서 타원을 그리면 타원 속 이미지가

    사진 B에 덮이는 프로그램 작성하기

-
