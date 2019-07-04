#ifndef HELPER_HPP
#define HELPER_HPP

void bitPrint(uint8_t a){
    for (int8_t i = 7; i >= 0; --i){
        hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
        if(i == 4){hwlib::cout << ' ';}
    }
    hwlib::cout << hwlib::endl;
}

#endif