//
// Created by jm1417 on 28/01/2021.
//

#include "analogue_register.h"
#include "../memory/si/si.h"
#include "../base/array.h"
#include "../metrics/stats.h"

AnalogueRegister::AnalogueRegister(int rows, int columns)
        : Register(rows, columns, MAT_TYPE, SI()) {}

Data AnalogueRegister::read() {
    this->inc_read();
    return this->value();
}

void AnalogueRegister::write(Data data) {
    this->value().setTo(data);
    this->inc_write();
}

void AnalogueRegister::write(int data) {
    this->value().setTo(data);
    this->inc_read();
}

AnalogueRegister &AnalogueRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

void AnalogueRegister::print_stats(const CycleCounter &counter) {

}

void AnalogueRegister::write_stats(const CycleCounter &counter, json &j) {
    double runtime = counter.to_seconds(stats::CLOCK_RATE);
    j[this->name_] =
            {{"Energy (J)",
                     {
                             {"Reads", this->get_read_energy()},
                             {"Writes", this->get_write_energy()},
                             {"Total", this->get_total_energy()},
                     }},
             {"Power (W)",
                     {
                             {"Reads", this->get_read_energy() / runtime},
                             {"Writes", this->get_write_energy() / runtime},
                             {"Total", this->get_total_energy() / runtime},
                     }
             },
             {"Accesses",
                     {
                             {"Reads", this->get_reads()},
                             {"Writes", this->get_writes()}
                     }

             }
            };
}
