#include "Capture.hpp"

using namespace std;
using namespace cv;

int main(void){

    int status;

    Capture cam_left;
    Capture cam_right;

    Mat RM1x, RM1y;
    Mat RM2x, RM2y;

    status = cam_left.open(0);

    if(status){
        return -1;
    }

    status = cam_right.open(1);

    if(status){
        return -1;
    }

    FileStorage fs("camera_remap.xml", FileStorage::READ);

    if(!fs.isOpened()){
        return -1;
    }

    fs["RM1x"] >> RM1x;
    fs["RM1y"] >> RM1y;
    fs["RM2x"] >> RM2x;
    fs["RM2y"] >> RM2y;

    fs.release();

 
    while (true){
        Mat frame1, frame2;
        Mat frame1_remap, frame2_remap;
        Mat stack;
        int status;

        status = cam_left.read(frame1);

        if(status){
            continue;
        }

        status = cam_right.read(frame2);

        if(status){
            continue;
        }

        remap(frame1, frame1_remap, RM1x, RM1y, INTER_LANCZOS4, BORDER_CONSTANT, Scalar(0, 0, 0));
        remap(frame2, frame2_remap, RM2x, RM2y, INTER_LANCZOS4, BORDER_CONSTANT, Scalar(0, 0, 0));

        hconcat(frame1_remap, frame2_remap, stack);

        imshow("img",stack);
        waitKey(1);
    }

    return 0;
}