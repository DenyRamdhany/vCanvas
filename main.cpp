#include "includes.h"
#include "draw.h"

int frameW = 640;
int frameH = 480;
int state  = 0;

int main()
{   detect det;
    draw canvas;
    Mat camera;

    det.setFrame(frameH,frameW);
    det.setObjChar(3,2,0.1);
    det.setMorph(8,20,1);
    det.setSmooth(5,1);
    det.setTresh(Scalar(100,40,10),Scalar(200,180,255));

    canvas.setDet(det);
    //canvas.setDrawState(0);

	VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,frameW);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,frameH);

	while(1)
    {   capture.read(camera);
        flip(camera,camera,1);

		(state)?det.objDetect(camera,4,Scalar(0,255,255)):det.objDetect(camera,4,Scalar(0,0,255));
		det.calibrate();
		canvas.setDrawState(state);
		canvas.objTrack(camera);

		resize(camera,camera,Size(frameW*1.5,frameH*1.5));
        imshow("DrawME", camera);

		if((char)waitKey(1) == 32)(state==1)?state=0:state=1;
	}
	return 0;
}
