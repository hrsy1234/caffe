#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define ch		3
///// filter /////
int** filter(int F_row, int F_col)
{
	int i,j;
	int **F;
	
    F = (int**)malloc(F_row * sizeof(int*));   //Filter
		for (i = 0; i < F_row; i++){
			F[i] = (int*)malloc(F_col * sizeof(int));
		}
	
	printf("write Filter value");
	for (i = 0; i < F_row; i++){
		for (j = 0; j < F_col; j++){
			scanf("%d", &F[i][j]);
		}
	}
	return F;
}
////padding function//////
int*** padding(int ***A,int A_row,int A_col,int padding_size){
	
	int ***padding;
	int padding_row = A_row+2*padding_size;
	int padding_col = A_col+2*padding_size;
	
    padding = (int***)malloc(ch * sizeof(int**));   //max_pooling 3-matrix
	for (int i = 0; i < ch; i++)
	{
		*(padding+i) = (int**)malloc(padding_row * sizeof(int*));
		for(int j=0; j<padding_row;j++){
			*(*(padding+i)+j) = (int*)malloc(padding_col * sizeof(int));
		}
	}
	
	for(int k=0; k<ch; k++)
	{
		for(int i=0; i<padding_row; i++)
		{
			for(int j=0; j<padding_col; j++)
			{
				padding[k][i][j] = 0;
			}
			
		}
	}
	
	for(int k=0; k<ch; k++)
	{
		for(int i=0; i<padding_row; i++)
		{
			for(int j=0; j<padding_col; j++)
			{
				int max=0;
				if(i<padding_size|| j<padding_size || 
				i>=padding_row-padding_size || j>=padding_col-padding_size) padding[k][i][j] = 0;
				else padding[k][i][j] = A[k][i-padding_size][j-padding_size];
			}
		}
	}
	return padding;
}
//////max_pooling function //////
int*** max_pooling(int ***A,int A_row,int A_col, int kernel ,int stride )
{
	int max = 0;
	int ***pooling;
	int pooling_row = (A_row-kernel)/stride +1;
	int pooling_col = (A_col-kernel)/stride +1;
	int i,j,k,l;
    pooling = (int***)malloc(ch * sizeof(int**));   //max_pooling 3-matrix
	for (i = 0; i < ch; i++)
	{
		*(pooling+i) = (int**)malloc(pooling_row * sizeof(int*));
		for(j=0; j<pooling_row;j++){
			*(*(pooling+i)+j) = (int*)malloc(pooling_col * sizeof(int));
		}
	}
	
	for(k=0; k<ch; k++)
	{
		for(i=0; i<pooling_row; i++)
		{
			for(j=0; j<pooling_col; j++)
			{
				pooling[k][i][j] = 0;
			}
			
		}
	}
	for(int m=0; m<ch; m++){
		for(  i=0; i<pooling_row; i++){  
	     	for( j=0; j<pooling_col; j++){
			max=0;
	         	for(  k=0; k<kernel;k++){
	             	for(  l=0; l<kernel; l++){
						if(max<A[m][i*stride+k][j*stride+l]) max = A[m][i*stride+k][j*stride+l];
					}
	        	} 
				pooling[m][i][j] = max;
	        }
    	}
	}
	return pooling;
}
/////convolution function /////
int *** convolution(int ***A,int A_row, int A_col, int **F, int F_row, int F_col,int padding_size,int stride)
{
	clock_t begin,end;
	int ***C;
	int i,j,k,l,m;
	int C_row = (A_row - F_row+ 2*padding_size)/stride + 1;
    int C_col = (A_col - F_col+ 2*padding_size)/stride + 1;
    
    int *** pad = padding(A,A_row,A_col,padding_size);
    
	 C = (int***)malloc(ch * sizeof(int**));   //max_pooling 3-matrix
	for (i = 0; i < ch; i++)
	{
		*(C+i) = (int**)malloc(C_row * sizeof(int*));
		for(j=0; j<C_row;j++){
			*(*(C+i)+j) = (int*)malloc(C_col * sizeof(int));
		}
	}
	for(k=0; k<ch; k++){
		for( i=0; i<C_row; i++){
			for( j=0; j<C_col; j++){
				C[k][i][j] = 0;
			}
		}
	}
	begin = clock();

	int filter_sum = 0;
    for( k=0; k<F_row;k++){
        for( l=0; l<F_col; l++){
            filter_sum += F[k][l];    
        }
    }
	////convolution////
	for(m=0; m<ch; m++){
		for( i=0; i<C_row; i++){  
     		for( j=0; j<C_col; j++){
				double temp = 0;
         		for( k=0; k<F_row;k++){
             		for( l=0; l<F_col; l++)
             			temp += double(pad[m][k+i*stride][l+j*stride] * F[k][l]); 
                }
				if(filter_sum!=0){
					temp = round(temp/filter_sum);
				}
				else	temp = round(temp);
				if(temp < 0) 			temp = 0;
				else if(temp > 255) 	temp = 255;
			
				C[m][i][j] = temp;
				
					
            }
	    }   
    }
	cout <<filter_sum;
    end = clock();
    cout << "running time : " << (double)(end - begin)/CLOCKS_PER_SEC << endl << endl;
    return C;
}
////// image -> array /////
int *** image_to_int_arr(Mat A){
	int*** result;
	int A_row = A.rows;
	int A_col = A.cols;
	int i,j,m;
	
	result = (int***)malloc(ch * sizeof(int**));   
	for (i = 0; i < ch; i++)
	{
		*(result+i) = (int**)malloc(A_row * sizeof(int*));
		for(j=0; j<A_row;j++){
			*(*(result+i)+j) = (int*)malloc(A_col * sizeof(int));
		}
	}
	
	for(int k=0; k<ch; k++){
	for( i=0; i<A_row; i++){
		for( j=0; j<A_col; j++){
			result[k][i][j] = 0;
		}
	}
	}
	for(m=0; m<ch; m++){
		for(i=0; i<A_row; i++){  
     		for( j=0; j<A_col; j++){
            	result[m][i][j] = A.at<Vec3b>(i,j)[m];
         	}
    	}
	}
        
	return result;
}

//////array -> image /////
Mat int_arr_to_image(int ***A,int A_row, int A_col,int type){
	Mat result(A_row,A_col,type);

	int i,j,m;

	for(m=0; m<ch; m++){
		for(i=0; i<A_row; i++){  
     		for( j=0; j<A_col; j++){
             	 result.at<Vec3b>(i,j)[m]=A[m][i][j]; 
         	}
     	}
	}

	return result;
}
