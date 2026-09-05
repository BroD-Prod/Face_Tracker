#include <iostream>

void calibrateBaseline(double& baselineRatio, int& calibrationFrames, int calibrationTarget, double smoothedRatio){
    if(calibrationFrames < calibrationTarget){
        baselineRatio += smoothedRatio;
        calibrationFrames++;
        if (calibrationFrames == calibrationTarget) {
            baselineRatio /= calibrationTarget;
        }
    } else if (smoothedRatio > baselineRatio * 2.5) {
            std::cout << "Smiling" << std::endl;
        }
}