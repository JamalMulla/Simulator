//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PIXEL_H
#define SIMULATOR_PIXEL_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "config.h"
#include "simulator/base/component.h"
#include "simulator/input/input_source.h"
#include "simulator/registers/analogue_register.h"

class Pixel : public Component {
    RTTR_ENABLE(Component);
   private:
    std::string path_;
    std::shared_ptr<InputSource> input_source;
    int camera_index_ = 0;

#ifdef TRACK_STATISTICS
    int cycle_count_;
#endif

   public:
    Pixel() = default;
    Pixel(int rows, int cols, int row_stride, int col_stride, Source src, const std::string& path, std::shared_ptr<Config> config);
    void init();

    void set_src(Source src);
    void set_path(const std::string& path);
    void set_camera_index(int camera_index);

    void reset();
    void read(Register& reg);
    cv::Mat read();
    double last_frame_time();
#ifdef TRACK_STATISTICS
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_width() override;
    double calc_height() override;
    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_PIXEL_H
