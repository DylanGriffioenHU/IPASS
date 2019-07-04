#include "ADXL345.hpp"
#include "registers.hpp"
#include "i2c_ipass.hpp"

/// @file

/// \brief
/// This is the constructor for an ADXL345 object
/// \details
/// Example: ADXL345 accelerometer(i2c_bus, 0x53, -5, 4, 8, 1);
///
/// The first variable is an hwlib::i2c_bus_bit_banged_scl_sda object.
/// The second variable is the uint8_t device_id of the sensor
/// The third variable is an int for the x_offset register
/// The fourth variable is an int for the y_offset register
/// The fifth variable is an int for the z_offset register
/// Those variables are used to calibrate the sensor in the setup function
ADXL345::ADXL345(const hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus, const uint8_t & device_id, const int & x_offset, const int & y_offset, const int & z_offset):
        i2c_ipass(i2c_bus),
        device_id(device_id),
        x_offset(x_offset),
        y_offset(y_offset),
        z_offset(z_offset)
    {}


/// \brief
/// This function writes the offset data to the offset registers and can put the sensor in measure mode base on the start_in_measure_mode varible.
/// \details
/// Example: ADXL345_object.setup();
///
/// The offset variable and the start_in_measure_mode are stored inside the object itsels so there is no need to give them to the function as variable.
/// It writes the offset variable to the correct register so that the sensor is calibrated correctly whenever this function is called.
/// And if set_in_measure_mode is true it also executes the set_measuring_mode function to put the device straight into measure mode.
void ADXL345::setup(const bool & start_in_measure_mode){
    write(OFSX, device_id, x_offset);
    write(OFSY, device_id, y_offset);
    write(OFSZ, device_id, z_offset);
    if(start_in_measure_mode){
        write(POWER_CTL, device_id, 8);
    }
}

/// \brief
/// This function wrytes a byte(00001000) to the POWER_CTL register to put the sensor in measure mode by making bit D3 high.
/// \details
/// Example: ADXL345_object.set_measuring_mode();
///
/// There is no need to give it any variable since this function will always turn on the same bit in the same register.
/// You can also skip this function and use the i2c_ipass write directly by using i2c_ipass.write(POWER_CTL, device_id, 8);
void ADXL345::set_measuring_mode(){
    write(POWER_CTL, device_id, 8);
}

/// \brief
/// This function sets the sensor back in standby mode by clearing bit D3 in the POWER_CTL register
/// \details
/// Example: ADXL345_object.set_standby_mode();
///
/// It requires no variable since it always clears the same bit.
///
/// This function only wants to set bit D3 low
/// So in order to not touch any of the other bits it reads the current data from the register.
/// It then ands that with 11110111 ensuring that the that aren't D3 and are set stay set.
void ADXL345::set_standby_mode(){
    uint8_t old_byte = read(POWER_CTL, device_id);
    uint8_t new_byte = (old_byte & 247);
    write(POWER_CTL, device_id, new_byte);
}


/// \brief
/// The data for the axis are stored in 2 registers, this function reads both and returns that data.
/// \details
/// Example read_axis_2g(DATAX0,DATAX1);
///
/// Since the data for the axis are stored in 2 registers the function requires them both as variable.
/// Both are const uint8_t variable.
/// It returns an int16-t variable which contains the combined data from the 2 registers.
int16_t ADXL345::read_axis_raw(const uint8_t & axis_register_address_1, const uint8_t & axis_register_address_2){
    auto byte_0 = read(axis_register_address_1, device_id);
    auto byte_1 = read(axis_register_address_2, device_id);
    int16_t byte = ( byte_0 | byte_1 << 8);
    return byte;
}

/// \brief
/// This function does the same as the read_axis_raw function but converts the data to -+2g.
/// \details
/// Since the data for the axis are stored in 2 registers the function requires them both as variable.
/// Both are const uint8_t variable.
/// It returns an intvariable which contains the combined data from the 2 registers but is also converted to -+2g.
/// 
/// Hwlib cannot print floats therefore the data has been multiplied with 100 so that instead of -1.00 to 1.00 we have -100 to 100
int ADXL345::read_axis_2g(const uint8_t & axis_register_address_1, const uint8_t & axis_register_address_2){
    auto byte_0 = read(axis_register_address_1, device_id);
    auto byte_1 = read(axis_register_address_2, device_id);
    int16_t byte = ( byte_0 | byte_1 << 8);
    auto return_data = (byte * 100) / 256;
    return return_data;
}

/// \brief
/// This function reads all 3 axis and returns an array containing the data from all 3 axis.
/// \details
/// Example: int axis_data[3];
/// Example: ADXL345.read_all_axis_2g(axis_data);
///
/// This function requires an int array that is 3 long.
/// It reads the 3 axis data and puts it in the array and returns that array.
/// Do keep in mind that this function uses the read_axis_2g function and not the raw data version
int* ADXL345::read_all_axis_2g(int axis_data[3]){
    axis_data[0] = read_axis_2g(DATAX0,DATAX1);
    axis_data[1] = read_axis_2g(DATAY0,DATAY1);
    axis_data[2] = read_axis_2g(DATAZ0,DATAZ1);
    return axis_data;
}