#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>




using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	char output[80];
	int thresh = 100;
	std::string pr;
	const std::string videoStreamAddress = "http://192.168.254.1:8090/?action=stream";
//	VideoCapture cap(videoStreamAddress);	// input of video. currently file name//// in the src directory. 
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Opening file failed" << endl;
		return -1;
	}
	namedWindow("Movement", CV_WINDOW_AUTOSIZE);

	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	int c = frame_width / 3;
	int r = frame_height / 3;
	



	////////////////////////////////////////////////
	/////////////////////////////////////////////////


	namedWindow("Movement", CV_WINDOW_AUTOSIZE);


	//////////////////////////////////////////////
	//////////////////////////////////////////////
	while (1) {
		std::string output = "(";
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


		// Convert input image to HSV
		cv::Mat hsv_image;
		cv::cvtColor(frame, hsv_image, cv::COLOR_BGR2HSV);
		
		 	// Threshold the HSV image, keep only the red pixels
		 	cv::Mat lower_red_hue_range;
		 	cv::Mat upper_red_hue_range;
		 	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
	 	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

		cv::Mat red_hue_image;
			cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
		
			cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(1, 1), 2, 2);
		findContours(red_hue_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

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



				

				rectangle(frame, boundRect[k].tl(), boundRect[k].br(), Scalar(255, 0, 0), 2, 8, 0);
				circle(frame, center[k], 5, Scalar(0, 255, 0), 10, 1, 0);
				stringstream ss;
				ss << "(";
				
				for (int i = -1; i <= 1; i++) {
					if (center[k].x >= (c*(i+1)) && center[k].x < (c)* (i+2)) {
						ss << i;
						break;
					}
				}
				ss << ", ";
				for (int i = -1; i <= 1; i++) {
					if (center[k].y >= (r * (i+1)) && center[k].y < (r)* (i + 2)) {
						ss << i;
						break;
					}
				}

				ss << ")";
				pr = ss.str();
				break;
			}
			else {
				output ="object not detected";
			}
		}
		
		
		
		putText(frame, pr, cvPoint(30, 45),
			FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 0, 0), 1, CV_AA);
		imshow("Movement", frame);
	

		if (waitKey(30) == 27) {
			cout << "quit" << endl;
			break;
		}
	}

	return 0;


}