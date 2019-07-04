#include "hwlib.hpp"
#include "registers.hpp"
#include "helper.hpp"
#include "i2c_ipass.hpp"
#include "ADXL345.hpp"
#include "oled_text.hpp"
#include "tests.hpp"

 
int main( void ){
    
    namespace target = hwlib::target;
 
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
 
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    
    auto btn1 = hwlib::target::pin_in( hwlib::target::pins::d22 );
    auto btn2 = hwlib::target::pin_in( hwlib::target::pins::d24 );

    i2c_ipass i2c_ipass_object(i2c_bus);

    ADXL345 accelerometer(i2c_bus, 0x53, -5, 4, 8);
    
    auto oled    = hwlib::glcd_oled( i2c_bus, 0x3c );
    auto font    = hwlib::font_default_8x8();
    auto display = hwlib::terminal_from( oled, font );
    
    oled_ipass oled_object(display);
    
    tests test_object(i2c_ipass_object, accelerometer);
    
    test_object.print_test_results();
    
    accelerometer.setup(1);
    

 
    for(;;){
        if(btn1.read()){
            accelerometer.set_measuring_mode();
            hwlib::wait_ms(500);
        } else if (btn2.read()){
            accelerometer.set_standby_mode();
            hwlib::wait_ms(500);
        }
        
        
        int axis_data[3];
        
        accelerometer.read_all_axis_2g(axis_data);
        
        oled_object.print_axis_to_oled(axis_data[0], axis_data[1], axis_data[2]);
    }
}
