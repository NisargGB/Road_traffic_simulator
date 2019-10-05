#ifndef FUNCTIONS_INCLUDE
#define FUNCTIONS_INCLUDE
#include <vector>

class Vehicle
{
    // Access specifier
    public:

    // Data Members
        std::string vehicle_type;
        int vehicle_length;
        int vehicle_width;
        int vehicle_maxspeed;
        int vehicle_acceleration;
        std::string color;
        int speed;
        int acceleration;
        int indexi;
        int indexj;
        std::string v_char;
        Vehicle(int x, int y, int z, int w, std::string type, std::string denote);

};
extern int road_length;
extern int road_width;
extern std::string signal_state;
extern int road_signal;
extern int overtaking_width_var;
bool is_empty_or_not(int* , int , int , int , int );
bool isEmptyLane(int* , int , int , int , int , int );
void gridOvertakeUpdate(int* , int , Vehicle**);
bool check_lane_exchange(int* , Vehicle*, Vehicle*);
void updateGrid(Vehicle**, int* , std::string* );
void fillOccupancy(int* , std::string* , int , int , int , int , std::string);
void updatePosition(Vehicle**, int* );
bool indexMatch(int , int , int , int );
void printLayout(std::string* );
void updateVelocity(Vehicle**, int*);


#endif
