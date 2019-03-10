#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class Vehicle
{
    // Access specifier
    public:

    // Data Members
        string vehicle_type;
        int vehicle_length;
        int vehicle_width;

        // Member Functions()
        // void printname()
        // {
        //    cout << "Geekname is: " << geekname;
        // }
        Vehicle()
        {
        }

        Vehicle(int x, int y)
        {
            vehicle_length = x;
            vehicle_width = y;
        }
};

class Car : public Vehicle
{
    public:
        int vehicle_maxspeed;
        int vehicle_acceleration;
        string color;
        Car(int x_car, int y_car, int z_car, int w_car)
        {
            vehicle_length = x_car;
            vehicle_width = y_car;
            vehicle_maxspeed = z_car;
            vehicle_acceleration = w_car;
        }

};

class Bike : public Vehicle
{
    public:
        int vehicle_maxspeed;
        int vehicle_acceleration;
        string color;
        Bike(int x_bike, int y_bike, int z_bike, int w_bike)
        {
            vehicle_length = x_bike;
            vehicle_width = y_bike;
            vehicle_maxspeed = z_bike;
            vehicle_acceleration = w_bike;
        }

};

class Bus : public Vehicle
{
    public:
        int vehicle_maxspeed;
        int vehicle_acceleration;
        string color;
        Bus(int x_bus, int y_bus, int z_bus, int w_bus)
        {
            vehicle_length = x_bus;
            vehicle_width = y_bus;
            vehicle_maxspeed = z_bus;
            vehicle_acceleration = w_bus;
        }

};
