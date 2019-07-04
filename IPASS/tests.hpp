#ifndef TESTS_HPP
#define TESTS_HPP

#include "i2c_ipass.hpp"
#include "ADXL345.hpp"
#include "registers.hpp"

class tests {
private: 
    i2c_ipass i2c_ipass_object;
    ADXL345 ADXL345_object;
    
public:
    tests(const i2c_ipass & i2c_ipass_object, const ADXL345 & ADXL345_object);
    
    bool test_i2c_ipass_read();
    bool test_i2c_ipass_write();
    bool test_ADXL345_measuring();
    bool test_ADXL345_set_standby_mode();
    void print_test_results();
};

#endif