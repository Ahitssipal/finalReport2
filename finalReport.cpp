#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
using namespace std;
using namespace cv;

int edgeThick = 3;

Mat based(500 + edgeThick * 2, 500 + 400 + edgeThick * 2, CV_8UC3, Scalar(255, 255, 255));
Point ptOld;
void on_mouse(int evnet, int x, int y, int flags, void* userdata);
void edgeLine(Mat img);
void drawText(Mat img, const string text);


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

int main(void)
{
	Mat numberDrawing(500 + edgeThick * 2, 500 + edgeThick * 2, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberDrawing);
	numberDrawing.copyTo(based(rectDrawing));

	Mat numberSave(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberSave);
	drawText(numberSave, "Save");
	numberSave.copyTo(based(rectSave));

	Mat numberLoad(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberLoad);
	drawText(numberLoad, "Load");
	numberLoad.copyTo(based(rectLoad));

	Mat numberClear(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberClear);
	drawText(numberClear, "Clear");
	numberClear.copyTo(based(rectClear));

	Mat numberRun(100 + edgeThick * 2, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberRun);
	drawText(numberRun, "Run");
	numberRun.copyTo(based(rectRun));

	Mat numberExit(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberExit);
	drawText(numberExit, "Exit");
	numberExit.copyTo(based(rectExit));

	Mat numberFeature1(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberFeature1);
	drawText(numberFeature1, "Contours");
	numberFeature1.copyTo(based(rectFeature1));

	Mat numberFeature2(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberFeature2);
	drawText(numberFeature2, "center");
	numberFeature2.copyTo(based(rectFeature2));

	Mat numberFeature3(100, 200, CV_8UC3, Scalar(255, 255, 255));
	edgeLine(numberFeature3);
	drawText(numberFeature3, "Feature3");
	numberFeature3.copyTo(based(rectFeature3));

	Mat clearBased = based.clone();

	namedWindow("numberWindow");
	setMouseCallback("numberWindow", on_mouse, &clearBased);
	imshow("numberWindow", based);
	waitKey(0);

	return 0;
}

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
	Mat* clearBased = static_cast<Mat*>(userdata);
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		if (drawingSpace.contains(Point(x, y))) {
			ptOld = Point(x, y);
		}
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
		else if (rectClear.contains(Point(x, y)))
		{
			clearBased->copyTo(based);
			imshow("numberWindow", based);
			cout << "입력창 삭제됨." << endl;
		}
		else if (rectExit.contains(Point(x, y)))
		{
			cout << "프로그램 종료" << endl;
			destroyAllWindows();
		}
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
		else if (rectFeature3.contains(Point(x, y)))
		{
			Mat gray;
			cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

			// Canny 엣지 검출
			Mat edges;
			Canny(gray, edges, 50, 150);

			// 컨투어 추출
			vector<vector<Point>> contours;
			findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			// 컨투어 루프
			for (size_t i = 0; i < contours.size(); i++) {
				// 컨투어를 둘러싸는 최소 사각형
				RotatedRect boundRect = minAreaRect(contours[i]);

				// 컨투어 방향 확인
				float angle = boundRect.angle;
				if (boundRect.size.width < boundRect.size.height)
					angle = 90 + angle;

				// 방향 결정
				if (angle > -45 && angle <= 45)
					cout << "오른쪽" << endl;
				else if (angle > 45 && angle <= 135)
					cout << "아래" << endl;
				else if (angle > 135 || angle <= -135)
					cout << "왼쪽" << endl;
				else
					cout << "위" << endl;
			}
		}
		break;
	case EVENT_MOUSEMOVE:
		if (flags && EVENT_FLAG_LBUTTON && drawingSpace.contains(Point(x, y))) {
			if (drawingSpace.contains(ptOld))
			{
				line(based, ptOld, Point(x, y), Scalar(0, 0, 0), 17);
				imshow("numberWindow", based);
			}
		}
		ptOld = Point(x, y);
		break;
	default:
		break;
	}
}

void edgeLine(Mat img)
{
	line(img, Point(0, 0), Point(img.cols - 1, 0), Scalar(0, 0, 0), edgeThick);
	line(img, Point(img.cols - 1, 0), Point(img.cols - 1, img.rows - 1), Scalar(0, 0, 0), edgeThick);
	line(img, Point(img.cols - 1, img.rows - 1), Point(0, img.rows - 1), Scalar(0, 0, 0), edgeThick);
	line(img, Point(0, img.rows - 1), Point(0, 0), Scalar(0, 0, 0), edgeThick);
}

void drawText(Mat img, const string text)
{
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 1;
	int thickness = 3;

	Size sizeText = getTextSize(text, fontFace, fontScale, thickness, 0);
	Size sizeImg = img.size();

	Point org((sizeImg.width - sizeText.width) / 2, (sizeImg.height + sizeText.height) / 2);
	putText(img, text, org, fontFace, fontScale, Scalar(0, 0, 0), thickness);
}