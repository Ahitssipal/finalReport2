# finalReport2

# 그림 및 이미지 분석 도구

이 프로젝트는 OpenCV를 사용하여 개발된 GUI 응용 프로그램으로, 사용자가 0부터 9까지 임의의 숫자를 마우스로 그리거나, 저장, 불러오기 등 여러 작업을 할 수 있는 환경을 제공함.

## 기능

- **그림 그리기 영역**: 마우스를 사용하여 특정 영역에 그림을 그릴 수 있습니다.
- **저장 및 불러오기**: 그린 이미지를 파일로 저장하고 파일에서 이미지를 불러올 수 있습니다.
- **그림 지우기**: 그림 그리기 영역을 지울 수 있습니다.
- **종료**: 응용 프로그램을 종료합니다.
- **기능 버튼**: 외곽선 검출 및 무게 중심 계산 등의 사용자 정의 기능을 수행합니다.

버튼을 클릭하여 다양한 작업을 수행할 수 있습니다:
  - **Save**: 현재 그림을 파일로 저장합니다.
  - **Load**: 파일에서 그림을 불러옵니다.
  - **Clean**: 그림 그리기 영역을 지웁니다.
  - **Exit**: 응용 프로그램을 닫습니다.
  - **Contours**: 그림에서 외곽선을 검출하여 표시합니다.
  - **Center**: 가장 큰 외곽선의 무게 중심을 계산하고 출력합니다.

## 코드 구조

### GUI 레이아웃
// 그리기 영역
'''
Rect rectDrawing(0, 0, 500 + edgeThick * 2, 500 + edgeThick * 2);
Rect drawingSpace(edgeThick, edgeThick, 500, 500);

// 1열 메뉴
Rect rectSave(500 + edgeThick * 2, 0, 200, 100);
Rect rectLoad(500 + edgeThick * 2, 100, 200, 100);
Rect rectClear(500 + edgeThick * 2, 200, 200, 100);
Rect rectRun(500 + edgeThick * 2, 300, 200, 100 + edgeThick * 2);
Rect rectExit(500 + edgeThick * 2, 400 + edgeThick * 2, 200, 100);

// 2열 메뉴
Rect rectFeature1(500 + 200 + edgeThick * 2, 0, 200, 100);
Rect rectFeature2(500 + 200 + edgeThick * 2, 100, 200, 100);
Rect rectFeature3(500 + 200 + edgeThick * 2, 200, 200, 100);
'''
`Rect` 개체를 사용하여 정의됩니다:
- `rectDrawing`, `rectSave`, `rectLoad`, `rectClear`, `rectRun`, `rectExit`, `rectFeature1`, `rectFeature2`, `rectFeature3`

### 주요 함수

#### main

창을 초기화하고 콜백 함수를 설정하며 버튼을 그립니다.

#### on_mouse

마우스 이벤트를 처리하여 그리기, 저장, 불러오기, 지우기, 기능 실행 등을 수행합니다.

#### edgeLine

주어진 이미지에 테두리를 그립니다.

#### drawText

주어진 이미지 중앙에 텍스트를 그립니다.

### 예제 기능

#### 외곽선 검출 (`Contours`)

그림을 그레이스케일로 변환하고 외곽선을 검출하여 검출된 외곽선의 수를 출력합니다.

#### 무게 중심 계산 (`Center`)

가장 큰 외곽선의 무게 중심을 계산하고, 이를 그림 영역의 백분율로 출력합니다.
