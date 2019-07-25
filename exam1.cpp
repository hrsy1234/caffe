#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "image_processing.hpp"
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;
using namespace std;

#define ch		3

int main(){	
    
   ///////////////image read////////////
   	Mat image;
	image = imread("image/index.jpg",IMREAD_COLOR);
	if(image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
    
    namedWindow("Original",WINDOW_AUTOSIZE);
    imshow("Original",image);

	int F_row;
	int F_col;

	printf("This is convolution\n\n");
	printf("write F_row and F_col\n");
	scanf("%d %d" , &F_row, &F_col);

	int A_row = image.rows;
	int A_col = image.cols;
	int padding;
	int stride;
	
	printf("write padding and stride value\n");
	scanf("%d %d" , &padding, &stride);

	
    ////////////convolution////////////////////////////////
	int** F = filter(F_row,F_col);
	int*** A = image_to_int_arr(image);
	int *** C = convolution(A, A_row,  A_col, F, F_row,  F_col,1, stride);
	Mat B = int_arr_to_image(C,(A_row - F_row+ 2*padding)/stride + 1, (A_col - F_col+ 2*padding)/stride + 1,image.type());
	
	///////////max_pooling////////////////////////////////
	int kernel;
	printf("This is maxpooling\n\n");
	printf("write kernel and stride value\n");
	scanf("%d %d" , &kernel, &stride);
	
	int*** M = max_pooling(A,A_row,A_col,kernel,stride);
	Mat M1 = int_arr_to_image(M , (A_row-kernel)/stride +1, (A_col-kernel)/stride +1, image.type());

	///////////show convolution_image/////////////////////
	namedWindow("convol",WINDOW_AUTOSIZE);
    imshow("convol",B);

	///////////show max_pooling_image/////////////////////
	namedWindow("maxpooling",WINDOW_AUTOSIZE);
    imshow("maxpooling",M1);

    //////////////use opencv image processing ////////////
    Mat dst(B.rows,B.cols,image.type());
    Mat cvkernel = (Mat_<double>(3,3) <<  -1,-1,-1,-1,8,-1,-1,-1,-1 ); // 0.11111,0.11111,0.11111,0.11111,0.11111,0.11111,0.11111,0.11111,0.11111
    int f_s = 0;
	for (int i=0; i<cvkernel.rows; i++){
		for (int j=0; j<cvkernel.cols; j++){
			f_s += cvkernel.at<double>(i,j);
		}
	}
	if (f_s!=0){
		for (int i=0; i<cvkernel.rows; i++){
			for (int j=0; j<cvkernel.cols; j++){
				cvkernel.at<double>(i,j) /= f_s;
			}
		}	
	}
	
	filter2D(image,dst,-1,cvkernel,Point(-1,-1),0,0); // 1,2,1,2,4,2,1,2,1 // -1,-1,-1,-1,8,-1,-1,-1,-1
    imshow("filter2D demo" , dst);

	/////compare myconvolution and opencv convolution/////
	Mat res = dst-B;
    imshow("compare image" , res);
	for(int m=0; m<ch; m++){
		for(int i=0; i<B.rows; i++){  
     		for(int j=0; j<B.cols; j++){
				if(dst.at<Vec3b>(i,j)[m]-B.at<Vec3b>(i,j)[m] != 0)
					printf("%d ",dst.at<Vec3b>(i,j)[m]-B.at<Vec3b>(i,j)[m]);
			//printf("%d " , res.at<Vec3b>(i,j)[m]);
			}
		}
	}
	
	waitKey(0);
    
	return 0;
}