//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SI_CELL_H
#define SIMULATOR_SI_CELL_H

#include "simulator/memory/memory.h"

class SiCell : public Memory {
   private:
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    int transistor_count_ = 7;
    double static_power_;  // in Watts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    Config* config_;
    cv::Mat array_transistor_count_;
    cv::Mat array_static_power_;
    cv::Mat array_dynamic_power_;
    double fun_static(const Config& config);
    double fun_dynamic_read(const Config& config);
    double fun_dynamic_write(const Config& config);
    cv::Mat scratch;

   public:
    explicit SiCell(int rows, int cols, int row_stride, int col_stride, Config& config);

    void update(double time) override;
    int get_cycle_count() override;
    cv::Mat& get_static_power() override;
    cv::Mat& get_dynamic_power() override;
    cv::Mat& get_transistor_count() override;
    void read(const cv::_InputOutputArray& mask) override;
    void read() override;
    void write(const cv::_InputOutputArray& mask) override;
    void write() override;
};

#endif  // SIMULATOR_SI_CELL_H
