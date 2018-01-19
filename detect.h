#include "includes.h"

class detect
{   protected:
        int erosi, dilasi, morph;
        int frame_h, frame_w;
        int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;
        int objMax, areaMax, areaMin;
        Mat thresholded, HSV;

    private:
        static void on_trackbar(int,void*);
        void createTrackbars();

    public:
        void objDetect(Mat&,int,Scalar);
        void objTrack(Mat&,int&,int&,int,Scalar);

        void setMorph(int,int,int=0);
        void setSmooth(int,int=0);
        void setTresh(Scalar,Scalar);
        void setFrame(int,int);
        void setObjChar(int,double,double);
        void calibrate();
        pair<int,int> getCoord();
};
