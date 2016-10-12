#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <utility>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/calib3d.hpp>
using namespace std;
struct pack{
cv::Mat g1;
cv::Mat g2;
cv::Mat disparity_left;
cv::Mat disp8;
cv::Ptr<cv::StereoBM> sbm;
};
void RefreshWindowMinDisparity(int pos,void * param){  //event handler for slider of minimum disparity
//cv::Ptr<cv::StereoBM> * s=(cv::Ptr<cv::StereoBM> *)param;
//cv::Ptr<cv::StereoBM> sobj=*s;
//sobj->setMinDisparity(pos);
struct pack *p=(struct pack *)param;
p->sbm->setMinDisparity(pos-62);
p->sbm->compute(p->g1,p->g2,p->disparity_left);
normalize(p->disparity_left, p->disp8, 0, 255, CV_MINMAX, CV_8U);
cv::imshow("disp8", p->disp8);
cv::moveWindow("disp8",400,400);
cout<<"\nvalue of min disparity="<<p->sbm->getMinDisparity()<<" "<<"value of disp12maxdiff="<<p->sbm->getDisp12MaxDiff();
}
void refreshDisp12MaxDiff(int pos,void * param){
struct pack *p=(struct pack *)param;
p->sbm->setDisp12MaxDiff(pos);
p->sbm->compute(p->g1,p->g2,p->disparity_left);
normalize(p->disparity_left, p->disp8, 0, 255, CV_MINMAX, CV_8U);
cv::imshow("disp8", p->disp8);
cv::moveWindow("disp8",400,400);
cout<<"\nvalue of min disparity="<<p->sbm->getMinDisparity()<<" "<<"value of disp12maxdiff="<<p->sbm->getDisp12MaxDiff();
}
int main()
{cv::Mat leftimg =cv::imread("./testimages/left.pgm");
cv::Mat rightimg = cv::imread("./testimages/right.pgm");
cv::Size imagesize = leftimg.size();
cv::Mat disparity_left=cv::Mat(imagesize.height,imagesize.width,CV_16S);
cv::Mat disparity_right=cv::Mat(imagesize.height,imagesize.width,CV_16S);
cv::Mat g1,g2,disp,disp8,disp9;
cv::cvtColor(leftimg,g1,cv::COLOR_BGR2GRAY);
cv::cvtColor(rightimg,g2,cv::COLOR_BGR2GRAY);

cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(0,21);

int MinDispSlider=-69; //starting of slider
sbm->setDisp12MaxDiff(22);
sbm->setSpeckleRange(8);
sbm->setSpeckleWindowSize(9);
sbm->setUniquenessRatio(0);
sbm->setTextureThreshold(507);
sbm->setMinDisparity(0);
sbm->setPreFilterCap(61);
sbm->setPreFilterSize(5);
sbm->compute(g1,g2,disparity_left);
pack param;
param.g1=g1;
param.g2=g2;
param.disparity_left=disparity_left;
param.disp8=disp8;
param.sbm=sbm;

normalize(disparity_left, disp8, 0, 255, CV_MINMAX, CV_8U);
//GaussianBlur( leftimg, disp9, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
cv::imshow("left", leftimg);
cv::moveWindow("left",0,0);
cv::imshow("right", rightimg);
cv::moveWindow("right",800,0);
cv::imshow("disp8", disp8);
cv::moveWindow("disp8",400,400);
//cv::imshow("disp9",disp9);
int minDisp12MaxDiff=0;
cv::createTrackbar("MinDisparity","disp8",&MinDispSlider,100,RefreshWindowMinDisparity,(void *)&param);
cv::createTrackbar("Disp12MaxDiff","disp8",&minDisp12MaxDiff,100,refreshDisp12MaxDiff,(void *)&param);

cv::waitKey(0);
}
