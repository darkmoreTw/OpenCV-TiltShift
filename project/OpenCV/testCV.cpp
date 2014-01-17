#include <iostream>
#include <vector>
 
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
 
using namespace std;
using namespace cv;
 
/// Global Variables
int smoothRate = 7;
int smoothType = 1;
bool mouseClicked=false;


CvPoint VertexOne,VertexThree;//����Ϊ����W�I�M�k�U�I
string filename;
//��X�ɮצW��
char outputName[20];
int outCount=1;

Mat src; Mat dst;
Mat rect_roi,src_roi;
Mat hsv_src,hsv_dst;
char window_name[] = "TiltShift";
char Rwindow_name[] = "TiltShift_Result";
/// Function headers
void enhanceColor();
void smoothImg();
void getRectangle();
void onMouse(int event,int x,int y,int flags,void* param); 
int main( int argc, char** argv )
{
	cout<<"Input the file name of image :";
	cin>>filename;
	// Load the source image
	src = imread( filename, 1 );

	namedWindow( window_name, 1 );
	cvSetMouseCallback(window_name,onMouse,NULL);
 
	dst = src.clone();
	imshow( window_name, dst );
	waitKey(0);
	return 0;
}
 
void onMouse(int event,int x,int y,int flag,void* param){

    if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){//�ƹ����U
        VertexOne=cvPoint(x,y);
		enhanceColor();
		mouseClicked=true;
    }
    if(event==CV_EVENT_LBUTTONUP||event==CV_EVENT_RBUTTONUP){//�ƹ���}
        VertexThree=cvPoint(x,y);
		getRectangle();

		src_roi = hsv_src(Rect(VertexOne,VertexThree)); 
		smoothImg(); 
		rect_roi = hsv_dst(Rect(VertexOne,VertexThree)); 
		src_roi.copyTo(rect_roi);
		sprintf(outputName,"output%d.jpg",outCount);
		imshow( Rwindow_name, hsv_dst );
		//��X�ɮ�
		printf("Image saved->%s\n",outputName);
		imwrite( outputName, hsv_dst );
		outCount++;
		//����Եe�ϴݼv����
		dst.release();
		src.copyTo(dst);

		rectangle(dst, VertexOne, VertexThree, cv::Scalar(0, 0, 200), 3, CV_AA);
		imshow( window_name, dst);	
		mouseClicked=false;
    }

    if(flag==CV_EVENT_FLAG_LBUTTON||flag==CV_EVENT_FLAG_RBUTTON){//�즲�ƹ�
		dst.release();
		src.copyTo(dst);//����Եe�ϴݼv����
        VertexThree=cvPoint(x,y);		
		line(dst, VertexOne, VertexThree, cv::Scalar(0, 0, 200), 3, CV_AA);
		imshow( window_name, dst );	
        
    }

}
void enhanceColor(){
		cvtColor(src,hsv_src,CV_BGR2HSV);//�নhsv����
		uchar lut[256][3];  
		CvMat* lut_mat;  

		lut_mat = cvCreateMatHeader( 1, 256, CV_8UC3 );  
		cvSetData( lut_mat, lut,0);  
		//�W�[���M��
		for( int i = 0; i < 256; i++ ) {  
			int v = i+40;  
			if( v < 0 )v = 0;  
			else if( v > 255 )v = 255;  
			lut[i][0] = (uchar)i; //h 
			lut[i][1] = (uchar)v; //s:���M 		
			lut[i][2] = (uchar)i; //v:�G��
		}  
		IplImage* oldstyleimg = &hsv_src.operator IplImage();
		cvLUT( oldstyleimg, oldstyleimg, lut_mat );  
		cvtColor( hsv_src, hsv_src, CV_HSV2BGR );  
		//�W�[���
		hsv_src.convertTo(hsv_src,-1,1.4,0);
		
}
//���ƤƼv��
void smoothImg(){
	switch(smoothType){
		case 0:
			blur( hsv_src, hsv_dst, Size( smoothRate, smoothRate ), Point(-1,-1) );
			break;
		case 1:
			GaussianBlur( hsv_src, hsv_dst, Size( smoothRate, smoothRate ), 0, 0 );
			break;
	}
}
//��X��ܰ϶�
void getRectangle(){
	int y1=VertexOne.y;
	int y2=VertexThree.y;
	if(y1>y2){
		int temp=y2;
		y2=y1;
		y1=temp;
	}
	VertexOne=cvPoint(0,y1);
	VertexThree=cvPoint(src.cols,y2);
}