/*
 * This code is provided as part of "A Practical Introduction to Computer Vision with OpenCV"
 * by Kenneth Dawson-Howe © Wiley & Sons Inc. 2014.  All rights reserved.
 */
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

Mat hls;
Mat* croppedImage;
vector<Mat> hls_planes;

#define NUMBER_IMAGES 30

bool actualResults[NUMBER_IMAGES] = {false};
bool* expectedResults;
void expectedResult(bool* expectedResults);
void compareResults(bool* expectedResults, bool* actualResults);
double cutOffPoint(Mat first, Mat second);
int main(int argc, const char** argv)
{
	char* file_location = "Media/";
	char* image_files[] = {

		"glue-0.png",
		"glue-1.png",
		"glue-2.png",
		"glue-3.png",
		"glue-4.png",
		"glue-5.png",
		"glue-6.png",
		"glue-7.png",
		"glue-8.png",
		"glue-9.png",
		"glue-10.png",
		"glue-11.png",
		"glue-12.png",
		"glue-13.png",
		"glue-14.png",
		"glue-15.png",
		"glue-16.png",
		"glue-17.png",
		"glue-18.png",
		"glue-19.png",
		"glue-20.png",
		"glue-21.png",
		"glue-22.png",
		"glue-23.png",
		"glue-24.png",
		"glue-25.png",
		"glue-26.png",
		"glue-27.png",
		"glue-28.png",
		"glue-29.png"
    };

	// Load images
	int number_of_images = sizeof(image_files)/sizeof(image_files[0]);
	Mat* image = new Mat[number_of_images];
	for (int file_no=0; (file_no < number_of_images); file_no++)
	{
		string filename(file_location);
		filename.append(image_files[file_no]);
		image[file_no] = imread(filename, -1);
		if (image[file_no].empty())
		{
			cout << "Could not open " << image[file_no] << endl;
			return -1;
		}
	}
	croppedImage = new Mat[NUMBER_IMAGES];
	expectedResults = new bool[NUMBER_IMAGES];

	/*
		Cropping images
	*/
	croppedImage[0] = image[0](Rect(20, 170, 85, 100));
	croppedImage[1] = image[1](Rect(20, 170, 65, 100));
	croppedImage[2] = image[2](Rect(20, 170, 75, 100));
	croppedImage[3] = image[3](Rect(20, 170, 75, 100));
	croppedImage[4] = image[4](Rect(20, 170, 85, 100));

	croppedImage[5] = image[5](Rect(25, 170, 85, 100));
	croppedImage[6] = image[6](Rect(32, 170, 83, 100));
	croppedImage[7] = image[7](Rect(35, 170, 80, 100));
	croppedImage[8] = image[8](Rect(30, 170, 75, 100));
	croppedImage[9] = image[9](Rect(30, 170, 68, 100));

	croppedImage[10] = image[10](Rect(24, 170, 84, 100));
	croppedImage[11] = image[11](Rect(20, 170, 86, 100));
	croppedImage[12] = image[12](Rect(20, 170, 80, 100));
	croppedImage[13] = image[13](Rect(20, 170, 74, 100));
	croppedImage[14] = image[14](Rect(20, 170, 79, 100));

	croppedImage[15] = image[15](Rect(20, 170, 79, 100));
	croppedImage[16] = image[16](Rect(24, 170, 82, 100));
	croppedImage[17] = image[17](Rect(24, 170, 78, 100));
	croppedImage[18] = image[18](Rect(24, 170, 68, 100));
	croppedImage[19] = image[19](Rect(20, 170, 80, 100));

	croppedImage[20] = image[20](Rect(20, 170, 77, 100));
	croppedImage[21] = image[21](Rect(20, 170, 70, 100));
	croppedImage[22] = image[22](Rect(20, 170, 69, 100));
	croppedImage[23] = image[23](Rect(13, 170, 77, 100));
	croppedImage[24] = image[24](Rect(20, 170, 73, 100));

	croppedImage[25] = image[25](Rect(20, 170, 85, 100));
	croppedImage[26] = image[26](Rect(22, 170, 79, 100));
	croppedImage[27] = image[27](Rect(22, 170, 79, 100));
	croppedImage[28] = image[28](Rect(22, 170, 71, 100));
	croppedImage[29] = image[29](Rect(20, 170, 75, 100));

	 //Retrieve the results that should be expected
	expectedResult(expectedResults);

	cv::Scalar mean, stddev; //0:1st channel, 1:2nd channel, 2:3rd channel

	double cutOff = cutOffPoint(croppedImage[16], croppedImage[23]);	

	/*
		The bulk of the work is done in this for loop.
		I convert the image to a HLS image, then split it into the three different channels.
		Hue, Luminance and Saturation.
		The SD of the luminance channel is got and compared to the cut off that was based from the two
		samples.
	*/
	for(int i = 0; i < number_of_images; i++ ) {
		cvtColor(croppedImage[i], hls, CV_BGR2HLS);	//converts color image to Hue Luminance and Saturation
		split( hls, hls_planes );					//splits the 3 channels

		meanStdDev(hls_planes[1], mean, stddev, cv::Mat());
		if(stddev.val[0] > cutOff) {
			actualResults[i] = true;
		}
		if(actualResults[i] == true) {
			std::cout<<"Standard Deviation: "<<stddev.val[0]<<" - Label"<<std::endl;	//print out standard deviation
		}
		else {
			std::cout<<"Standard Deviation: "<<stddev.val[0]<<" - No Label"<<std::endl;	//print out standard deviation
		}
		imshow("", hls_planes[1]);
		waitKey(0);
	}	
	compareResults(expectedResults, actualResults);

	waitKey(0); 
	return(0);
}

/*
	This function fills the expected results array with which bottles should have a label (true)
	and which should not (false)
*/
void expectedResult(bool* expectedResults) {
	for(int i = 0; i < NUMBER_IMAGES; i++ ) {
		/*	
			if it is any of the ones with no label, set to false.
			Messy. I tried setting the array all to true and then modifying the value only in the right index positions.
			Did not work for me.
		*/
		if(i == 4 || i == 6 || i == 13 || i == 16 || i == 17 || i == 20 || i == 23 || i == 29) {
			expectedResults[i] = false;
		}
		else {
			expectedResults[i] = true;
		}
	}
}

/*
	This function takes two samples of glue bottles with no label that I felt would have the highest SD
	based on visual inspection. The greater SD will be used as my cut off point for label/non-lablled bottles. 
*/ 
double cutOffPoint(Mat first, Mat second) {
	double cutOff = 0.0;
	cv::Scalar mean, stddev1, stddev2; //0:1st channel, 1:2nd channel, 2:3rd channel
	meanStdDev(first, mean, stddev1, cv::Mat());
	meanStdDev(second, mean, stddev2, cv::Mat());
	if(stddev1.val[0] < stddev2.val[0])
		cutOff = stddev2.val[0];
	else
		cutOff = stddev1.val[0];

	return cutOff;
}
/*
	The metrics is done in this function.
	I had a read through how to do this in the Practical Introduction to Computer Vision With OpenCV
	book, and made my own function based on my understanding of this.
*/
void compareResults(bool* expectedResults, bool* actualResults) {
	int TP = 0;
	int FP = 0;
	int TN = 0;
	int FN = 0;
	for(int i = 0; i < NUMBER_IMAGES; i++) {
		if(actualResults[i] == true && expectedResults[i] == true) {
			TP++;
		}
		else if(actualResults[i] == false && expectedResults[i] == false) {
			TN++;
		}
		else if(actualResults[i] == true && expectedResults[i] == false) {
			FP++;
		}
		else if(actualResults[i] == false && expectedResults[i] == true) {
			FN++;
		}
	}
	double precision = ((double) TP) / ((double) (TP+FP));
	double recall = ((double) TP) / ((double) (TP+FN));
	double accuracy = ((double) (TP+TN)) / ((double) (TP+FP+TN+FN));
	double specificity = ((double) TN) / ((double) (FP+TN));
	double f1 = 2.0*precision*recall / (precision + recall);

	cout<<"Precision = "<<precision<<std::endl;
	cout<<"Recall = "<<recall<<std::endl;
	cout<<"Accuracy = "<<accuracy<<std::endl;
	cout<<"Specifity = "<<specificity<<std::endl;
	cout<<"F1 = "<<f1<<std::endl;
}
