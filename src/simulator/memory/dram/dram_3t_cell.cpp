//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram_3t_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

Dram3tCell::Dram3tCell(int rows, int cols, int row_stride, int col_stride, Config& config) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    static_power_(fun_static(config)),
    dynamic_read_power_(fun_dynamic_read(config)),
    dynamic_write_power_(fun_dynamic_write(config)),
    config_(&config),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(transistor_count_)),
    array_static_power_(rows, cols, CV_32F, cv::Scalar(0)),
    array_dynamic_power_(rows, cols, CV_32F, cv::Scalar(0)) {
    this->fun_internal_mask(rows, cols, row_stride, col_stride);
}

double Dram3tCell::fun_static(const Config& config) {
    return 2.17e-8;  // TODO find better numbers
}

double Dram3tCell::fun_dynamic_read(const Config& config) {
    return 5.8180745e-7;
}

double Dram3tCell::fun_dynamic_write(const Config& config) {
    return 2.4934605e-7;
}

void Dram3tCell::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for (int row = 0; row < rows; row += row_stride) {
        for (int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
}

cv::Mat& Dram3tCell::get_static_power() {
    return array_static_power_;
}

cv::Mat& Dram3tCell::get_dynamic_power() {
    return array_dynamic_power_;
}

cv::Mat& Dram3tCell::get_transistor_count() {
    return array_transistor_count_;
}

int Dram3tCell::get_cycle_count() {
    return cycle_count_;
}

void Dram3tCell::read(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_power_, this->dynamic_read_power_, this->array_dynamic_power_, scratch);
}

void Dram3tCell::read() {
    this->read(cv::noArray());
}

void Dram3tCell::write(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_power_, this->dynamic_write_power_, this->array_dynamic_power_, scratch);
}

void Dram3tCell::write() {
    this->write(cv::noArray());
}

void Dram3tCell::update(double time) {
    cv::add(this->array_static_power_, this->static_power_ * time, this->array_static_power_);
}