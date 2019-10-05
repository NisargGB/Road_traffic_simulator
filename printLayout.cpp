#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <iomanip>
using namespace std;
using std::string;
void printLayout(string *layout)
{
    for(int i = 0; i < road_width; i++)
    {

        for(int j = 0; j < road_length; j++)
        {
            if(signal_state == "RED" and j == road_signal )
            {
                cout << "|";
            }
            cout << layout[i*road_length + j];
        }
        cout << endl;
    }
    cout << "------------------------------------------------------------" << endl;
}
