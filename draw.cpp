#include "draw.h"

detect detc;
struct point{
    vector<pair<int,int>> points;
    Scalar color;
    int ptSize;
    int ptType;
}shape;

vector<point> canvas(1);
pair<int,int> before;
int obj_count = 0;
auto keeper = high_resolution_clock::now();
int draw_avail = 1;

Scalar color = Scalar(255,0,0);
string notiv = "Free Lines";
int ptSize  = 1;
int ptType  = 1;

void draw::setDet(detect &det)
{   detc = det;
}

void draw::drawVector(Mat &frame)
{   if(canvas.size()>0)
        for(int i=0;i<canvas.size();i++)
            if(canvas[i].points.size()>1)
            {   if(canvas[i].ptType==1)
                {   for(int j=0;j<canvas[i].points.size()-1;j++)
                        line(frame,Point(canvas[i].points[j].first,canvas[i].points[j].second),Point(canvas[i].points[j+1].first,canvas[i].points[j+1].second),canvas[i].color,canvas[i].ptSize+1);
                }
                if(canvas[i].ptType==2)
                {   int j = 0;
                    for(j;j<canvas[i].points.size()-1;j++)
                        line(frame,Point(canvas[i].points[j].first,canvas[i].points[j].second),Point(canvas[i].points[j+1].first,canvas[i].points[j+1].second),canvas[i].color,canvas[i].ptSize+1);
                    line(frame,Point(canvas[i].points[j].first,canvas[i].points[j].second),Point(canvas[i].points[0].first,canvas[i].points[0].second),canvas[i].color,canvas[i].ptSize+1);
                }
                if(canvas[i].ptType==3)
                {   for(int j=0;j<canvas[i].points.size()-1;j++)
                        cv::circle(frame,Point(canvas[i].points[j].first,canvas[i].points[j].second),(canvas[i].ptSize+1)*2,canvas[i].color,-1);
                }
            }
}

void draw::ui(Mat &frame, int H)
{   int thick   = 2;
    int btnWdh  = frame.cols/8;
    Mat control = imread("canvas.png",1);

    char buff[3];
    sprintf(buff, "%d", ptSize);
    string ptS(buff);

    addWeighted(frame,1.0,control,0.6,0.0,frame);
    putText(frame,ptS,Point(400,465),2,1.5,color,2);
    putText(frame,notiv,Point(10,35),2,0.7,color,2);

    if(obj_count > 0)
    {   putText(frame,"Clear",Point(560,35),2,0.7,Scalar(0,0,255),2);
        rectangle(frame,Point(555,10),Point(627,45),Scalar(0,255,255),2);

        putText(frame,"Undo",Point(477,35),2,0.7,Scalar(255,0,0),2);
        rectangle(frame,Point(470,10),Point(540,45),Scalar(0,255,255),2);
    }
}

void draw::objTrack(Mat &frame)
{   auto real = high_resolution_clock::now();
    int pxBound = frame.rows-65;
    int upBound = 50;

    ui(frame,pxBound);

    if(detc.getCoord()!=make_pair(0,0) && detc.getCoord()!=before && draw_avail)
    {   int xDetc = detc.getCoord().first;
        int yDetc = detc.getCoord().second;

        shape.color  = color;
        shape.ptSize = ptSize;
        shape.ptType = ptType;

        if(yDetc<pxBound && yDetc>upBound)
        {   if(duration_cast<seconds>(real-keeper).count()>0.5)
            {   canvas.push_back(shape);
                ++obj_count;
                shape.points.clear();
                keeper = high_resolution_clock::now();
            }else keeper = high_resolution_clock::now();

            shape.points.push_back(detc.getCoord());
            canvas[obj_count]=shape;
        }
        else if(yDetc > upBound)
        {   if(xDetc>570) color = Scalar(255,0,0);
            else if(xDetc>500) color = Scalar(0,255,0);
            else if(xDetc>440) color = Scalar(0,0,255);
            else if(xDetc<390)
            {   if(xDetc>330) ptSize = 3;
                else if(xDetc>265) ptSize = 2;
                else if(xDetc>200) ptSize = 1;
                else if(xDetc>140)
                     {  ptType = 3;
                        notiv = "Dots";
                     }
                else if(xDetc>70)
                     {  ptType = 2;
                        notiv = "Closed";
                     }
                else if(xDetc>0)
                     {  ptType = 1;
                        notiv = "Free Lines";
                     }
            }
        }
        else if(obj_count > 0)
        {   if(xDetc>550)
            {   canvas.clear();
                shape.points.clear();
                canvas.push_back(shape);
                obj_count = 0;
            }
            else if(xDetc>465)
            {   if(canvas[obj_count].points.size()>0)
                canvas[obj_count].points.pop_back();
            }
        }
    }

    before = detc.getCoord();
    drawVector(frame);
}

void draw::setDrawState(int in){ draw_avail=in; }
int draw::getDrawState(){ return draw_avail; }
