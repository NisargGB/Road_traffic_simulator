#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <iomanip>
using namespace std;
using std::string;
bool is_empty_or_not(int *grid, int i, int y, int upto, int x)
{
    for(int k = -x + 1; x + k < y + upto + 1; k++)
    {
        if(i < 0 || i >= road_width || i*road_length + x + k > road_length*road_width - 1 || grid[i*road_length + x + k] > 0)//------
            return false;
    }
    return true;
}

bool isEmptyLane(int *grid, int i, int y, int upto, int x, int w)
{
	for (int k = i; k < i + w; k++)
	{
		if (is_empty_or_not(grid, k, y, upto, x) == false)
			return false;
	}
	return true;
}
void gridOvertakeUpdate(int *grid, int ia, Vehicle *a)
{
    for(int l = 0; l < a->vehicle_length; l++)
    {
        for(int w = 0; w < a->vehicle_width; w++)
        {
            grid[(ia+w)*road_length + a->indexj - l] = 0;
            grid[(a->indexi+w)*road_length + a->indexj - l] = 1;
        }
    }
}
bool indexMatch(int ia, int ib, int aw, int bw)
{
    for(int k = 1 - aw; k < bw; k++)
    {
        if(ia == ib + k)
        return true;
    }
    return false;
}

bool check_lane_exchange(int* grid, Vehicle* a, Vehicle* b)   //a is the vehicle behind and b is the leading vehicle
{
	int ia = a->indexi;
	int ib = b->indexi;
	int ja = a->indexj;
	int jb = b->indexj;
	if (a->vehicle_type == "Bike" and b->vehicle_type == "Bike")
	{
		if (indexMatch(ia, ib, a->vehicle_width, b->vehicle_width))
		{
			if (((jb - ja - b->vehicle_length) <= ((a->speed - b->speed) * 1)) && (jb - ja - b->vehicle_length) >= 0)
			{
				int ic = max(ia, ib);
				int id = min(ia, ib);

				if (isEmptyLane(grid, id - a->vehicle_width - 1, jb, a->speed, ja, a->vehicle_width) || isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width) || isEmptyLane(grid, ic + a->vehicle_width, jb, a->speed, ja, a->vehicle_width) || (isEmptyLane(grid, ic + a->vehicle_width + 1, jb, a->speed, ja, a->vehicle_width)))
				{
					if (isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ic + a->vehicle_width, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = ic + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, id - a->vehicle_width - 1, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width - 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ic + a->vehicle_width + 1, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = ic + a->vehicle_width + 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					// a would change the lane; grid update
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if (isEmptyLane(grid, ic + a->vehicle_width + 2, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = ic + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, id - a->vehicle_width - 2, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (signal_state == "RED")
					{
						if (a->speed == 0 && ib + b->vehicle_width + 1 + a->vehicle_width < road_width && isEmptyLane(grid, ia + a->vehicle_width, jb, a->speed, ja, b->vehicle_width + ib - ia))
						{
							a->indexi = ib + b->vehicle_width;
							gridOvertakeUpdate(grid, ia, a);
						}
						if (a->speed == 0 && ib - a->vehicle_width >= 0 && isEmptyLane(grid, ib - a->vehicle_width, jb, a->speed, ja, ia + a->vehicle_width - ib))
						{
							a->indexi = ib - a->vehicle_width;
							gridOvertakeUpdate(grid, ia, a);
						}
					}
					a->speed = b->speed;
					bool temp1 = true;
					int m;
					for (m = a->indexj + 1; m < b->indexj - b->vehicle_length + 1; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + m] == 1)
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
					a->indexj = m - 1;
					/*bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
							break;
						}
					}

					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;      //to stick them*/
						//a->speed would decrease
						//a->acceleration would decrease
					return false;
				}
			}
		}
	}
	if (a->vehicle_type == "Bike" and b->vehicle_type != "Bike")
	{
		if (indexMatch(ia, ib, a->vehicle_width, b->vehicle_width))
		{
			if (((jb - ja - b->vehicle_length + 1) <= ((a->speed - b->speed) * 1) + 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				int ic = max(ia, ib);
				int id = min(ia, ib);

				if ((isEmptyLane(grid, id - a->vehicle_width - 1, jb, a->speed, ja, a->vehicle_width)) || isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width) || isEmptyLane(grid, ic + a->vehicle_width + 1, jb, a->speed, ja, a->vehicle_width) || (isEmptyLane(grid, ic + a->vehicle_width + 2, jb, a->speed, ja, a->vehicle_width)))
				{
					if (isEmptyLane(grid, ic + a->vehicle_width + 1, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = ic + a->vehicle_width + 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, id - a->vehicle_width - 1, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width - 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ic + a->vehicle_width + 2, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = ic + a->vehicle_width + 2;
						gridOvertakeUpdate(grid, ia, a);
					}// a would change the lane;
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if ((isEmptyLane(grid, ic + a->vehicle_width + 3, jb, a->speed, ja, a->vehicle_width)) || (isEmptyLane(grid, ic + a->vehicle_width + 2, jb, a->speed, ja, a->vehicle_width)))
					{
						a->indexi = ic + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if ((isEmptyLane(grid, id - a->vehicle_width - 1, jb, a->speed, ja, a->vehicle_width)) || (isEmptyLane(grid, id - a->vehicle_width - 2, jb, a->speed, ja, a->vehicle_width)))
					{
						a->indexi = id - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (signal_state == "RED")
					{
						if (a->speed == 0 && ib + b->vehicle_width + 1 + a->vehicle_width < road_width && isEmptyLane(grid, ia + a->vehicle_width, jb, a->speed, ja, b->vehicle_width + ib - ia))
						{
							a->indexi = ib + b->vehicle_width;
							gridOvertakeUpdate(grid, ia, a);
						}
						if (a->speed == 0 && ib - a->vehicle_width >= 0 && isEmptyLane(grid, ib - a->vehicle_width, jb, a->speed, ja, ia + a->vehicle_width - ib))
						{
							a->indexi = ib - a->vehicle_width;
							gridOvertakeUpdate(grid, ia, a);
						}
					}
					a->speed = b->speed;
					//cout << "wwc" << endl;
					bool temp1 = true;
					int m;
					for (m = a->indexj + 1; m < b->indexj - b->vehicle_length + 1; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + m] == 1)
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
					a->indexj = m - 1;
					/*for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
						}
					}
					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;*/
						//a->speed would decrease
						//a->acceleration would decrease
					return false;
				}
			}
		}
	}

	if (a->vehicle_type != "Bike")
	{
		if (indexMatch(ia, ib, a->vehicle_width, b->vehicle_width))
		{
			if (((jb - ja - b->vehicle_length + 1) <= ((a->speed - b->speed) * 1) + 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{

				int ic = max(ia, ib);
				int id = min(ia, ib);

				if (a->v_char == "C" && b->v_char == "T")
				{
					//todo
					cout << "" << endl;
				}
				if ((isEmptyLane(grid, ic + a->vehicle_width, jb, a->speed, ja, a->vehicle_width)) || (isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width)))
				{
					if (isEmptyLane(grid, ic + a->vehicle_width, jb, a->speed, ja, a->vehicle_width))
					{
						cout << "enter " << endl;
						a->indexi = ic + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, id - a->vehicle_width, jb, a->speed, ja, a->vehicle_width))
					{
						a->indexi = id - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					// a would change the lane;
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					a->speed = b->speed;
					bool temp1 = true;
					int m;
					for (m = a->indexj + 1; m < b->indexj - b->vehicle_length + 1; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + m] == 1)
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
					a->indexj = m - 1;
					/*bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
						}
					}
					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;*/
						//a->speed would decrease
						//a->acceleration would decrease
					return false;
				}
			}
		}
	}
	// if(a->vehicle_type != "Bike" and b->vehicle_type == "Bike")
	// {
	//     if(indexMatch(ia,ib,a->vehicle_width,a->s))
	//     {
	//         if(((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed)*1) && jb - ja - b->vehicle_length + 1 >= 1)
	//         {
	//             if((isEmptyLane(grid, ib-a->vehicle_width, jb, b->speed,ja,a->vehicle_width)) || (isEmptyLane(grid, ib+a->s, jb, b->speed,ja,a->vehicle_width)))
	//             {
	//                 if(isEmptyLane(grid, ib+2, jb, b->speed,ja) && (isEmptyLane(grid, ib+3, jb, b->speed,ja)))
	//                 {
	//                     cout << "enter " << endl;
	//                     a->indexi = ib + 2;
	//                     grid[ia*road_length + ja] = 0;
	//                     grid[ia*road_length + ja - 1] = 0;
	//                     grid[(ia+1)*road_length + ja] = 0;
	//                     grid[(ia+1)*road_length + ja - 1] = 0;
	//                     grid[a->indexi*road_length + ja] = 1;
	//                     grid[a->indexi*road_length + ja - 1] = 1;
	//                     grid[(a->indexi+1)*road_length + ja] = 1;
	//                     grid[(a->indexi+1)*road_length + ja - 1] = 1;
	//                 }
	//                 else if(isEmptyLane(grid, ib-2, jb, b->speed,ja) && isEmptyLane(grid, ib-1, jb, b->speed,ja))
	//                 {
	//                     a->indexi = ib - 2;
	//                     grid[ia*road_length + ja] = 0;
	//                     grid[ia*road_length + ja - 1] = 0;
	//                     grid[(ia+1)*road_length + ja] = 0;
	//                     grid[(ia+1)*road_length + ja - 1] = 0;
	//                     grid[a->indexi*road_length + ja] = 1;
	//                     grid[a->indexi*road_length + ja - 1] = 1;
	//                     grid[(a->indexi+1)*road_length + ja] = 1;
	//                     grid[(a->indexi+1)*road_length + ja - 1] = 1;
	//                 }
	//                 // a would change the lane;
	//                 //during changing lane, acceleration would be zero
	//                 return true;
	//             }
	//             else
	//             {
	//                 a->speed = b->speed;
	//                 bool temp1 = true;
	//                 for(int m = b->vehicle_length; jb - m > ja ;m++)
	//                 {
	//                     if(grid[a->indexi*road_length + b->indexj - m] == 1)
	//                         temp1 = false;
	//                 }
	//                 if(temp1 == true)
	//                     a->indexj = b->indexj - b->vehicle_length;
	//                 //a->speed would decrease
	//                 //a->acceleration would decrease
	//                 return false;
	//             }
	//         }
	//     }
	// }
	return false;
}
