//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/pixel.h"

#include <simulator/input/image_input.h>
#include <simulator/input/live_input.h>
#include <simulator/input/video_input.h>

#include <opencv4/opencv2/imgproc.hpp>

#include <rttr/registration>

void Pixel::init() {
#ifdef TRACK_STATISTICS
    process_node_ = 180;
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_power_ = calc_dynamic();
    width_ = calc_width();
    height_ = calc_height();
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));

    this->calc_internal_mask();
#endif
}

void Pixel::set_path(const std::string& path) {
    this->path_ = path;
}

void Pixel::set_src(Source src) {
    switch(src) {
        case LIVE: {
            input_source = std::make_shared<LiveInput>(rows_, cols_);
            break;
        }
        case VIDEO: {
            input_source = std::make_shared<VideoInput>(rows_, cols_, path_);
            break;
        }
        case IMAGE: {
            input_source = std::make_shared<ImageInput>(rows_, cols_, path_);
            break;
        }
    }
}

void Pixel::reset() { input_source->reset(); }

cv::Mat Pixel::read() {
    cv::Mat m = input_source->read();
#ifdef TRACK_STATISTICS
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->get_clock_rate();
    cv::add(this->array_dynamic_energy_, this->dynamic_power_, this->array_dynamic_energy_, this->internal_mask);
#endif
    return m;
}

void Pixel::read(Register& reg) {
    input_source->read(reg);
#ifdef TRACK_STATISTICS
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->get_clock_rate();
    cv::add(this->array_dynamic_energy_, this->dynamic_power_, this->array_dynamic_energy_, this->internal_mask);
#endif
}

double Pixel::last_frame_time() {
    return input_source->last_frame_time();
}

#ifdef TRACK_STATISTICS

int Pixel::get_cycle_count() {
    return cycle_count_;
}

void Pixel::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Pixel::print_stats(const CycleCounter& counter) {

}

int Pixel::calc_transistor_count() {
    return 4;
}

double Pixel::calc_static() {
    return 0;
}

double Pixel::calc_dynamic() {
    return 0;
}

double Pixel::calc_width() {
    return this->scale_width(6);
}

double Pixel::calc_height() {
    return this->scale_height(9);
}

#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Pixel>("Pixel")
        .constructor()
        .method("init", &Pixel::init)
        .method("set_path", &Pixel::set_path)
        .method("set_src", &Pixel::set_src);
};

