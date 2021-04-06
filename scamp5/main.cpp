//
// Created by Jamal on 20/02/2021.
//

#include <simulator/util/utility.h>

#include "scamp5.h"

int main() {
    SCAMP5 s = SCAMP5::builder{}.build();
    UI ui;
    ui.start();

    while(true) {
        s.get_image(s.A, s.D);
        int e1 = cv::getTickCount();
        s.superpixel_create(s.R5, s.A);
        s.superpixel_shift_left(s.R6, s.R5);
        s.superpixel_shift_right(s.R7, s.R5);
        s.superpixel_dac(s.B, 0, s.R6);
        s.superpixel_dac(s.C, 0, s.R7);
//        s.movx(s.B, s.A, south);
//        s.add(s.B, s.B, s.A);
//        s.movx(s.A, s.B, north);
//        s.addx(s.B, s.B, s.A, east);
//        s.sub2x(s.A, s.B, west, west, s.B);
        int e2 = cv::getTickCount();
//        std::cout << ((e2 - e1) / cv::getTickFrequency()) * 1000 << " ms"
//                  << std::endl;
        ui.display_reg(*s.A);
        ui.display_reg(*s.B);
        ui.display_reg(*s.C);
        ui.display_reg(*s.R5);
        ui.display_reg(*s.R6);
        ui.display_reg(*s.R7);
//        utility::display_register<int8_t>("ORIGINAL", *s.A);
//        utility::display_register<int8_t>("SHIFT_LEFT", *s.B);
//        utility::display_register<int8_t>("SHIFT_RIGHT", *s.C);
//        cv::waitKey(1);
    }

    return 0;
}
