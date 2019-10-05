#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <iomanip>
using namespace std;
using std::string;
void updatePosition(Vehicle* array[], int* grid)
{
	int k = 0;
	int i = 0;
	//check for lane exchange to be called for vehicles that might be involve din overlapping
	while (array[i] != nullptr)
	{
		for (int j = i; array[j] != nullptr; j++)
		{
			if (i != j)
			{
				check_lane_exchange(grid, array[i], array[j]);
				check_lane_exchange(grid, array[j], array[i]);
			}
		}
		i++;
	}
	while (array[k] != nullptr)
	{
		if (signal_state == "GREEN" || ((signal_state == "RED" && array[k]->indexj + (array[k]->speed) * 1 < road_signal - 2)) || ((signal_state == "RED" && array[k]->indexj > road_signal)))
		{
			array[k]->indexj = array[k]->indexj + (array[k]->speed) * 1;
		}
		else
		{
			array[k]->speed = 0;
			// cout << "present sir " << endl;
			/*bool temp1 = true;
			for (int m = road_signal + 1; m > array[k]->indexj && array[k]->indexj >= 0; m--)
			{
				// cout << array[k]->indexi*road_length + m << endl;
				if (array[k]->indexi*road_length + m < road_length*road_width && array[k]->indexi*road_length + m > -1)
				{
					if (grid[array[k]->indexi*road_length + m] == 1)
					{
						temp1 = false;
					}
				}

			}
			if (temp1 == true)
			{
				// cout << "present here also" << endl;
				array[k]->indexj = road_signal - 1;
			}
			else
			{
				// cout << "present here " << endl;
				array[k]->indexj = array[k]->indexj;				//TODO
			}*/
			int m = array[k]->indexj;
			bool temp1 = true;
			for (m = array[k]->indexj + 1; m < road_signal && array[k]->indexj >= 0; m++)
			{
				// cout << array[k]->indexi*road_length + m << endl;
				if (array[k]->indexi*road_length + m < road_length*road_width && array[k]->indexi*road_length + m > -1)
				{
					if (grid[array[k]->indexi*road_length + m] == 1)
					{
						temp1 = false;
						cout << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii" << endl;
						break;
					}
				}
			}
			cout << m << "  " << array[k]->color << "   " << endl;
			array[k]->indexj = m - 1;
		}

		//signal packing logic
		if (signal_state == "RED" && array[k]->speed == 0 && array[k]->indexi > 0 && array[k]->indexi < road_width)
		{
			bool temp1 = true;
			int m = array[k]->indexj + 1;
			for (m = array[k]->indexj + 1; m < road_signal && (array[k]->indexj >= 0) && (array[k]->indexj < road_length); m++)
			{
				for (int w = 0; w < array[k]->vehicle_width; w++)
				{
					if (grid[(array[k]->indexi + w)*road_length + m] == 1)
					{
						temp1 = false;
						break;
					}
				}
				if (temp1 == false)
				{
					break;
				}
			}
			array[k]->indexj = m - 1;
			if (array[k]->speed == 0 && array[k]->v_char == "b" && array[k]->indexj == road_signal - road_length / 60 - 1 && grid[array[k]->indexi*road_length + road_signal + road_length / 60] == 0)
			{
				cout << "-------------------------------------------" << endl;
				array[k]->indexj = road_signal + array[k]->vehicle_length;
			}
		}

		k++;
	}

}
