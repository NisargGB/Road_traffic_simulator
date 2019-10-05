#include <iostream>
#include <string>
#include <fstream>
#include "functions.h"
using namespace std;

Vehicle::Vehicle(int x, int y, int z, int w, std::string type, std::string denote)
{
    vehicle_length = x;
    vehicle_width = y;
    vehicle_maxspeed = z;
    vehicle_acceleration = w;
    vehicle_type = type;
    v_char = denote;
}
