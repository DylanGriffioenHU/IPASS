#ifndef LINE_HPP
#define LINE_HPP

class line : public drawable {
protected:

   hwlib::xy end;
   
public:

    line( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end, const hwlib::xy & moving_cube_angle):
      drawable( w, location, end - location , moving_cube_angle),
      end( end )
    {}
   
   
    void draw() override {
      hwlib::line x( location, end );
      x.draw( w );;
    }
};

#endif