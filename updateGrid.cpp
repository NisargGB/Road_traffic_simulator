#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <iomanip>
using namespace std;
using std::string;

void fillOccupancy(int* grid, string* layout, int i, int j, int l, int w, string vChar)
{
    // cout << i << j << l << w << vChar << endl;
    if(i < road_width)
    {
        for(int m = 0; m < w; m++)
        {
            for(int n = l-1; n >=0; n--)
            {
                if(j-n >= 0 && j-n < road_length)
                {
                    grid[(i+m)*road_length + j - n] = 1;
                    layout[(i+m)*road_length + j - n] = vChar;
                }
            }
        }
    }
}

void updateGrid(Vehicle* array[], int* grid, string* layout)
{
	for (int i = 0; i < road_width; i++)
	{
		for (int j = 0; j < road_length; j++)
		{
			grid[i*road_length + j] = 0;
			layout[i*road_length + j] = "-";
		}
	}
	int k = 0;
	while (array[k] != nullptr)
	{
		if (array[k]->indexi >= 0 && array[k]->indexj < road_length + array[k]->vehicle_width)
		{
			fillOccupancy(grid, layout, array[k]->indexi, array[k]->indexj, array[k]->vehicle_length, array[k]->vehicle_width, array[k]->v_char);
		}
		k++;
	}
}
