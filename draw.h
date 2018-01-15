#include "detect.h"

class draw: public detect
{   private:
        void ui(Mat&,int);
    public:
        void objDetect(Mat&,int);
        void objTrack(Mat&);
        void setDet(detect&);
        void drawVector(Mat&);
        void setDrawState(int);
        int getDrawState();

};
