#ifndef OLED_IPASS_HPP
#define OLED_IPASS_HPP

class oled_ipass {
private:
    hwlib::terminal_from & display;
    
public:
    oled_ipass(auto & display):
        display(display)
    {}
    
    
    void print_axis_to_oled(auto & x_axis, auto & y_axis, auto & z_axis){
        
        display 
             << "\f" << "X: " << x_axis 
             << "\n" 
             << "\n" << "Y: " << y_axis
             << "\n"  
             << "\n" << "Z: " << z_axis 
             << hwlib::flush;
    }
};

#endif