#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>




using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	int thresh = 100;
	VideoCapture cap("Name of video");		// input of video. currently file name in the src directory. 
	if (!cap.isOpened()) {
		cout << "Opening file failed" << endl;
		return -1;
	}
	////////////////////////////////////////////////
	/////////////////////////////////////////////////


	namedWindow("Movement", CV_WINDOW_AUTOSIZE);


	//////////////////////////////////////////////
	//////////////////////////////////////////////
	while (1) {
		Mat frame;
		Mat gray;
		Mat copyy;
		vector<vector<Point> > contours;

		bool bSuccess = cap.read(frame);
		if (!bSuccess) {
			cout << "can't read frame" << endl;
			break;
		}
		CvMemStorage* storage = cvCreateMemStorage(0);
		vector<vector<Point> >  contour;
		
		
		cvtColor(frame, gray, CV_BGR2GRAY);
		
		threshold(gray, gray, 240, 255, CV_THRESH_BINARY);
		findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		contour.resize(contours.size());

		cv::Mat gray2(gray.size(), CV_8UC3, cv::Scalar(0, 0, 0));
		for (size_t k = 0; k < contours.size(); k++) {
			//obtain a sequence of points of contour, pointed by the variable 'contour'
			approxPolyDP(Mat(contours[k]), contour[k], 16, true);

			//if there are 3  vertices  in the contour(It should be a triangle)
			if (contour[k].size() == 3 && contour[k][0].y < 600 && contour[k][0].x > 100) // filtering out the arms and problem sections of the submerine.
			{
				
				vector<Rect> boundRect(contours.size());
				vector<Point2f>center(contours.size());
				vector<float>radius(contours.size());

			
				
					boundRect[k] = boundingRect(Mat(contour[k]));
					minEnclosingCircle((Mat)contour[k], center[k], radius[k]);
				


				/// Draw polygon around target

					rectangle(frame, boundRect[k].tl(), boundRect[k].br(), Scalar(255, 0, 0), 2, 8, 0);
					circle(frame, center[k], 5, Scalar(0,255,0), 10, 1, 0);
			
				
			}
			else {

			}
		}


		imshow("Movement", frame);

		if (waitKey(30) == 27) {
			cout << "quit" << endl;
			break;
		}
	}

	return 0;


}
