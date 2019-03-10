#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <pthread.h>
#include <chrono>
#include "functions.h"
#include <iomanip>
using namespace std::chrono;
using namespace std;
using std::string;

int road_length;
int road_width;
struct Road
{
    int road_id;
    int road_length;
    int road_width;
    int road_signal;
};
class Vehicle
{
    // Access specifier
    public:

    // Data Members
        string vehicle_type;
        int vehicle_length;
        int vehicle_width;
        int vehicle_maxspeed;
        int vehicle_acceleration;
        string color;
        int speed;
        int acceleration;
        int indexi;
        int indexj;
        Vehicle(int x, int y, int z, int w, string type)
        {
            vehicle_length = x;
            vehicle_width = y;
            vehicle_maxspeed = z;
            vehicle_acceleration = w;
            vehicle_type = type;
        }
};
Vehicle getVehicle(Vehicle* arr, int i)
{
    for(int m = 0; m < 100; m++)
    {
        if(arr[m].indexi == i)
        {
            return arr[m];
        }
    }
}
bool is_empty_or_not(int *grid, int i, int y, int upto)
{
    for(int k = 0; k < upto+1; k++)
    {
        if(i < 0 || i >= road_width || grid[i*road_length + y+k] > 0)
            return false;
    }
    return true;
}

bool check_lane_exchange(int* grid, Vehicle a, Vehicle b)   //a is the vehicle behind and b is the leading vehicle
{
    int ia = a.indexi;
    int ib = b.indexi;
    int ja = a.indexj;
    int jb = b.indexj;
    if(a.vehicle_type == "Bike" and b.vehicle_type == "Bike")
    {
        if(ia == ib)
        {
            if(((jb - ja - 1) <= (a.speed - b.speed)*1) && (jb - ja - 1) >= 1)
            {
                if(is_empty_or_not(grid, ib-2, jb, b.speed) || is_empty_or_not(grid, ib-1, jb, b.speed) || is_empty_or_not(grid, ib+1, jb, b.speed) || (is_empty_or_not(grid, ib+2, jb, b.speed)))
                {
                    if( is_empty_or_not(grid, ib-1, jb, b.speed))
                    {
                        a.indexi = ib-1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if( is_empty_or_not(grid, ib+1, jb, b.speed))
                    {
                        a.indexi = ib+1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if( is_empty_or_not(grid, ib-2, jb, b.speed))
                    {
                        a.indexi = ib-2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if( is_empty_or_not(grid, ib+2, jb, b.speed))
                    {
                        a.indexi = ib+2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    // a would change the lane; grid update
                    //during changing lane, acceleration would be zero
                    return true;
                }
                else
                {
                    if(is_empty_or_not(grid, ib+3, jb, b.speed))
                    {
                        a.indexi = ib+1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if(is_empty_or_not(grid, ib-3, jb, b.speed))
                    {
                        a.indexi = ib-1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    a.speed = b.speed;
                    //a.speed would decrease
                    //a.acceleration would decrease
                    return false;
                }
            }
        }
    }
    if(a.vehicle_type == "Bike" and b.vehicle_type != "Bike")
    {
        if(ia == ib || ia == ib + 1)
        {
            if(((jb - ja - b.vehicle_length + 1) <= (a.speed - b.speed)*1) && jb - ja - b.vehicle_length + 1 >= 1)
            {
                if((is_empty_or_not(grid, ib-2, jb, b.speed) && ia == ib) || is_empty_or_not(grid, ib-1, jb, b.speed) || is_empty_or_not(grid, ib+2, jb, b.speed) || (is_empty_or_not(grid, ib+3, jb, b.speed) && ia == ib+1))
                {
                    if(is_empty_or_not(grid, ib+2, jb, b.speed))
                    {
                        a.indexi = ib+2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if(is_empty_or_not(grid, ib-2, jb, b.speed) && ia == ib)
                    {
                        a.indexi = ib-2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if(is_empty_or_not(grid, ib-1, jb, b.speed))
                    {
                        a.indexi = ib-1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if(is_empty_or_not(grid, ib+3, jb, b.speed) && ia == ib + 1)
                    {
                        a.indexi = ib+3;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }// a would change the lane;
                    //during changing lane, acceleration would be zero
                    return true;
                }
                else
                {
                    if((is_empty_or_not(grid, ib+4, jb, b.speed) && ia == ib + 1) || (is_empty_or_not(grid, ib+3, jb, b.speed) && ia == ib))
                    {
                        a.indexi = ib+1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    else if((is_empty_or_not(grid, ib-2, jb, b.speed) && ia == ib + 1) || (is_empty_or_not(grid, ib-3, jb, b.speed) && ia == ib))
                    {
                        a.indexi = ib-1;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                    }
                    a.speed = b.speed;
                    //a.speed would decrease
                    //a.acceleration would decrease
                    return false;
                }
            }
        }
    }

    if(a.vehicle_type != "Bike" and b.vehicle_type != "Bike")
    {
        if(ia == ib || ia == ib + 1 || ia == ib - 1)
        {
            if(((jb - ja - b.vehicle_length + 1) <= (a.speed - b.speed)*1) && jb - ja - b.vehicle_length + 1 >= 1)
            {
                if((is_empty_or_not(grid, ib-2, jb, b.speed) && is_empty_or_not(grid, ib-1, jb, b.speed)) || (is_empty_or_not(grid, ib+2, jb, b.speed) && (is_empty_or_not(grid, ib+3, jb, b.speed))))
                {
                    if(is_empty_or_not(grid, ib+2, jb, b.speed) && (is_empty_or_not(grid, ib+3, jb, b.speed)))
                    {
                        cout << "enter " << endl;
                        a.indexi = ib + 2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[(ia+1)*road_length + ja] = 0;
                        grid[(ia+1)*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                        grid[(a.indexi+1)*road_length + ja] = 1;
                        grid[(a.indexi+1)*road_length + ja - 1] = 1;
                    }
                    else if(is_empty_or_not(grid, ib-2, jb, b.speed) && is_empty_or_not(grid, ib-1, jb, b.speed))
                    {
                        a.indexi = ib - 2;
                        grid[ia*road_length + ja] = 0;
                        grid[ia*road_length + ja - 1] = 0;
                        grid[(ia+1)*road_length + ja] = 0;
                        grid[(ia+1)*road_length + ja - 1] = 0;
                        grid[a.indexi*road_length + ja] = 1;
                        grid[a.indexi*road_length + ja - 1] = 1;
                        grid[(a.indexi+1)*road_length + ja] = 1;
                        grid[(a.indexi+1)*road_length + ja - 1] = 1;
                    }
                    // a would change the lane;
                    //during changing lane, acceleration would be zero
                    return true;
                }
                else
                {
                    a.speed = b.speed;
                    //a.speed would decrease
                    //a.acceleration would decrease
                    return false;
                }
            }
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    Road road;
    int def_maxspeed = 1;
    int def_acceleration = 0;
    string inifile = argv[1];
    string data;
    vector<vector<string>> v;
    ifstream file;
    file.open(inifile.c_str());
    if(file.is_open())
      {
          while(file >> data)
          {
              if(data == "Road_Id")
              {
                  file >> data;
                  file >> data;
                  road.road_id = stoi(data);
              }
              if(data == "Road_Length")
              {
                  file >> data;
                  file >> data;
                  road.road_length = stoi(data);
                  road_length = stoi(data);
              }
              if(data == "Road_Width")
              {
                  file >> data;
                  file >> data;
                  road.road_width = stoi(data);
                  road_width = stoi(data);
              }
              if(data == "Road_Signal")
              {
                  file >> data;
                  file >> data;
                  road.road_signal = stoi(data);
              }
              if(data == "Default_MaxSpeed")
              {
                  file >> data;
                  file >> data;
                  def_maxspeed = stoi(data);
              }
              if(data == "Default_Acceleration")
              {
                  file >> data;
                  file >> data;
                  def_acceleration = stoi(data);
              }
              if(data == "Vehicle_Type")
              {
                  file >> data;
                  file >> data;
                  cout << data << endl;
                  vector<string> a;
                  a.push_back(data);
                  a.push_back("5");
                  a.push_back("5");
                  string s1;
                  std::stringstream ss1;
                  ss1 << def_maxspeed;
                  s1 = ss1.str();
                  a.push_back(s1);
                  string s2;
                  std::stringstream ss2;
                  ss2 << def_acceleration;
                  s2 = ss2.str();
                  a.push_back(s2);
                  while(data != "end_v")
                  {
                      file >> data;
                      if(data == "Vehicle_Length")
                      {
                          file >> data;
                          file >> data;
                          a[1] = data;
                      }
                      if(data == "Vehicle_Width")
                      {
                          file >> data;
                          file >> data;
                          a[2] = data;
                      }
                      if(data == "Vehicle_MaxSpeed")
                      {
                          file >> data;
                          file >> data;
                          a[3] = data;
                      }
                      if(data == "Vehicle_Acceleration")
                      {
                          file >> data;
                          file >> data;
                          a[4] = data;
                      }
                  }
                  v.push_back(a);
                  cout << "is is" << v[0][0] << endl;
              }

          }
      }
      else
      {
        cout << "The file '" << inifile << "' could not be opened.";
        return 0;
      }
    file.close();

    Vehicle* array[100];
    int i = 0;
    int time = 0;
    ifstream file2;
    file2.open(inifile.c_str());
    string word;
    while(file2 >> word)
    {
        if(word == "START")
        {
            while(file2 >> word)
            {
                for(int j = 0; j < v.size(); j++)
                {
                    if(word == v[j][0])
                    {
                        array[i] = new Vehicle(stoi(v[j][1]),stoi(v[j][2]),stoi(v[j][3]),stoi(v[j][4]),v[j][0]);
                        file2 >> word;
                        array[i]->color = word;
                        cout << array[i]->vehicle_type << " " << i << array[i]->vehicle_length << " " << array[i]->vehicle_width << " " << array[i]->vehicle_maxspeed << " " << array[i]->vehicle_acceleration << " " << array[i]->color << " " << array[i]->vehicle_type << endl;
                        i++;
                        time++;

                    }
                }
            }
        }
    }
    file2.close();

    int grid[road.road_length*road.road_width] = {0};
    Vehicle a(2,2,3,1, "Car");
    Vehicle b(2,1,6,1, "Bike");
    a.speed = 3;
    a.indexi = 2;
    a.indexj = 5;
    grid[2*road.road_length + 5] = 1;
    grid[2*road.road_length + 4] = 1;
    //grid[1*road.road_length + 3] = 1;
    grid[3*road.road_length + 5] = 1;
    grid[3*road.road_length + 4] = 1;
    //grid[2*road.road_length + 3] = 1;
    b.indexi = 3;
    b.indexj = 3;
    b.speed = 5;
    grid[3*road.road_length + 3] = 1;
    grid[3*road.road_length + 2] = 1;
    //grid[3*road.road_length + 2] = 1;
    //grid[3*road.road_length + 1] = 1;
    grid[4*road.road_length + 5] = 1;
    grid[4*road.road_length + 4] = 1;
    grid[1*road.road_length + 5] = 1;
    grid[1*road.road_length + 4] = 1;
    cout << boolalpha; //for printing true or false;
    for(int l = 0; l < road.road_width; l++)
    {
        for(int k = 0; k < road.road_length; k++)
        {
            cout << grid[l*road.road_length + k];
        }
        cout << endl;
    }
    cout << "checking lane " << check_lane_exchange(grid, b, a) << endl;
    for(int l = 0; l < road.road_width; l++)
    {
        for(int k = 0; k < road.road_length; k++)
        {
            cout << grid[l*road.road_length + k];
        }
        cout << endl;
    }
    cout << "checking lane again" << check_lane_exchange(grid, b, a) << endl;

}
