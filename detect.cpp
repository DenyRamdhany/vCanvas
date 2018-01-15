#include "detect.h"

int erosi   = 1;
int dilasi  = 1;
int smooth  = 0;
int morph   = 0;

int frame_h, frame_w;
int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;
int objMax, areaMax, areaMin;

int x=0,y=0;

Mat thresholded, HSV;

void detect::on_trackbar( int, void* )
{
}

void detect::createTrackbars(){

	namedWindow("Calibrate",0);

	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	sprintf( TrackbarName, "Erode", erosi);
	sprintf( TrackbarName, "Dilate", dilasi);

    createTrackbar( "H_MIN", "Calibrate", &H_MIN, 255, on_trackbar );
    createTrackbar( "H_MAX", "Calibrate", &H_MAX, 255, on_trackbar );
    createTrackbar( "S_MIN", "Calibrate", &S_MIN, 255, on_trackbar );
    createTrackbar( "S_MAX", "Calibrate", &S_MAX, 255, on_trackbar );
    createTrackbar( "V_MIN", "Calibrate", &V_MIN, 255, on_trackbar );
    createTrackbar( "V_MAX", "Calibrate", &V_MAX, 255, on_trackbar );
    createTrackbar( "Erode", "Calibrate", &erosi, 50, on_trackbar );
    createTrackbar( "Dilate", "Calibrate", &dilasi, 50, on_trackbar );
}

void detect::setMorph(int erode, int dilate, int use)
{   if(use)
    {   erosi = erode;
        dilasi = dilate;
        morph = 1;
    }
    else morph = 0;
}

void detect::setSmooth(int num, int x)
{   if(x==0) smooth=0;
    else smooth = num;
}

void detect::setTresh(Scalar l, Scalar h)
{   H_MIN=l[0]; H_MAX=h[0];
    S_MIN=l[1]; S_MAX=h[1];
    V_MIN=l[2]; V_MAX=h[2];
}

void detect::setFrame(int w, int h)
{   frame_h = h;
    frame_w = w;
}

void detect::setObjChar(int omax, double amax, double amin)
{   objMax = omax; areaMax = (frame_h*frame_w)*(amax/100); areaMin = (frame_h*frame_w)*(amin/100);
}


void detect::calibrate()
{   createTrackbars();
    imshow("Tresholded Image",thresholded);
    imshow("HSV Image",HSV);
}

pair<int,int> detect::getCoord(){ return make_pair(x,y); }

void detect::objDetect(Mat &canvas, int rad)
{   cvtColor(canvas,HSV,COLOR_BGR2HSV_FULL);
    if(smooth>0) cv::blur(HSV,HSV,Size(smooth,smooth));

    inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),thresholded);

    if(morph)
    {   Mat erodeElement = getStructuringElement( MORPH_RECT,Size(erosi,erosi));
        Mat dilateElement = getStructuringElement( MORPH_RECT,Size(dilasi,dilasi));

        erode(thresholded,thresholded,erodeElement);
        dilate(thresholded,thresholded,dilateElement);
    }

    Mat temp;
	thresholded.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );

	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if(hierarchy.size() > 0)
    {   int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<objMax){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>areaMin && area<areaMax && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;
			}
			if(objectFound == true) objTrack(canvas,x,y,rad);
		}else putText(canvas,"Objek Terlalu Banyak/Dekat!",Point(5,80),1,2,Scalar(0,0,255),2);
    }
}

void detect::objTrack(Mat &canvas, int &x, int &y, int rad)
{   circle(canvas,Point(x,y),rad,Scalar(0,255,0),1);
}
