#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <hwlib.hpp>
#include <array>

// ===========================================================================

class drawable {
protected:

    hwlib::window & w;
    hwlib::xy location;
    hwlib::xy size;
    hwlib::xy moving_cube_angle = {1,1};
   
public:

    drawable( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & size ):
      w( w ),
      location( location ),
      size( size )
   {}  

   drawable( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & size, const hwlib::xy moving_cube_angle ):
      w( w ),
      location( location ),
      size( size ),
      moving_cube_angle(moving_cube_angle)
   {}      
   
   virtual void draw() = 0;
   virtual void update(){}
   bool overlaps( const drawable & other );   
   virtual void interact( drawable & other ){}
   
   hwlib::ostream & print( hwlib::ostream & out ) const {
      return out << location << " " << ( location + size );
   }

    hwlib::xy get_moving_cube_angle() {
        return moving_cube_angle;
    }
};

hwlib::ostream & operator<<( hwlib::ostream & lhs, const drawable & rhs ){
   return rhs.print( lhs );
}

bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

bool drawable::overlaps( const drawable & other ){
   
   bool x_overlap = within( 
      location.x, 
      other.location.x, 
      other.location.x + other.size.x
   ) || within( 
      other.location.x, 
      location.x, 
      location.x + size.x
   );
     
   bool y_overlap = within( 
      location.y, 
      other.location.y, 
      other.location.y + other.size.y
   ) || within( 
      other.location.y, 
      location.y, 
      location.y + size.y
   );
   
   return x_overlap && y_overlap;
}

// ===========================================================================

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

// ===========================================================================

class player : public line {
private:
    int speed = 0;
   
public:

    player( hwlib::window & w, const hwlib::xy & location, const hwlib::xy & end, const hwlib::xy & moving_cube_angle):
      line( w, location, end, moving_cube_angle)
    {}
    
    void set_speed(const int & new_speed){
        speed = new_speed;
    }
   
    void update() override {
        location.y = location.y + speed;
        end.y = end.y + speed;      
    }
   
};

// ===========================================================================

class cube : public drawable {
protected:

   int radius;
 
public:

   cube( hwlib::window & w, const hwlib::xy & midpoint, int radius ):
      drawable( w, 
         midpoint - hwlib::xy( radius, radius ), 
         hwlib::xy( radius, radius ) * 2 ),
      radius( radius )
   {}
   
   void draw() override {
    for(int i = location.x; i < location.x + size.x; i++){
        for(int j = location.y; j < location.y + size.y; j++){
            w.write(hwlib::xy(i,j));
        }
    }
}
};

// ===========================================================================

class moving_cube : public cube {
private:

   hwlib::xy speed;
   int score_1 = 0;
   int score_2 = 0;
   
public:

   moving_cube( 
      hwlib::window & w, 
      const hwlib::xy & midpoint, 
      int radius, 
      const hwlib::xy & speed 
   ):
      cube( w, midpoint, radius ),
      speed( speed )  
   {}
   
   void update() override {
      location = location + speed; 
   }
   
   void interact( drawable & other ) override {
      if( this != & other){
         if( overlaps( other )){
            auto moving_cube_angle = other.get_moving_cube_angle();
            if(moving_cube_angle.x < 2 && moving_cube_angle.y < 2){
                speed.x *= moving_cube_angle.x;
                speed.y *= moving_cube_angle.y;
            } else {
                if(moving_cube_angle.x == 3){
                    score_2 += 1;
                    location.x = 20;
                    location.y = 27;
                    speed.x = 5;
                    speed.y = 2;                    
                } else if(moving_cube_angle.x == 4) {
                    score_1 += 1;
                    location.x = 107;
                    location.y = 27;
                    speed.x = -5;
                    speed.y = -2;
                }
                w.clear();
                auto font    = hwlib::font_default_8x8();
                auto display = hwlib::terminal_from( w, font );
                if(score_1 < 4 && score_2 < 4){
                display 
                     << "\t0503" << score_1 << " - " << score_2
                     << hwlib::flush;
                } else {
                    if(score_1 == 4){
                         display 
                             << "\t0303" << " P1 WINS!!"
                             << hwlib::flush;
                    } else {
                        display 
                             << "\t0303" << " P2 WINS!!"
                             << hwlib::flush;
                    }
                    score_1 = 0;
                    score_2 = 0;
                }
                hwlib::wait_ms(1000);
            }
         }
      }
   }   
};

// ===========================================================================

#endif
