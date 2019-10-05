#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <iomanip>
using namespace std;
using std::string;
void updateVelocity(Vehicle* array[], int* grid)
{
	int k = 0;
	while (array[k] != nullptr)
	{
		if (signal_state == "GREEN" || (signal_state == "RED" && array[k]->speed != 0))
		{
			if (array[k]->speed + (array[k]->acceleration) * 1 <= array[k]->vehicle_maxspeed)
				array[k]->speed = array[k]->speed + (array[k]->acceleration) * 1;
			else
				array[k]->speed = array[k]->vehicle_maxspeed;
		}

		bool temp = true;								//if fouund empty road length/10 patch => accelerate
		for (int l = array[k]->indexj + 1; l < array[k]->indexj + road_length / 10 && l > 0; l++)
		{
			if (array[k]->v_char == "RED" || array[k]->indexi*road_length + l < 0 || (array[k]->indexi*road_length + l) >(road_length*road_width))
			{

				temp = false;
				break;
			}
			else if (grid[array[k]->indexi*road_length + l] != 0)
			{
				temp = false;
				break;
			}
		}
		if (temp == true)
		{
			array[k]->acceleration = array[k]->vehicle_acceleration + 5;
		}
		k++;
	}
}
