//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_LIVE_INPUT_H
#define SIMULATOR_LIVE_INPUT_H


#include "input_source.h"
#include <opencv2/videoio.hpp>

class LiveInput : public InputSource {
private:
    std::unique_ptr<cv::VideoCapture> capture;
    std::unique_ptr<cv::Size> size;
public:
    LiveInput(int rows, int cols);
    void read(Register& reg) override;
    void reset() override;
    double last_frame_time() override;
};


#endif //SIMULATOR_LIVE_INPUT_H