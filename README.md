> 실습환경

- MacOs
- Xcode
- C++

> 참고

[OpenCV](https://opencv.org)

> 기본 코드

```cpp
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
 Mat image(300,400,CV_8U,Scalar(255));

 waitKey();
 return 0;
}
```

> Xcode 에서의 초기 세팅

최종 세팅

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/989d4d6f-ffa5-445e-8a3f-6e05561d936f/_2020-05-10__9.42.33.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/989d4d6f-ffa5-445e-8a3f-6e05561d936f/_2020-05-10__9.42.33.png)

- **build setting에 들어간다.**
- Header Search Path 설정하기  hd' '(매크로)
- Library Search Path 설정하기  lb' '
- Other Link 설정하기 link' '
- Signing & Capabilities에서 Disable Library Validation 체크하기

[OpenCV_8주차](https://www.notion.so/OpenCV_8-0d1164cf14c3446589118c4d7c7119c0)

[OpenCV_9주차](https://www.notion.so/OpenCV_9-5553b2fd81fa40f7ad739e7ddbdce48f)
