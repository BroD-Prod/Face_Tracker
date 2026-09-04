#include <iostream>

void calibrateBaseline(double& baselineRatio, int& calibrationFrames, int calibrationTarget, double mouthRatio){
    if(calibrationFrames < calibrationTarget){
        baselineRatio += mouthRatio;
        calibrationFrames++;
        if (calibrationFrames == calibrationTarget) {
            baselineRatio /= calibrationTarget;
        }
    } else if (mouthRatio > baselineRatio * 1.15) {
            std::cout << "Smiling" << std::endl;
        }
}