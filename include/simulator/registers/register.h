//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <simulator/ui/ui.h>

#include <opencv2/core/mat.hpp>

#include "simulator/base/component.h"
#include "simulator/memory/memory.h"

typedef cv::Mat Data;

class UI;

class Register : public Component {
   private:
    UI* ui = nullptr;

   protected:
    Memory memory_type_;
    Data value_;
#ifdef TRACK_STATISTICS
    cv::Mat read_counter;          // Number of reads for each PE
    cv::Mat write_counter;         // Number of writes
    cv::Mat read_energy_counter;   // Energy consumed by reads
    cv::Mat write_energy_counter;  // Energy consumed by writes
    int reads;   // Number of reads not per PE but across the array
    int writes;  // Number of writes not per PE but across the array
#endif
   public:
    std::string name_;
    int min_val;
    int max_val;

    Register(int rows, int columns, int type, Memory memoryType);

    Data& value();
    const Data& value() const;

    void change_memory_type(const Memory& memory_type);

#ifdef TRACK_STATISTICS
    void inc_read(const cv::_InputOutputArray& mask = cv::noArray());
    void inc_write(const cv::_InputOutputArray& mask = cv::noArray());

    int get_reads();
    int get_writes();

    int get_total_reads();
    int get_total_writes();

    double get_read_energy();
    double get_write_energy();
    double get_total_energy();

    double get_static_power(double time) override = 0;
    double get_dynamic_power() override = 0;
    int get_cycle_count() override = 0;
    int get_transistor_count() override = 0;
    void print_stats(const CycleCounter& counter) override = 0;
    void write_stats(const CycleCounter& counter, json& j) override = 0;
#endif

    void set_ui_handler(UI* ui_ptr);

    virtual Data read() = 0;
    virtual void write(Data data) = 0;
    virtual void write(int data) = 0;
};

#endif  // SIMULATOR_REGISTER_H
