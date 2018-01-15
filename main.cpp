#include "includes.h"
#include "draw.h"

int frameW = 640;
int frameH = 480;
int state  = 0;

int main(int argc, char* argv[])
{   detect det;
    draw canvas;
    Mat camera;

    det.setFrame(frameH,frameW);
    det.setObjChar(3,2,0.1);
    det.setMorph(8,20,1);
    det.setSmooth(5,1);
    det.setTresh(Scalar(100,40,10),Scalar(200,180,255));

    canvas.setDet(det);

	VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,frameW);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,frameH);

	while(1)
    {   capture.read(camera);
        flip(camera,camera,1);

		det.objDetect(camera,5);
		//det.calibrate();
		canvas.objTrack(camera);
		resize(camera,camera,Size(frameW*1.5,frameH*1.5));
        imshow("DrawME", camera);

		waitKey(1);
	}
	return 0;
}
