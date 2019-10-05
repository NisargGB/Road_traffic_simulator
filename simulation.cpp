// #include <iostream>
// #include <string>
// #include <fstream>
// #include <vector>
// #include <sstream>
// #include "functions.h"
// #include <GL/glut.h>
// #include <iomanip>
// using namespace std;
// using std::string;
//
// int road_length;
// int road_width;
// string signal_state;
// int road_signal;
// struct Road
// {
//     int road_id;
//     int road_length;
//     int road_width;
//     string signal_state;
//     int road_signal;
// };
//
// // Vehicle getVehicle(Vehicle* arr, int i)
// // {
// //     for(int m = 0; m < 100; m++)
// //     {
// //         if(arr[m].indexi == i)
// //         {
// //             return arr[m];
// //         }
// //     }
// // }
//
// void execute(Vehicle* array[], int* grid, string* layout)
// {
//     // cout << "here1" << endl;;
//     updateGrid(array, grid, layout);
//     // cout << "here2" << endl;;
//     printLayout(layout);
//     // cout << "here3" << endl;;
//     updatePosition(array,grid);
//     // cout << "here4" << endl;;
//     updateVelocity(array,grid);
//     // updateAcceleration(array);
//
// }
//
// int main(int argc, char* argv[])
// {
//     Road road;
//     int def_maxspeed = 1;
//     int def_acceleration = 0;
//     string inifile = argv[1];
//     string data;
//     vector<vector<string>> v;
//     ifstream file;
//     file.open(inifile.c_str());
//     if(file.is_open())
//       {
//           while(file >> data)
//           {
//               if(data == "Road_Id")
//               {
//                   file >> data;
//                   file >> data;
//                   road.road_id = stoi(data);
//               }
//               if(data == "Road_Length")
//               {
//                   file >> data;
//                   file >> data;
//                   road.road_length = stoi(data);
//                   road_length = stoi(data);
//               }
//               if(data == "Road_Width")
//               {
//                   file >> data;
//                   file >> data;
//                   road.road_width = stoi(data);
//                   road_width = stoi(data);
//               }
//               if(data == "Road_Signal")
//               {
//                   file >> data;
//                   file >> data;
//                   road.road_signal = stoi(data);
//                   road_signal = stoi(data);
//               }
//               if(data == "Default_MaxSpeed")
//               {
//                   file >> data;
//                   file >> data;
//                   def_maxspeed = stoi(data);
//               }
//               if(data == "Default_Acceleration")
//               {
//                   file >> data;
//                   file >> data;
//                   def_acceleration = stoi(data);
//               }
//               if(data == "Vehicle_Type")
//               {
//                   file >> data;
//                   file >> data;
//                   //cout << data << endl;
//                   vector<string> a;
//                   a.push_back(data);
//                   a.push_back("0");
//                   a.push_back("0");
//                   string s1;
//                   std::stringstream ss1;
//                   ss1 << def_maxspeed;
//                   s1 = ss1.str();
//                   a.push_back(s1);
//                   string s2;
//                   std::stringstream ss2;
//                   ss2 << def_acceleration;
//                   s2 = ss2.str();
//                   a.push_back(s2);
//                   a.push_back("X");
//                   while(data != "end_v")
//                   {
//                       file >> data;
//                       if(data == "Vehicle_Length")
//                       {
//                           file >> data;
//                           file >> data;
//                           a[1] = data;
//                       }
//                       if(data == "Vehicle_Width")
//                       {
//                           file >> data;
//                           file >> data;
//                           a[2] = data;
//                       }
//                       if(data == "Vehicle_MaxSpeed")
//                       {
//                           file >> data;
//                           file >> data;
//                           a[3] = data;
//                       }
//                       if(data == "Vehicle_Acceleration")
//                       {
//                           file >> data;
//                           file >> data;
//                           a[4] = data;
//                       }
//                       if(data == "Vehicle_Denotion")
//                       {
//                           file >> data;
//                           file >> data;
//                           a[5] = data;
//                       }
//                   }
//                   v.push_back(a);
//                   cout << "is is" << v[0][0] << endl;
//               }
//
//           }
//       }
//       else
//       {
//         cout << "The file '" << inifile << "' could not be opened.";
//         return 0;
//       }
//     file.close();
//     Vehicle** array;
//     int dimension = road_length*road_width;
//     array = new Vehicle*[dimension/10];
//     // array = (Vehicle**)malloc(sizeof(Vehicle)*(100));
//     for(int i = 0; i < dimension/10; i++)
//     {
//         array[i] = nullptr;
//     }
//     string* road_layout;
//     road_layout = new string[dimension];
//     int* grid;
//     grid = new int[dimension];
//     // grid = (int*)malloc(sizeof(int)*(road_length*road_width));
//     for(int i = 0; i < dimension; i++)
//     {
//         road_layout[i] = "-";
//         grid[i] = 0;
//     }
//     //Vehicle* array[100] = {nullptr};
//     //string road_layout[road.road_length*road.road_width] = {"-"};
//     //int grid[road.road_length*road.road_width] = {0};
//     int i = 0;
//     int time = 0;
//     ifstream file2;
//     file2.open(inifile.c_str());
//     string word;
//     while(file2 >> word)
//     {
//         if(word == "START")
//         {
//
//             while(file2 >> word)
//             {
//                 if(word == "#")
//                 {
//                     string line;
//                     getline(file2, line);
//                 }
//                 for(int j = 0; j < v.size(); j++)
//                 {
//                     if(word == v[j][0])
//                     {
//                         array[i] = new Vehicle(stoi(v[j][1]),stoi(v[j][2]),stoi(v[j][3]),stoi(v[j][4]),v[j][0],v[j][5]);
//                         file2 >> word;
//                         array[i]->color = word;
//                         cout << array[i]->vehicle_type << " " << i << array[i]->vehicle_length << " " << array[i]->vehicle_width << " " << array[i]->vehicle_maxspeed << " " << array[i]->vehicle_acceleration << " " << array[i]->color << " " << array[i]->vehicle_type << endl;
//                         cout << array[i]->v_char << endl;
//
//                         int temp = 1;
//                         for(int l = 0; l < road_width; l++)
//                         {
//                             temp = 1;
//                             for(int w = 0; w < array[i]->vehicle_width; w++)
//                             {
//                                 if(grid[(l+w)*road_length] != 0 || (l+w) >= road_width)
//                                 {
//                                     temp = 0;
//                                     break;
//                                 }
//                             }
//                             if(temp == 1)
//                             {
//                                 array[i]->indexi = l;
//                                 array[i]->indexj = 0;
//                                 array[i]->speed = 1;
//                                 array[i]->acceleration = array[i] -> vehicle_acceleration;
//                                 break;
//                             }
//                         }
//                         if(temp == 1)
//                             i++;        //if there is no space for the vehicle then reject its enterance
//                         else
//                         {
//                             cout << "----------------------------------------------------------------------------------" << i << endl;;
//                             array[i] = nullptr;
//                         }
//                         execute(array, grid, road_layout);
//                         time++;
//                         cout << "time = " << time << endl;;
//
//                     }
//                 }
//                 if(word == "Pass")
//                 {
//                     cout << "pass stage" << endl;;
//                     file2 >> word;
//                     int counter = stoi(word);
//                     for(int i = 0; i < counter - 1; i++)
//                     {
//                         execute(array, grid, road_layout);
//                         time++;
//                         cout << time << endl;;
//                     }
//                     int c = 0;
//                     int l = 0;
//                     int count = 0;
//                     for(c = 0; ; c++)
//                     {
//                         if(array[c]->indexj > road_length)
//                             count++;
//                         else
//                             break;
//                     }
//                     cout << count << endl;
//                     for(l = 0; array[count + l] != nullptr ; l++)
//                     {
//                         //cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
//                         array[l] = array[count+l];
//                     }
//                     i = l;
//                 }
//                 if(word == "Signal")
//                 {
//                     file2 >> word;
//                     cout << "Signal changed to " << word << endl;
//                     road.signal_state = word;
//                     signal_state = word;
//                 }
//
//                 if(word == "END")
//                 {
//                     cout << "end received" << endl;
//                     bool temp = false;
//                     int k = 0;
//                     while(true)
//                     {
//                         temp = false;
//                         k = 0;
//                         while(array[k] != nullptr)
//                         {
//                             if(array[k]->indexj - array[k]->vehicle_length < road_length)
//                             {
//                                 temp = true;
//                                 cout << "vehicle number " << k << endl;
//                             }
//
//                             k++;
//                         }
//                         // if(array[9] != nullptr)
//                         // {
//                         //     cout << array[9]->indexj << endl;
//                         //     cout << array[9]->speed << endl;
//                         // }
//                         // cout << temp << endl;
//                         if(temp == true)
//                         {
//                             execute(array, grid, road_layout);
//                             time++;
//                             cout << time << endl;
//                         }
//                         else
//                             break;
//                     }
//                 }
//             }
//         }
//     }
//     file2.close();
//
//
//     array[i] = new Vehicle(2,2,3,1, "Car","C");
//     array[i+1] = new Vehicle(2,1,6,1, "Bike","b");
//     array[i]->speed = 3;
//     array[i]->indexi = 2;
//     array[i]->indexj = 5;
//     grid[2*road.road_length + 5] = 1;
//     grid[2*road.road_length + 4] = 1;
//     //grid[1*road.road_length + 3] = 1;
//     grid[3*road.road_length + 5] = 1;
//     grid[3*road.road_length + 4] = 1;
//     //grid[2*road.road_length + 3] = 1;
//     array[i+1]->indexi = 3;
//     array[i+1]->indexj = 3;
//     array[i+1]->speed = 5;
//     grid[3*road.road_length + 3] = 1;
//     grid[3*road.road_length + 2] = 1;
//     //grid[3*road.road_length + 2] = 1;
//     //grid[3*road.road_length + 1] = 1;
//     grid[4*road.road_length + 5] = 1;
//     grid[4*road.road_length + 4] = 1;
//     grid[1*road.road_length + 5] = 1;
//     grid[1*road.road_length + 4] = 1;
//     cout << boolalpha; //for printing true or false;
//     for(int l = 0; l < road.road_width; l++)
//     {
//         for(int k = 0; k < road.road_length; k++)
//         {
//             cout << grid[l*road.road_length + k];
//         }
//         cout << endl;
//     }
//     cout << "checking lane " << check_lane_exchange(grid, array[i+1], array[i]) << endl;
//     for(int l = 0; l < road.road_width; l++)
//     {
//         for(int k = 0; k < road.road_length; k++)
//         {
//             cout << grid[l*road.road_length + k];
//         }
//         cout << endl;
//     }
//     cout << "checking lane again" << check_lane_exchange(grid, array[i+1], array[i]) << endl;
//
// }
