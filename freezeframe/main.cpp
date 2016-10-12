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
//   p     - toggle pause between frames
//   v     - toggle vertical flip
//   h     - toggle horizontal flip

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cerr << "Usage: ./freeze <save dir> <video>\n";
        return -1;
    }

    char dateString[16];
    auto as_time_t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    strftime(dateString, 16, "%Y%m%d/%H%M%S", localtime(&as_time_t));
    string filepath = string(argv[1]) + string(dateString);
    string command = "mkdir -p " + filepath;
    
    if (0 > system(command.c_str()))
//    if (0 > mkdir(filepath.c_str(), S_IRWXU|S_IROTH|S_IXOTH))
    {
        cerr << "Could not create local frame dir!\n";
        return -1;
    }

    cout << "Saving all captures to " + filepath << endl;

    VideoCapture cap(argv[2]);
    if (!cap.isOpened())
    {
        cerr << "Could not open video!\n";
        return -1;
    }
            
    namedWindow("window", WINDOW_AUTOSIZE);

    bool flipHorizontal = false;
    bool flipVertical = false;
    unsigned int saveNum = 0;
    int waitMS = 0;
    
    for (;;)
    {
        Mat frame;
        cap >> frame;

        if (flipHorizontal) flip(frame, frame, 0);
        if (flipVertical) flip(frame, frame, 1);
        
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
        case 'p':
            waitMS = 50 - waitMS;
            break;
        case 'h': // flip horizontal
            flipHorizontal = !flipHorizontal;
            break;
        case 'v':
            flipVertical = !flipVertical;
            break;
        case ' ':
        default:
            break;
        }
        saveNum++;
    }
    
    return 0;
}
