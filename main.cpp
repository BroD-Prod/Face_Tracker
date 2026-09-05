#include <iostream>
#include <queue>
#include <deque>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <numeric>
using namespace cv;
 
void calibrateBaseline(double& baselineRatio, int& calibrationFrames, int calibrationTarget, double smoothedRatio);

int main()
{
    cv::VideoCapture camera(0, CAP_AVFOUNDATION);
    if (!camera.isOpened())
    {
        std::cerr << "Error: Could not open video capture" << std::endl;
        return 1;
    }

    Ptr<FaceDetectorYN> faceDetect = FaceDetectorYN::create(
        "models/face_detection_yunet_2026may.onnx",
        "",
        Size(1920, 1080),
        0.5f,
        0.1f,
        5000);
    if (faceDetect.empty())
    {
        std::cerr << "Error: Could not create FaceDetectorYN" << std::endl;
        return 1;
    }
    cv::Mat frame;
    cv::Mat smile;

    double baselineRatio = 0.0;
    int calibrationFrames = 0;
    const int CalibrationTarget = 60;  

    std::deque<double> ratioHistory;
    const size_t maxHistorySize = 100;

    while (true)
    {
        camera >> frame;
        if (frame.empty())
        {
            break;
        }

        cv::Mat faces;
        faceDetect->detect(frame, faces);

        for (int i = 0; i < faces.rows; i++)
        {
            // Calculate Facial Points
            Rect2f face(faces.at<float>(i, 0), faces.at<float>(i, 1),
                        faces.at<float>(i, 2), faces.at<float>(i, 3));
            Point2f rightEye(faces.at<float>(i, 4), faces.at<float>(i, 5));
            Point2f leftEye(faces.at<float>(i, 6), faces.at<float>(i,7));
            Point2f noseTip(faces.at<float>(i, 8), faces.at<float>(i,9));
            Point2f mouthRight(faces.at<float>(i, 10), faces.at<float>(i, 11));
            Point2f mouthLeft(faces.at<float>(i, 12), faces.at<float>(i, 13));
            
            // Calculate Mouth Movements
            double mouthWidth = norm(mouthRight - mouthLeft);
            double eyeDistance = norm(rightEye - leftEye);
            double mouthRatio = mouthWidth / eyeDistance;

            std::cout << "Mouth Ratio:" << mouthRatio << std::endl;

            // Create a history of mouth ratios to improve stability of detection
            ratioHistory.push_back(mouthRatio);
            if(ratioHistory.size() > maxHistorySize){
                ratioHistory.pop_front();
            }

            double smoothedRatio = std::accumulate(ratioHistory.begin(), ratioHistory.end(), 0.0) / ratioHistory.size();

            calibrateBaseline(baselineRatio, calibrationFrames, CalibrationTarget, smoothedRatio);
            
            // Draw Facial Points
            rectangle(frame, face, Scalar(0, 255, 0), 2);
            circle(frame, rightEye, 3, Scalar(0, 0, 255), -1);
            circle(frame, leftEye, 3, Scalar(0, 0, 255), -1);
            circle(frame, noseTip, 3, Scalar(0, 0, 255), -1);
            circle(frame, mouthRight, 3, Scalar(0, 0, 225), -1);
            circle(frame, mouthLeft, 3, Scalar(0, 0, 225), -1);
        }

        imshow("Webcam", frame);
        if (waitKey(30) == 27)
        {
            break; // Esc to quit
        }
    }
    return 0;
}