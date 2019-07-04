#include "tests.hpp"

/// \brief
/// Constructor for a tests object
/// \details
/// The constructor requires an i2c_ipass object and an ADXL345_object
tests::tests(const i2c_ipass & i2c_ipass_object, const ADXL345 & ADXL345_object):
        i2c_ipass_object(i2c_ipass_object),
        ADXL345_object(ADXL345_object)
    {}
    

/// \brief
/// Tests the read function from the i2c_ipass class.
/// \details
/// The 0x00 register from the ADXL345 module holds the device id which is 0xE5 or 229.
/// By using that value we can check if the data that the read function returns is what's supposed to be in the register.
bool tests::test_i2c_ipass_read(){
    int read_data = i2c_ipass_object.read(0x00, 0x53);
    if(read_data == 229){
        return true;
    }
    return false;
}

/// \brief
/// Tests the write function form the i2c_ipass class.
/// \details
/// We tested the read function in the previous test so we can use that to check our write function.
/// For this test the function writes 8 to the z offest register so if we read that register it should containt the value 8.
/// Daarna zetten we de byte weer op 0 om te voorkomen dat het ergens anders problemen veroorzaakt.
bool tests::test_i2c_ipass_write(){
    i2c_ipass_object.write(0x20, 0x53, 8);
    int read_data = i2c_ipass_object.read(0x20, 0x53);
    if(read_data == 8){
        i2c_ipass_object.write(0x20, 0x53, 0);
        return true;
    }
    i2c_ipass_object.write(0x20, 0x53, 0);
    return false;
}

/// \brief
/// Tests wether or not the sensor can start measuring
/// \details
/// The sensor starts in standby mode and untill it gets into measure mode the axis data is 0.
/// What this test does is that it first checks wether or not they are 0.
/// Then if all axis are 0 it uses the setup funciton to calibrate the sensor and put it in measure mode.
/// The setup function just contains a bunch of i2c_ipass write functions which we already tested.
/// Once the sensor has entered measure mode it starts to fill the axis registers with data.
/// Do keep in mind that when the sensor is hold upright the X and Y axis are 0 so in order for this funciton to work the sensor needs to be at an angle.
bool tests::test_ADXL345_measuring() {
    int axis_data[3];
    ADXL345_object.read_all_axis_2g(axis_data);
    hwlib::cout << hwlib::endl << "X: " << axis_data[0] << " Y: " << axis_data[1] << " Z: " << axis_data[2] << hwlib::endl;
    if((axis_data[0] == 0) && (axis_data[1] == 0) && (axis_data[2] == 0)){
        ADXL345_object.setup(1);
        hwlib::wait_ms(500);
        ADXL345_object.read_all_axis_2g(axis_data);
        hwlib::cout << "X: " << axis_data[0] << " Y: " << axis_data[1] << " Z: " << axis_data[2] << hwlib::endl;
        if((axis_data[0] != 0) && (axis_data[1] != 0) && (axis_data[2] != 0)){
            hwlib::cout << "Result: "; 
            return true;
        }
    }
    return false;
}

/// \brief
/// Test if the set_standby_mode function can return the sensor to standby mode without touching the other bits in the register.
/// \details
/// So the set_stanby_mode function is designed to only clear bit D3 which is the measure mode bit.
/// To test this we write 12 to the register which is 00001100.
/// Then after we exectute the set_standby_mode function it should clear bit D3 leaving 00000100 which is 4.
/// Then it writes 0 to the POWER_CTL register to ensure we don't leave any unwanted bits in there.
bool tests::test_ADXL345_set_standby_mode(){
    i2c_ipass_object.write(POWER_CTL, 0x53, 12);
    ADXL345_object.set_standby_mode();
    int read_data = i2c_ipass_object.read(POWER_CTL, 0x53);
    if(read_data == 4){
        i2c_ipass_object.write(POWER_CTL, 0x53, 0);
        return true;
    }
    i2c_ipass_object.write(POWER_CTL, 0x53, 0);
    return false;
}


void tests::print_test_results(){
    hwlib::cout << "Running tests" << hwlib::endl;
    hwlib::cout << "Test i2c_ipass read: " << test_i2c_ipass_read() << hwlib::endl;
    hwlib::cout << "Test i2c_ipass write: " << test_i2c_ipass_write() << hwlib::endl;
    hwlib::cout << "Test ADXL345 measuring: " << test_ADXL345_measuring() << hwlib::endl;
    hwlib::cout << "Test ADXL345 set standby mode: " << test_ADXL345_set_standby_mode() << hwlib::endl;
    hwlib::cout << "Finished running tests" << hwlib::endl;
}