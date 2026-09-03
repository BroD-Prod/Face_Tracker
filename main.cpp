#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;

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
            Rect2f face(faces.at<float>(i, 0), faces.at<float>(i, 1),
                        faces.at<float>(i, 2), faces.at<float>(i, 3));
            rectangle(frame, face, Scalar(0, 255, 0), 2);
        }

        imshow("Webcam", frame);
        if (waitKey(30) == 27)
        {
            break; // Esc to quit
        }
    }
    return 0;
}
