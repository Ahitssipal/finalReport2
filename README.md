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

```
// 그리기 영역
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
```
`Rect` 개체를 사용하여 정의됩니다:
- `rectDrawing`, `rectSave`, `rectLoad`, `rectClear`, `rectRun`, `rectExit`, `rectFeature1`, `rectFeature2`, `rectFeature3`

### 주요 예제 기능

#### 이미지 저장 (`Save`)

저장할 파일의 이름을 입력받고 Drawing 영역의 이미지를 저장한다.

```
else if (rectSave.contains(Point(x, y)))
{
	string filename;
	cout << "저장할 파일명 입력 : ";
	getline(cin, filename);
	if (filename == "cancel") break;
	bool saving = imwrite(filename, based(drawingSpace));
	if (saving) cout << filename << " 파일이 저장됨." << endl;
	else cout << filename << " 파일 저장실패." << endl;
}
```

#### 이미지 불러오기 (`Load`)

불러올 파일 이름을 입력받고 Drawing 영역에 이미지를 표시한다.
```
else if (rectLoad.contains(Point(x, y)))
{
	string loadFileName;
	cout << "불러올 파일명을 입력 : ";
	getline(cin, loadFileName);
	if (loadFileName == "cancel") break;
	Mat temp = imread(loadFileName, IMREAD_COLOR);
	if (temp.empty()) cout << loadFileName << " 불러오기 실패." << endl;
	else cout << loadFileName << " 불러옴." << endl;
	temp.copyTo(based(drawingSpace));
	imshow("numberWindow", based);
}
```

#### 외곽선 검출 (`Contours`)

그림을 그레이스케일로 변환하고 외곽선을 검출하여 검출된 외곽선의 수를 출력합니다.
```
else if (rectFeature1.contains(Point(x, y)))
{
	Mat gray;
	cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);
	gray = ~gray;
	Mat bi;
	vector<vector<Point>> contours;
	findContours(gray, contours, RETR_TREE, CHAIN_APPROX_NONE);
	cout << "외각선의 개수 : " << contours.size() << endl;
	//imshow("gray", gray);
}
```

#### 무게 중심 계산 (`Center`)

가장 큰 외곽선의 무게 중심을 계산하고, 이를 그림 영역의 백분율로 출력합니다.
```
else if (rectFeature2.contains(Point(x, y)))
{
	Mat gray;
	cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

	Mat binary;
	threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	vector<vector<Point>> contours;
	findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	if (contours.size() > 0)
	{
		int largest_contour_index = 0;
		double largest_area = 0.0;

		for (int i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);
			if (area > largest_area)
			{
				largest_area = area;
				largest_contour_index = i;
			}
		}
		RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

		Point2f vertices[4];    //왼쪽부터 시계방향
		rotated_rect.points(vertices);
		Mat dst;
		cvtColor(binary, dst, COLOR_GRAY2BGR);
		for (int i = 0; i < 4; i++)
		{
			line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
		}

		Rect bounding_box = boundingRect(contours[largest_contour_index]);
		rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

		//imshow("Bounding Box", dst);

		// Calculate the slope (angle) using vertices[0] and vertices[1]
		float dx = vertices[1].x - vertices[0].x;
		float dy = vertices[1].y - vertices[0].y;
		float angle = atan2(dy, dx) * 180.0 / CV_PI;

		// Normalize the angle to be within 0 to 180 degrees
		if (angle < -45) {
			angle += 90;
			swap(rotated_rect.size.width, rotated_rect.size.height);
		}

		// Get the rotation matrix
		Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

		// Rotate the image
		Mat rotated;
		warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

		//imshow("rotated", rotated);

		Mat rotated_gray;
		cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
		Mat rotated_binary;
		threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

		vector<vector<Point>> rotated_contours;
		findContours(rotated_binary, rotated_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		if (rotated_contours.size() > 0)
		{
			int largest_contour_index_rotated = 0;
			double largest_area_rotated = 0.0;

			for (int i = 0; i < rotated_contours.size(); i++)
			{
				double area = contourArea(rotated_contours[i]);
				if (area > largest_area_rotated)
				{
					largest_area_rotated = area;
					largest_contour_index_rotated = i;
				}
			}

			Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);	//바운딩박스 영역
			//rectangle(rotated, bounding_box_rotated, Scalar(0, 255, 0), 2);	//바운딩박스 그리기

			//imshow("important", rotated(bounding_box_rotated));

			Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());

			cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
			cop = ~cop;

			int white_x = 0;
			int white_y = 0;
			int white_total = 0; // 하얀색 픽셀의 개수를 저장할 변수를 추가합니다.

			// 이미지를 순회하며 하얀색 픽셀의 무게 중심을 계산합니다.
			for (int y = 0; y < cop.rows; y++) {
				for (int x = 0; x < cop.cols; x++) {
					if (cop.at<uchar>(y, x) == 255) { // 픽셀 값이 255인 경우 (하얀색)
						white_x += x;
						white_y += y;
						white_total++; // 하얀색 픽셀의 개수를 증가시킵니다.
					}
				}
			}

			// 하얀색 픽셀이 존재하는 경우에만 무게 중심을 계산합니다.
			if (white_total > 0) {
				double mx = static_cast<double>(white_x) / white_total; // 무게 중심 x 좌표를 계산합니다.
				double my = static_cast<double>(white_y) / white_total; // 무게 중심 y 좌표를 계산합니다.

				// 무게 중심을 이미지 크기에 대한 백분율로 변환합니다.
				double per_mx = (mx / cop.cols) * 100.0;
				double per_my = (my / cop.rows) * 100.0;

				// 결과를 출력합니다.
				cout << "무게 중심 백분율: (" << per_mx << "%, " << per_my << "%)" << endl;

				if (per_mx > 60)
					cout << "오른쪽";
				else if (per_mx < 40)
					cout << "왼쪽" ;
				else cout << "가운데";
				if (per_my > 60)
					cout << ", 아래" << endl;
				else if (per_my < 40)
					cout << ", 위" << endl;
				else cout << endl;
			}
			else {
				cout << "하얀색 픽셀이 없습니다." << endl; // 하얀색 픽셀이 없는 경우 메시지를 출력합니다.
			}
		}
	}
}
```

https://youtube.com/watch?v=5LRrK9UHPvM?si=AUAzcB5gp2_Pb33S
