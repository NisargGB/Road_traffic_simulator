/*
 * GL06TimerFunc.cpp: Translation and Rotation
 * Transform primitives from their model spaces to world space (Model Transform).
 */
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
using namespace std;
using std::string;

 // global variable
GLfloat angle = 0.0f;  // rotational angle of the shapes
GLfloat xcoord[100];
GLfloat ycoord[100];
int refreshMills = 1000; // refresh interval in milliseconds
int frame_num = 0;
int frame = 1;

string signal_state;
int road_signal;
int road_length;
int road_width;
float gap;

int* grid;
string* road_layout;

struct Road
{
	int road_id;

	string signal_state;
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
	string v_char;
	Vehicle(int x, int y, int z, int w, string type, string denote)
	{
		vehicle_length = x;
		vehicle_width = y;
		vehicle_maxspeed = z;
		vehicle_acceleration = w;
		vehicle_type = type;
		v_char = denote;
	}
};

Vehicle** array2;
//Vehicle* array2[100] = { nullptr };
//string road_layout[(::road_length)*(::road_width)] = { "-" };
//int grid[road_length * road_width] = { 0 };

/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}


/* Called back when timer expired */
void Timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, Timer, 0); // next Timer call milliseconds later
	//printf("%d", frame_num);
	frame_num++;
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
}




Vehicle getVehicle(Vehicle* arr, int i)
{
	for (int m = 0; m < 100; m++)
	{
		if (arr[m].indexi == i)
		{
			return arr[m];
		}
	}
}
bool check_lane_exchange(int* grid, Vehicle* a, Vehicle* b);
bool is_empty_or_not(int *grid, int i, int y, int upto, int x)
{
	for (int k = 0; x + k < y + upto + 1; k++)
	{
		if (i < 0 || i >= road_width || grid[i*road_length + x + k] > 0)
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
void fillOccupancy(int* grid, string* layout, int i, int j, int l, int w, string vChar)
{
	// cout << i << j << l << w << vChar << endl;
	if (i < road_width)
	{
		for (int m = 0; m < w; m++)
		{
			for (int n = l - 1; n >= 0; n--)
			{
				if (j - n >= 0 && j - n < road_length)
				{
					grid[(i + m)*road_length + j - n] = 1;
					layout[(i + m)*road_length + j - n] = vChar;
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


void printLayout(string *layout, Vehicle* array[])
{
	for (int i = 0; i < road_width; i++)
	{

		for (int j = 0; j < road_length; j++)
		{
			if (signal_state == "RED" and j == road_signal)
			{
				cout << "|";
			}
			cout << layout[i*road_length + j];
		}
		cout << endl;
	}
	cout << "------------------------------------------------------------" << endl;
	int i = 0;
	while (array[i] != nullptr)
	{
		xcoord[i] = (2*(float)(array[i]->indexj ) / (float)road_length) - 1;
		ycoord[i] = ((road_width - 1)/2 - array[i]->indexi)*gap;
		float length = ((2*(float)(array[i]->vehicle_length) / (float)road_length)*2)*0.8;	//factor of two for overall scaling of x axis and 0.8 to prevent gaps
		float width = (((array[i]->vehicle_width)*gap) - gap/4)*0.8;	//window scaled by 0.8
		//printf("%f  %f", xcoord[i], ycoord[i]);
		float r1 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r2 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r3 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));

		glPushMatrix();                     // Save model-view matrix setting
		glTranslatef((xcoord[i]*2 - length / 2), (ycoord[i]*0.8 - (width - gap*0.8)/2), 0.0f);    // Translate
		//glRotatef(angle, 0.0f, 0.0f, 1.0f); // rotate by angle in degrees
		glBegin(GL_QUADS);                  // Each set of 4 vertices form a quad
		if(array[i]->color == "BLUE")
		{
			glColor3f(0.0f, 0.0f, 1.0f);     // BLue
		}
		else if (array[i]->color == "GREEN")
		{
			glColor3f(0.0f, 1.0f, 0.0f);     // Green
		}
		else if (array[i]->color == "BLACK")
		{
			glColor3f(1.0f, 0.0f, 0.0f);     // BLack but RED
		}
		else if (array[i]->color == "BROWN")
		{
			glColor3f(1.0f, 1.0f, 0.0f);     // Brown but YELLOW
		}
		else if (array[i]->color == "GREY")
		{
			glColor3f(0.4f, 0.4f, 0.4f);     // Grey
		}
		else if (array[i]->color == "WHITE")
		{
			glColor3f(1.0f, 1.0f, 1.0f);     // White
		}
		else
		{
			glColor3f(0.5f, 0.0f, 0.0f);     // Default CYAN
		}
		glVertex2f((0 - length) / 2, (0 - width) / 2);
		glVertex2f(length / 2, (0 - width) / 2);
		glVertex2f(length / 2, width / 2);
		glColor3f(1.0f, 1.0f, 1.0f);     // white
		glVertex2f((0 - length) / 2, width / 2);
		glEnd();

		glPopMatrix();                      // Restore the model-view matrix
		i++;
	}

	glutSwapBuffers();   // Double buffered - swap the front and back buffers
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

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
		if (signal_state == "GREEN" || (signal_state == "RED" && array[k]->indexj + (array[k]->speed) * 1 < road_signal - 2))
		{
			array[k]->indexj = array[k]->indexj + (array[k]->speed) * 1;
		}
		else
		{
			array[k]->speed = 0;
			// cout << "present sir " << endl;
			bool temp1 = true;
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
				array[k]->indexj = array[k]->indexj;
			}
		}
		k++;
	}

}

void updateVelocity(Vehicle* array[])
{
	int k = 0;
	while (array[k] != nullptr)
	{
		if (signal_state == "GREEN" || (signal_state == "RED" && array[k]->speed != 0))
		{
			if (array[k]->speed + (array[k]->acceleration) * 1 <= array[k]->vehicle_maxspeed)
				array[k]->speed = array[k]->speed + (array[k]->acceleration) * 1;
		}
		bool temp = true;
		for (int l = array[k]->indexj + 1; l < road_length && l > 0; l++)
		{
			if (array[k]->v_char == "RED" || array[k]->indexi*road_length + l < 0 || (array[k]->indexi*road_length + l) > (road_length*road_width))
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
			array[k]->acceleration++;
		}
		k++;
	}
}

void execute(Vehicle* array[], int* grid, string* layout)
{
	updateGrid(array, grid, layout);
	printLayout(layout,array);
	updatePosition(array, grid);
	updateVelocity(array);
	// updateAcceleration(array);

}

void gridOvertakeUpdate(int *grid, int ia, Vehicle *a)
{
	for (int l = 0; l < a->vehicle_length; l++)
	{
		for (int w = 0; w < a->vehicle_width; w++)
		{
			grid[(ia + w)*road_length + a->indexj - l] = 0;
			grid[(a->indexi + w)*road_length + a->indexj - l] = 1;
		}
	}
}

bool indexMatch(int ia, int ib, int aw, int bw)
{
	for (int k = 1 - aw; k < bw; k++)
	{
		if (ia == ib + k)
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
			if (((jb - ja - 1) <= (a->speed - b->speed) * 1) && (jb - ja - 1) >= 1)
			{
				if (isEmptyLane(grid, ib - a->vehicle_width - 1, jb, b->speed, ja, a->vehicle_width) || isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width) || isEmptyLane(grid, ib + a->vehicle_width, jb, b->speed, ja, a->vehicle_width) || (isEmptyLane(grid, ib + a->vehicle_width + 1, jb, b->speed, ja, a->vehicle_width)))
				{
					if (isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib + a->vehicle_width, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib - a->vehicle_width - 1, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib - a->vehicle_width - 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib + a->vehicle_width + 1, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib + a->vehicle_width + 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					// a would change the lane; grid update
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if (isEmptyLane(grid, ib + a->vehicle_width + 2, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib - a->vehicle_width - 2, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					a->speed = b->speed;
					bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
						}
					}

					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;      //to stick them
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
			if (((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed) * 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				if ((isEmptyLane(grid, ib - a->vehicle_width - 1, jb, b->speed, ja, a->vehicle_width) && ia == ib) || isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width) || isEmptyLane(grid, ib + b->vehicle_width + 1, jb, b->speed, ja, a->vehicle_width) || (isEmptyLane(grid, ib + b->vehicle_width + 2, jb, b->speed, ja, a->vehicle_width) && ia == ib + a->vehicle_width))
				{
					if (isEmptyLane(grid, ib + b->vehicle_width + 1, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib + a->vehicle_width + 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib - a->vehicle_width - 1, jb, b->speed, ja, a->vehicle_width) && ia == ib)
					{
						a->indexi = ib - a->vehicle_width - 1;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib + b->vehicle_width + 2, jb, b->speed, ja, a->vehicle_width) && ia == ib + a->vehicle_width)
					{
						a->indexi = ib + a->vehicle_width + 2;
						gridOvertakeUpdate(grid, ia, a);
					}// a would change the lane;
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if ((isEmptyLane(grid, ib + b->vehicle_width + 3, jb, b->speed, ja, a->vehicle_width) && ia == ib + a->vehicle_width) || (isEmptyLane(grid, ib + a->vehicle_width + 2, jb, b->speed, ja, a->vehicle_width) && ia == ib))
					{
						a->indexi = ib + a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if ((isEmptyLane(grid, ib - a->vehicle_width - 1, jb, b->speed, ja, a->vehicle_width) && ia == ib + a->vehicle_width) || (isEmptyLane(grid, ib - a->vehicle_width - 2, jb, b->speed, ja, a->vehicle_width) && ia == ib))
					{
						a->indexi = ib - a->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					a->speed = b->speed;
					bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
						}
					}
					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;
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
			if (((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed) * 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				if ((isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width)) || (isEmptyLane(grid, ib + b->vehicle_width, jb, b->speed, ja, a->vehicle_width)))
				{
					if (isEmptyLane(grid, ib + b->vehicle_width, jb, b->speed, ja, a->vehicle_width))
					{
///						cout << "enter " << endl;
						a->indexi = ib + b->vehicle_width;
						gridOvertakeUpdate(grid, ia, a);
					}
					else if (isEmptyLane(grid, ib - a->vehicle_width, jb, b->speed, ja, a->vehicle_width))
					{
						a->indexi = ib - a->vehicle_width;
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
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						for (int w = 0; w < a->vehicle_width; w++)
						{
							if (grid[(a->indexi + w)*road_length + b->indexj - m] == 1)
								temp1 = false;
						}
					}
					if (temp1 == true)
						a->indexj = b->indexj - b->vehicle_length;
					//a->speed would decrease
					//a->acceleration would decrease
					return false;
				}
			}
		}
	}
	// if(a->vehicle_type != "Bike" and b->vehicle_type == "Bike")
	// {
	//     if(indexMatch(ia,ib,a->vehicle_width,b->vehicle_width))
	//     {
	//         if(((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed)*1) && jb - ja - b->vehicle_length + 1 >= 1)
	//         {
	//             if((isEmptyLane(grid, ib-a->vehicle_width, jb, b->speed,ja,a->vehicle_width)) || (isEmptyLane(grid, ib+b->vehicle_width, jb, b->speed,ja,a->vehicle_width)))
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

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
	glLoadIdentity();               // Reset the model-view matrix

	if (array2[frame - 1] != nullptr)
	{
		int temp = 1;
		if (array2[frame - 1]->v_char == "P")
		{
			int l;
			for (l = frame - 1; array2[l] != nullptr; l++)
			{
				//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
				array2[l] = array2[l + 1];
			}
		}
		else if (array2[frame - 1]->v_char == "RED")
		{
			array2[frame - 1]->indexi = 0;
			array2[frame - 1]->indexj = road_signal - 1;
			array2[frame - 1]->speed = 0;
			array2[frame - 1]->acceleration = 0;
			frame++;
			::signal_state = "RED";
			
		}
		else if (array2[frame - 1]->v_char == "GREEN")
		{
			for (int l = frame - 1; array2[l] != nullptr; l++)
			{
				//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
				array2[l] = array2[l + 1];
			}
			int k = 0;
			::signal_state = "GREEN";
			while (array2[k] != nullptr)
			{
				if (array2[k]->v_char == "RED")
				{
					int l;
					for (l = k; array2[l] != nullptr; l++)
					{
						//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
						array2[l] = array2[l + 1];
					}
					/*array2[k]->indexi = 1000;
					array2[k]->indexj = 1000;*/
					break;
				}
				k++;
			}
		}
		else if (array2[frame - 1]->v_char != "P")
		{
			for (int l = 0; l < road_width; l++)
			{
				temp = 1;
				for (int w = 0; w < array2[frame - 1]->vehicle_width; w++)
				{
					if (grid[(l + w) * road_length] != 0)
					{
						temp = 0;
						break;
					}
				}
				if (temp == 1)
				{
					array2[frame - 1]->indexi = l;
					array2[frame - 1]->indexj = 0;
					array2[frame - 1]->speed = 1;
					array2[frame - 1]->acceleration = array2[frame - 1]->vehicle_acceleration;
					break;
				}

			}
		}
		/*else
		{
			array2[frame - 1]->indexi = road_width;
			array2[frame - 1]->indexj = road_length + 1;
			array2[frame - 1]->speed = 100ffr;
			array2[frame - 1]->acceleration = array2[frame - 1]->vehicle_acceleration;
		}*/
		if (temp == 1)
			frame++;        //if there is no space for the vehicle then reject its enterance
		else
		{
						cout << "----------------------------------------------------------------------------------" << frame << endl;;
			int l;
			for (l = frame-1; array2[l] != nullptr; l++)
			{
				//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
				array2[l] = array2[l+1];
			}
			
		}
	}
	
	
	for (int i = 0; i < road_width; i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);		//white
		glBegin(GL_LINES);
		for (int i = 0; i < 2 * road_width - 1; i++)
		{
			glVertex2f(-2.0f, ((i-road_width+1)*gap/2)*0.8);
			glVertex2f(2.0f, ((i-road_width+1)*gap/2)*0.8);
		}
		glEnd();
	}
	execute(array2, ::grid, ::road_layout);
	


	/*for (int i = 0; (i < 100) && ((array[i]) != nullptr); i++)
	{
		xcoord[i] = ((float)(array[i]->indexj) / (float)road_length);
		ycoord[i] = ((float)(array[i]->indexi) / (float)road_width);
		float length = ((float)(array[i]->vehicle_length) / (float)road_length);
		float width = ((float)(array[i]->vehicle_width) / (float)road_width);

		//printf("%f  %f", xcoord[i], ycoord[i]);

		glPushMatrix();                     // Save model-view matrix setting
		glTranslatef(xcoord[i], ycoord[i], 0.0f);    // Translate
		//glRotatef(angle, 0.0f, 0.0f, 1.0f); // rotate by angle in degrees
		glBegin(GL_QUADS);                  // Each set of 4 vertices form a quad
		glColor3f(1.0f, 0.0f, ((float)i) / 100);     // Red
		glVertex2f((0 - length) / 2, (0 - width) / 2);
		glVertex2f(length / 2, (0 - width) / 2);
		glVertex2f(length / 2, width / 2);
		glVertex2f((0 - length) / 2, width / 2);
		glEnd();
		glPopMatrix();                      // Restore the model-view matrix
	}

	glutSwapBuffers();   // Double buffered - swap the front and back buffers*/

	// Change the rotational angle after each display()
	angle += 2.0f;
	//printf("%f   %f", xcoord, ycoord);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
	Road road;
	int def_maxspeed = 1;
	int def_acceleration = 0;
	string inifile = "config.ini";
	string data;
	vector<vector<string>> v;
	vector<string> pass_v;
	pass_v.push_back("PASS");
	pass_v.push_back("0");
	pass_v.push_back("0");
	pass_v.push_back("0");
	pass_v.push_back("0");
	pass_v.push_back("P");
	v.push_back(pass_v);
	ifstream file;
	file.open(inifile.c_str());
	if (file.is_open())
	{
		while (file >> data)
		{
			if (data == "Road_Id")
			{
				file >> data;
				file >> data;
				road.road_id = stoi(data);
			}
			if (data == "Road_Length")
			{
				file >> data;
				file >> data;
				/*road.road_length = stoi(data);*/
				::road_length = stoi(data);
			}
			if (data == "Road_Width")
			{
				file >> data;
				file >> data;
				/*road_width = stoi(data);*/
				::road_width = stoi(data);
				::gap = (float)((float)1.6 / (float)(road_width - 1));
			}
			if (data == "Road_Signal")
			{
				file >> data;
				file >> data;
				road.road_signal = stoi(data);
				::road_signal = stoi(data);
			}
			if (data == "Default_MaxSpeed")
			{
				file >> data;
				file >> data;
				def_maxspeed = stoi(data);
			}
			if (data == "Default_Acceleration")
			{
				file >> data;
				file >> data;
				def_acceleration = stoi(data);
			}
			if (data == "Vehicle_Type")
			{
				file >> data;
				file >> data;
				//cout << data << endl;
				vector<string> a;
				a.push_back(data);
				a.push_back("0");
				a.push_back("0");
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
				a.push_back("X");
				while (data != "end_v")
				{
					file >> data;
					if (data == "Vehicle_Length")
					{
						file >> data;
						file >> data;
						a[1] = data;
					}
					if (data == "Vehicle_Width")
					{
						file >> data;
						file >> data;
						a[2] = data;
					}
					if (data == "Vehicle_MaxSpeed")
					{
						file >> data;
						file >> data;
						a[3] = data;
					}
					if (data == "Vehicle_Acceleration")
					{
						file >> data;
						file >> data;
						a[4] = data;
					}
					if (data == "Vehicle_Denotion")
					{
						file >> data;
						file >> data;
						a[5] = data;
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


	int dimension = road_length * road_width;
	array2 = new Vehicle*[dimension];
	// array2 = (Vehicle**)malloc(sizeof(Vehicle)*(100));
	for (int i = 0; i < dimension; i++)
	{
		array2[i] = nullptr;
	}
	
	road_layout = new string[dimension];
	
	grid = new int[dimension];
	// grid = (int*)malloc(sizeof(int)*(road_length*road_width));
	for (int i = 0; i < dimension; i++)
	{
		road_layout[i] = "-";
		grid[i] = 0;
	}
	//Vehicle* array2[100] = {nullptr};
	//string road_layout[road.road_length*road.road_width] = {"-"};
	//int grid[road.road_length*road.road_width] = {0};
	
	int i = 0;
	int time = 0;
	ifstream file2;
	file2.open(inifile.c_str());
	string word;
	while (file2 >> word)
	{
		if (word == "START")
		{
			while (file2 >> word)
			{
				for (int j = 1; j < v.size(); j++)
				{
					if (word == v[j][0])
					{
						array2[i] = new Vehicle(stoi(v[j][1]), stoi(v[j][2]), stoi(v[j][3]), stoi(v[j][4]), v[j][0], v[j][5]);
						file2 >> word;
						array2[i]->color = word;
						cout << array2[i]->vehicle_type << " " << i << array2[i]->vehicle_length << " " << array2[i]->vehicle_width << " " << array2[i]->vehicle_maxspeed << " " << array2[i]->vehicle_acceleration << " " << array2[i]->color << " " << array2[i]->vehicle_type << endl;
						cout << array2[i]->v_char << endl;
						/*for (int l = 0; l < road_width; l++)
						{
							int temp = 1;
							for (int w = 0; w < array2[i]->vehicle_width; w++)
							{
								if (grid[(l + w) * road_length] != 0)
								{
									temp = 0;
									break;
								}
							}
							if (temp == 1)
							{
								array2[i]->indexi = l;
								array2[i]->indexj = 0;
								array2[i]->speed = 1;
								array2[i]->acceleration = array2[i]->vehicle_acceleration;
								break;
							}
						}*/
						i++;
						//execute(array, grid, road_layout);
						time++;
						cout << "time = " << time << endl;;


					}
				}

				if (word == "Pass")
				{
					cout << "pass stage" << endl;;
					file2 >> word;
					int counter = stoi(word);
					for (int c = 0; c < counter - 1; c++)
					{
						array2[i] = new Vehicle(stoi(v[0][1]), stoi(v[0][2]), stoi(v[0][3]), stoi(v[0][4]), v[0][0], v[0][5]);//execute(array, grid, road_layout);
						time++;
						cout << time << endl;;
						i++;
					}
				}
				if (word == "Signal")
				{
					file2 >> word;
					cout << "Signal changed to " << word << endl;
					if (word == "RED")
					{
						array2[i] = new Vehicle(1, road_width, 0, 0, "RED", "RED");
						i++;
					}
					if (word == "GREEN")
					{
						array2[i] = new Vehicle(0, 0, 0, 0, "GREEN", "GREEN");
						i++;
					}
					road.signal_state = word;
					/*::signal_state = word;*/
				}
			}
		}
	}
	file2.close();

	/*
	array2[i] = new Vehicle(2, 2, 3, 1, "Car", "C");
	array2[i + 1] = new Vehicle(2, 1, 6, 1, "Bike", "b");
	array2[i]->speed = 3;
	array2[i]->indexi = 2;
	array2[i]->indexj = 5;
	grid[2 * road_length + 5] = 1;
	grid[2 * road_length + 4] = 1;
	//grid[1*road_length + 3] = 1;
	grid[3 * road_length + 5] = 1;
	grid[3 * road_length + 4] = 1;
	//grid[2*road_length + 3] = 1;
	array2[i + 1]->indexi = 3;
	array2[i + 1]->indexj = 3;
	array2[i + 1]->speed = 5;
	grid[3 * road_length + 3] = 1;
	grid[3 * road_length + 2] = 1;
	//grid[3*road_length + 2] = 1;
	//grid[3*road_length + 1] = 1;
	grid[4 * road_length + 5] = 1;
	grid[4 * road_length + 4] = 1;
	grid[1 * road_length + 5] = 1;
	grid[1 * road_length + 4] = 1;
	cout << boolalpha; //for printing true or false;
	for (int l = 0; l < 5; l++)
	{
		for (int k = 0; k < road_length; k++)
		{
			cout << grid[l*road_length + k];
		}
		cout << endl;
	}
	cout << "checking lane " << check_lane_exchange(grid, array2[i + 1], array2[i]) << endl;
	for (int l = 0; l < 5; l++)
	{
		for (int k = 0; k < road_length; k++)
		{
			cout << grid[l*road_length + k];
		}
		cout << endl;
	}
	cout << "checking lane again" << check_lane_exchange(grid, array2[i + 1], array2[i]) << endl;*/

	//-------------------------------------------------------------------------------------------------------------------------------------------------------

	glutInit(&argc, argv);          // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
	glutInitWindowSize(1560, 960);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
	glutCreateWindow("Animation via Idle Function");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutTimerFunc(0, Timer, 0);     // First timer call immediately
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
