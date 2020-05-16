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

- **build setting에 들어간다.**
- Header Search Path 설정하기  hd' '(매크로)
- Library Search Path 설정하기  lb' '
- Other Link 설정하기 link' '
- Signing & Capabilities에서 Disable Library Validation 체크하기
