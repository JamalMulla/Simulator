//
// Created by jm1417 on 22/12/2020.
//

#include "ProcessingElement.h"


ProcessingElement::ProcessingElement() {
    IN = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    PIX = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    A = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    B = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    C = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    D = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    E = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    F = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    NEWS = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    XN = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    XE = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    XS = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    XW = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    FLAG = cv::UMat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U, 255);
}

void ProcessingElement::pushToNews(AREG& src, news_t dir) {
    switch (dir) {
        case east: {
            auto chunk = cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            src(cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case west: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            src(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1),FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case north: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            src(cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, SCAMP_HEIGHT - 1, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1),FLAG(cv::Rect(0, SCAMP_HEIGHT - 1, SCAMP_WIDTH,1)));
            break;
        }
        case south: {
            auto chunk = cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            src(cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        default:
            break;
    }
}


void ProcessingElement::pullFromNews(AREG& dst, news_t dir) {
    switch (dir) {
        case east: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            auto n = NEWS(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT));
            n.copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case west: {
            auto chunk = cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case north: {
            auto chunk = cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(0, 0, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        case south: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            NEWS(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        default:
            break;
    }
}

// Misc

void ProcessingElement::nop() {}

// Image Capturing
void ProcessingElement::rpix() {
    //reset PIX
}

void ProcessingElement::get_image(AREG &y) {
    //y := half-range image, and reset PIX
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void ProcessingElement::get_image(AREG &y, AREG &h) {
    //y := full-range image, h := negative half-range image, and reset PIX
    this->bus(NEWS, PIX);
    this->bus(h,PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, h, NEWS, PIX);
}

void ProcessingElement::respix() {
    //reset PIX
    this->rpix();
    this->rpix();
    this->nop();
}

void ProcessingElement::respix(AREG &y) {
    //reset PIX, keep its reset level in y
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void ProcessingElement::getpix(AREG &y, AREG &pix_res) {
    //y := half-range image, supplying the reset level of PIX
    this->bus(NEWS,PIX);
    this->bus(y,NEWS, pix_res);
}

void ProcessingElement::getpix(AREG &y, AREG &h, AREG &pix_res) {
    //y := full-range, h := half-range image, supplying the reset level of PIX
    this->bus(h,PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

// Analog Register Transfer
void ProcessingElement::bus(AREG& a) {
    //a = 0 + error
    a.setTo(0.0, FLAG);
}

void ProcessingElement::bus(AREG& a, const AREG& a0) {
    //a = -a0 + error
    cv::bitwise_not(a0, a, FLAG);
}

void ProcessingElement::bus(AREG& a, const AREG& a0, const AREG& a1) {
    //a = -(a0 + a1) + error
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    cv::bitwise_not(intermediate, a, FLAG);
}

void ProcessingElement::bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2) {
    //a = -(a0 + a1 + a2) + error
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    cv::add(intermediate, a2, intermediate, FLAG);
    cv::bitwise_not(intermediate, a, FLAG);
}

void ProcessingElement::bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2, const AREG& a3) {
    //a = -(a0 + a1 + a2 + a3) + error
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    cv::add(intermediate, a2, intermediate, FLAG);
    cv::add(intermediate, a3, intermediate, FLAG);
    cv::bitwise_not(intermediate, a, FLAG);
}

void ProcessingElement::bus2(AREG& a, AREG& b) {
    //a,b = 0 + error
    a.setTo(0.0, FLAG);
    b.setTo(0.0, FLAG);
}

void ProcessingElement::bus2(AREG& a, AREG& b, const AREG& a0) {
    //a,b = -0.5*a0 + error + noise
    AREG intermediate;
    cv::divide(2, a0, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG);
    intermediate.copyTo(a, FLAG);
    intermediate.copyTo(b, FLAG);
}

void ProcessingElement::bus2(AREG& a, AREG& b, const AREG& a0, const AREG& a1) {
    //a,b = -0.5*(a0 + a1) + error + noise
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    cv::divide(2, intermediate, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG);
    intermediate.copyTo(a, FLAG);
    intermediate.copyTo(b, FLAG);
}

void ProcessingElement::bus3(AREG& a, AREG& b, AREG& c, const AREG& a0) {
    //a,b,c = -0.33*a0 + error + noise
    AREG intermediate;
    cv::divide(3, a0, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG);
    intermediate.copyTo(a, FLAG);
    intermediate.copyTo(b, FLAG);
    intermediate.copyTo(c, FLAG);
}

void ProcessingElement::where(const AREG& a) {
    //FLAG := a > 0.
    AREG intermediate;
    cv::threshold(a, intermediate, 0, 255, cv::THRESH_BINARY);
    intermediate.convertTo(FLAG, CV_8U, 2, 1);
}

void ProcessingElement::where(const AREG& a0, const AREG& a1) {
    //FLAG := (a0 + a1) > 0.
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    threshold(intermediate, intermediate, 0, 255, cv::THRESH_BINARY);
    intermediate.convertTo(FLAG, CV_8U, 2, 1);
}

void ProcessingElement::where(const AREG& a0, const AREG& a1, const AREG& a2) {
    //FLAG := (a0 + a1 + a2) > 0.
    AREG intermediate;
    cv::add(a0, a1, intermediate, FLAG);
    cv::add(intermediate, a2, intermediate, FLAG);
    threshold(intermediate, intermediate, 0, 255, cv::THRESH_BINARY);
    intermediate.convertTo(FLAG, CV_8U, 2, 1);
}

void ProcessingElement::all() {
    //FLAG := 1.
    this->FLAG.setTo(1);
}

void ProcessingElement::mov(AREG& y, const AREG& x0) {
    //y = x0
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

// Analog Arithmetic
void ProcessingElement::res(AREG& a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void ProcessingElement::res(AREG& a, AREG& b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void ProcessingElement::add(AREG& y, const AREG& x0, const AREG& x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void ProcessingElement::add(AREG& y, const AREG& x0, const AREG& x1, const AREG& x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void ProcessingElement::sub(AREG& y, const AREG& x0, const AREG& x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void ProcessingElement::neg(AREG& y, const AREG& x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void ProcessingElement::abs(AREG& y, const AREG& x0) {
    // y = |x0|
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void ProcessingElement::div(AREG& y0, AREG& y1, AREG& y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::div(AREG& y0, AREG& y1, AREG& y2, const AREG& x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2,NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::diva(AREG& y0, AREG& y1, AREG& y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void ProcessingElement::divq(AREG& y0, const AREG& x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

// Analog Neighbour Access
//TODO Neighbours are moved into NEWS
void ProcessingElement::movx(AREG& y, const AREG& x0, const news_t dir) {
    // y = x0_dir

    //TODO What is write on read??
    AREG intermediate;
    //std::cout << "x0: " << x0 << std::endl;
    this->bus(intermediate, x0);
    //std::cout << "Intermediate: " << intermediate << std::endl;
    this->pushToNews(intermediate, dir);
    //std::cout << "NEWS: " << NEWS << std::endl;
    this->bus(y, NEWS);
    //std::cout << "Y: " << y << std::endl;
}

void ProcessingElement::mov2x(AREG& y, const AREG& x0, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir (note: this only works when FLAG is "all")
    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    //std::cout << "Y: " << x0 << std::endl;
    this->bus(intermediate, x0);
//    std::cout << "int: " << intermediate << std::endl;
//    std::cout << "NEWS before: " << NEWS << std::endl;
    this->pushToNews(intermediate, dir);
//    std::cout << "NEWS: " << NEWS << std::endl;
    this->pullFromNews(intermediate2, dir2);
//    std::cout << "NEWS: " << NEWS << std::endl;
    this->bus(y, intermediate2);
//    std::cout << "Y: " << y << std::endl;

}

void ProcessingElement::addx(AREG& y, const AREG& x0, const AREG& x1, const news_t dir) {
    // y = x0_dir + x1_dir
    AREG intermediate;
    this->bus(intermediate, x0, x1);
    this->pushToNews(intermediate, dir);
    this->bus(y, NEWS);
}

void ProcessingElement::add2x(AREG& y, const AREG& x0, const AREG& x1, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir2 + x1_dir_dir2

    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);

    this->bus(intermediate, x0, x1);
    this->pushToNews(intermediate, dir);
    this->pullFromNews(intermediate2, dir2);
    this->bus(y, intermediate2);
}

void ProcessingElement::subx(AREG& y, const AREG& x0, const news_t dir, const AREG& x1) {
    // y = x0_dir - x1
    AREG intermediate;
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->bus(y, NEWS, x1);
}

void ProcessingElement::sub2x(AREG& y, const AREG& x0, const news_t dir, const news_t dir2, const AREG& x1) {
    // y = x0_dir_dir2 - x1
    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->pullFromNews(intermediate2, dir2);
    this->bus(y, intermediate2, x1);
}

// Asynchronized Blur

// Digital Logic Operations
void ProcessingElement::OR(DREG d, DREG d0, DREG d1) {

}

void ProcessingElement::OR(DREG d, DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::OR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::NOT(DREG d, DREG d0) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::NOT(DREG Rl) {
    // logic operation Rl := NOT Rl
    //Rl.set(not Rl.get());
}

void ProcessingElement::OR(DREG Rl, DREG Rx) {
    // logic operation Rl := Rl OR Rx
    //Rl.set(Rl.get() or Rx.get());
}

void ProcessingElement::NOR(DREG Rl, DREG Rx) {
    // logic operation Rl := Rl NOR Rx
    //Rl.set(not (Rl.get() or Rx.get()));
}

void ProcessingElement::AND(DREG Ra, DREG Rx, DREG Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->NOR(Ra, RF, RP);
}

void ProcessingElement::NAND(DREG Ra, DREG Rx, DREG Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->OR(Ra, RF, RP);
}

void ProcessingElement::ANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, RF, Rb);
}

void ProcessingElement::NANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, RF, Rb);
}

void ProcessingElement::IMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    this->NOT(RF, Ry);
    this->OR(RS, Rx, RF);
}

void ProcessingElement::NIMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(RF, Ry);
    this->NOR(RS, Rx, RF);
}

void ProcessingElement::XOR(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(RF, Ry);
    this->NOR(Rl, Rx, RF);
    this->NOT(RF, Rx);
    this->NOR(Rx, Ry, RF);
    this->OR(Rl, Rx);
}

// Digital Register Transfer
void ProcessingElement::WHERE(DREG d) {

}

void ProcessingElement::WHERE(DREG d0, DREG d1) {

}

void ProcessingElement::WHERE(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::ALL() {

}

void ProcessingElement::SET(DREG d0) {

}

void ProcessingElement::SET(DREG d0, DREG d1) {

}

void ProcessingElement::SET(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::SET(DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::CLR(DREG d0) {

}

void ProcessingElement::CLR(DREG d0, DREG d1) {

}

void ProcessingElement::CLR(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::CLR(DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::MOV(DREG d, DREG d0) {

}

void ProcessingElement::MUX(DREG Rl, DREG Rx, DREG Ry, DREG Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    this->SET(RF);
    this->MOV(RP, Rz);
    this->MOV(RF, Rx);
    this->MOV(RP, Ry);
    this->MOV(Rl, RP);
}

void ProcessingElement::CLR_IF(DREG Rl, DREG Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    this->NOT(RF, Rl);
    this->NOR(Rl, RF, Rx);
}

void ProcessingElement::REFRESH(DREG Rl) {
    // refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds) without any operations
    //TODO do we need to do anything here? Maybe add a small delay?
}

// Digital Neighbour Access
void ProcessingElement::DNEWS0(DREG d, DREG d0) {

}

void ProcessingElement::DNEWS1(DREG d, DREG d0) {

}

void ProcessingElement::DNEWS(DREG Ra, DREG Rx, const int dir, const bool boundary) {
    this->CLR(RS, RW, RN, RE);
    // set multiple dreg (upto 4) can be done via one icw
    if(dir&south){
        this->SET(RS);
    }
    if(dir&west){
        this->SET(RW);
    }
    if(dir&north){
        this->SET(RN);
    }
    if(dir&east){
        this->SET(RE);
    }

    if(boundary){
        this->DNEWS1(Ra,Rx);
    }else{
        this->DNEWS0(Ra,Rx);
    }
}

// Digital Propagation
void ProcessingElement::PROP0() {
    // async-propagation on RP, masked by RF
}

void ProcessingElement::PROP1() {

}









