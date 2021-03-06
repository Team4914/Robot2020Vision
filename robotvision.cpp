/* Uses edges and colour thresholding to determine ROI.
*  
*  2020 - 12 - 10
*/
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

#define vid1 "video7_Trim.mp4"
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
    VideoWriter video("outcpp.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, tempsrc.size(), true);
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
        inRange(dst, Scalar(0, 70, 0), Scalar(130, 255, 10), result);
        //std::cout << result.type() << std::endl;
        //imshow("dst", mask1);
        
        Mat roiDetection;
        roiDetection = result;
        int counter = 0;
        int top = NULL, bottom = 0, right = 0, left = roiDetection.size().width;
        for (int i = 0; i < roiDetection.rows; i++) {
            for (int j = 0; j < roiDetection.cols; j++) {

                if ((int)roiDetection.at<uchar>(i, j) > 0) {
                    top = (top == NULL) ? i : top;
                    bottom = i;
                    if (right < j) right = j;
                    if (left > j) left = j;
                    counter++;
                }

            }
        }
        //Draw box around target on src image
        Rect ROI(Point(right, bottom), Point(left, top));
        rectangle(src, ROI.tl(), ROI.br(), Scalar(0,0,255), 5, 8, 0);
        /*
        std::cout << "Number of point: " << counter << std::endl;
        std::cout << "Top: " << top << std::endl;
        std::cout << "Bottom: " << bottom << std::endl;
        std::cout << "Left: " << left << std::endl;
        std::cout << "Right: " << right << std::endl;
        */
        //debugging
        //std::cout << frameNumber << std::endl;
        //frameNumber++;

        //record frame
        video.write(src);
        
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
