#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;


// Loop over a video file with options.
// Provided as is. Linux only.
// Commands:
//   SPACE - next frame
//   ESC   - exit
//   s     - save frame to <dir>
//   f     - no pause between frames
//   d     - pause between frames

int main(int argc, char** argv)
{
    char dateString[16];
    auto as_time_t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    strftime(dateString, 16, "%Y%m%d/%H%M%S", localtime(&as_time_t));
    string filepath = to_string(*argv[1]) + string(dateString);

    if (argc < 3)
    {
        cerr << "Usage: ./freeze <save dir> <video>\n";
        return -1;
    }

    if (0 > mkdir(filepath.c_str(), S_IRWXU|S_IROTH|S_IXOTH))
    {
        cerr << "Could not create local frame dir!\n";
        return -1;
    }

    VideoCapture cap(argv[2]);
    if (!cap.isOpened())
    {
        cerr << "Could not open video!\n";
        return -1;
    }
            
    namedWindow("window", WINDOW_AUTOSIZE);

    unsigned int saveNum = 0;
    int waitMS = -1;
    for (;;)
    {
        Mat frame;
        cap >> frame;
        imshow("window", frame);

        // Ubuntu-specific codes
        char c = waitKey(waitMS); 
        switch(c)
        {
        case 27:
            return 0;
        case 's':
            imwrite(filepath + "/frame" + to_string(saveNum) + ".png", frame);
            break;
        case 'f':
            waitMS = 50;
            break;
        case 'd':
            waitMS = -1;
            break;
        case ' ':
        default:
            break;
        }
        saveNum++;
    }
    
    return 0;
}
