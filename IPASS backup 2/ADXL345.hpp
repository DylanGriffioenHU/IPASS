#ifndef ADXL345_HPP
#define ADXL345_HPP

#include "hwlib.hpp"
#include "i2c_ipass.hpp"

class ADXL345 : public i2c_ipass {
private:
    uint8_t device_id;
    int x_offset;
    int y_offset;
    int z_offset;
    bool start_in_measure_mode;
    
public:
    ADXL345(const hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus, const uint8_t & device_id, const int & x_offset, const int & y_offset, const int & z_offset);
    void setup(const bool & start_in_measure_mode);
    void set_measuring_mode();
    void set_standby_mode();
    int16_t read_axis_raw(const uint8_t & axis_register_address_1, const uint8_t & axis_register_address_2);
    int read_axis_2g(const uint8_t & axis_register_address_1, const uint8_t & axis_register_address_2);
    int* read_all_axis_2g(int axis_data[3]);
};

#endif