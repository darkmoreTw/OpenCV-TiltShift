#include <iostream>
#include <vector>
 
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
 
using namespace std;
using namespace cv;
 
/// Global Variables
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
int THRESHOLD = 7;
bool mouseClicked=false;


CvPoint VertexOne,VertexThree;//����Ϊ����W�I�M�k�U�I
CvScalar Color;//�خ��C��
int Thickness;//�خزʲ�
int Shift;//�خؤj�p(0�����`)
int key;//����X 

Mat src; Mat dst;
Mat rect_roi,src_roi;
Mat hsv;
Mat hhsv;
char window_name[] = "Smoothing Demo";
 
/// Function headers
int display_caption( char* caption );
int display_dst( int delay );
void enhanceColor();
void smoothImg();
void onMouse(int event,int x,int y,int flags,void* param); 
int main( int argc, char** argv )
{
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
 
  /// Load the source image
  src = imread( "14300000883681127684929714972.jpg", 1 );
  cvSetMouseCallback(window_name,onMouse,NULL);//	
  //if( display_caption( "Original Image" ) != 0 ) { return 0; }
 
  dst = src.clone();
  if( display_dst( DELAY_CAPTION ) != 0 ) { return 0; }
 
  waitKey(0);
  return 0;
}
 
int display_caption( char* caption )
{
  dst = Mat::zeros( src.size(), src.type() );
  putText( dst, caption,
           Point( src.cols/4, src.rows/2),
           CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );
 
  imshow( window_name, dst );
  int c = waitKey( DELAY_CAPTION );
  if( c >= 0 ) { return -1; }
  return 0;
}
 
int display_dst( int delay )
{
  imshow( window_name, dst );
  int c = waitKey ( delay );
  if( c >= 0 ) { return -1; }
  return 0;
}
void onMouse(int event,int x,int y,int flag,void* param){

    if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){//�o�쥪�W���y��
        VertexOne=cvPoint(x,y);
		enhanceColor();
		
		mouseClicked=true;
    }
    if(event==CV_EVENT_LBUTTONUP||event==CV_EVENT_RBUTTONUP){//�o��k�U���y��
        VertexThree=cvPoint(x,y);
		
		src_roi = hhsv(Rect(VertexOne,VertexThree)); 
		smoothImg(); 
		rect_roi = hhsv(Rect(VertexOne,VertexThree)); 
		src_roi.copyTo(rect_roi);
		imshow( "HSV_test", hhsv );
		imshow( window_name, dst);	
		mouseClicked=false;
    }

    if(flag==CV_EVENT_FLAG_LBUTTON||flag==CV_EVENT_FLAG_RBUTTON){//�즲�ƹ�
		dst.release();
		src.copyTo(dst);//����Եe�ϴݼv����
        VertexThree=cvPoint(x,y);		
		rectangle(dst, VertexOne, VertexThree, cv::Scalar(0, 0, 200), 3, CV_AA);
		imshow( window_name, dst );	
        
    }
	if(event!=0){
		printf("VertexOne( %d, %d) ",VertexOne.x,VertexOne.y);
		printf("VertexThree( %d, %d)\n",VertexThree.x,VertexThree.y);
	}
}
void enhanceColor(){
		cvtColor(src,hsv,CV_BGR2HSV);//�নhsv����
		uchar lut[256][3];  
		CvMat* lut_mat;  

		lut_mat = cvCreateMatHeader( 1, 256, CV_8UC3 );  
		cvSetData( lut_mat, lut,0);  

		for( int i = 0; i < 256; i++ ) {  
			int v = (i+30);  
			if( v < 0 )v = 0;  
			else if( v > 255 )v = 255;  
			lut[i][0] = (uchar)i; //h 
			lut[i][1] = (uchar)v; //s:�W�[���M 			
			lut[i][2] = (uchar)v-15; //v:�W�[�G��
		}  
		IplImage* oldstyleimg = &hsv.operator IplImage();
		cvLUT( oldstyleimg, oldstyleimg, lut_mat );  
		cvtColor( hsv, hhsv, CV_HSV2BGR );  
		//�W�[���
		hhsv.convertTo( hhsv, -1, 1.2,0);
		
}
void smoothImg(){
	blur( hhsv, hhsv, Size( 7, 7 ), Point(-1,-1) );
	
}