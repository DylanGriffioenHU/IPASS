#ifndef I2C_IPASS_HPP
#define I2C_IPASS_HPP

#include "hwlib.hpp"

class i2c_ipass {
private:
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;

public:
    i2c_ipass(const hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus);
    
    void write(const uint8_t & register_address, const uint8_t & device_id, const uint8_t & data);
    uint8_t read(const uint8_t & register_address, const uint8_t & device_id);
    
};

#endif
