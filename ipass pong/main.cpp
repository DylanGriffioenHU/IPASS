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
private:

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
            speed.x *= moving_cube_angle.x;
            speed.y *= moving_cube_angle.y;
         }
      }
   }   
};

// ===========================================================================

int main(){
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );

    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    
    auto w       = hwlib::glcd_oled( i2c_bus, 0x3c );

    
    line top( w, hwlib::xy(   0,  0 ), hwlib::xy( 127,  0 ) , hwlib::xy(1,-1));
    line right( w, hwlib::xy( 127,  0 ), hwlib::xy( 127, 63 ), hwlib::xy(-1,1) );
    line bottom( w, hwlib::xy(   0, 63 ), hwlib::xy( 127, 63 ), hwlib::xy(1,-1) );
    line left( w, hwlib::xy(   0,  0 ), hwlib::xy(   0, 63 ), hwlib::xy(-1,1)  );
    player player1( w, hwlib::xy(   10, 24 ), hwlib::xy(   10, 37  ), hwlib::xy(-1,1)  );
    player player2( w, hwlib::xy(   117, 24 ), hwlib::xy(   117, 37  ), hwlib::xy(-1,1)  );
    moving_cube mc( w, hwlib::xy( 50, 20 ), 3, hwlib::xy( 5, 2 ) );

    std::array< drawable *, 7 > objects = { &mc, &top, &left, &right, &bottom, &player1, &player2 };
    
    

    for(;;){
      w.clear();
      for( auto & p : objects ){
         p->draw();
      }
      w.flush();
      hwlib::wait_ms( 200 );
      for( auto & p : objects ){
          p->update();
      }
      for( auto & p : objects ){
         for( auto & other : objects ){
            p->interact( *other );
         } 
      }
    }
}
