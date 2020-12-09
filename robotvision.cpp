/* Uses edges and colour thresholding to determine ROI.
*  TO DO: Contour detection
*  2020 - 12 -09
*/
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#define vid1 "video5.mp4"
using namespace cv;
// Canny detection parameters
int cannythreshold = 30;
const int ratio = 3;
const int kernel_size = 3;

int main() {

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(vid1);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }
    //temp variable for getting video fram dimesions
    Mat tempsrc;
    //take one frame from the video
    cap >> tempsrc;
    // -- Video recording --
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
    // last arg is true if output video is in RGB
    VideoWriter video("outcpp.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, tempsrc.size(), false);
    int frameNumber = 1;
    while (1) {
        //dst - destination, detected_edges - image with canny output
        //src - source, src_gray - source in grayscale
        Mat dst, detected_edges;
        Mat src, src_gray;
        // Capture frame-by-frame
        cap >> src;

        // If the frame is empty, break immediately
        if (src.empty())
            break;
        //Canny detection processing
        dst.create(src.size(), src.type());
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        blur(src_gray, detected_edges, Size(3, 3));
        Canny(detected_edges, detected_edges, cannythreshold, cannythreshold * ratio, kernel_size);

        //Overlay the edges with the original image to get coloured edges
        dst = Scalar::all(0);
        src.copyTo(dst, detected_edges);
        //debugging
        //imshow("result", dst);
        
        //Colour detection
        Mat result;
        //Creating masks to detect the upper and lower green target color in BGR.
        inRange(dst, Scalar(0, 100, 0), Scalar(140, 255, 10), result);
        //imshow("dst", mask1);

        //debugging
        //std::cout << frameNumber << std::endl;
        //frameNumber++;

        //record frame
        video.write(mask1);

        // Press ESC on keyboard to exit
        char c = (char)waitKey(1);
        if (c == 27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();
    // Closes all the frames
    destroyAllWindows();
    
    return 0;
}
