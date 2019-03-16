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
int counter = 1;

string signal_state;
int road_signal;
const int road_length = 30;
const int road_width = 5;


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


Vehicle* array[100] = { nullptr };
string road_layout[(::road_length)*(::road_width)] = { "-" };
int grid[30 * 5] = { 0 };

/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}


/* Called back when timer expired */
void Timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, Timer, 0); // next Timer call milliseconds later
	printf("%d", frame_num);
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
		if (i < 0 || i >= 5 || grid[i*30 + x + k] > 0)
			return false;
	}
	return true;
}

void fillOccupancy(int* grid, string* layout, int i, int j, int l, int w, string vChar)
{
	// cout << i << j << l << w << vChar << endl;
	if (i < 5)
	{
		for (int m = 0; m < w; m++)
		{
			for (int n = l - 1; n >= 0; n--)
			{
				if (j - n >= 0 && j - n < 30)
				{
					grid[(i + m)*30 + j - n] = 1;
					layout[(i + m)*30 + j - n] = vChar;
				}
			}
		}
	}
}

void updateGrid(Vehicle* array[], int* grid, string* layout)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			grid[i*30 + j] = 0;
			layout[i*30 + j] = "-";
		}
	}
	int k = 0;
	while (array[k] != nullptr)
	{
		if (array[k]->indexi >= 0 && array[k]->indexi < 30)
		{
			fillOccupancy(grid, layout, array[k]->indexi, array[k]->indexj, array[k]->vehicle_length, array[k]->vehicle_width, array[k]->v_char);
		}
		k++;
	}

}

void printLayout(string *layout, Vehicle* array[])
{
	for (int i = 0; i < 5; i++)
	{

		for (int j = 0; j < 30; j++)
		{
			if (::signal_state == "RED" and j == ::road_signal)
			{
				cout << "|";
			}
			cout << layout[i*30 + j];
		}
		cout << endl;
	}
	cout << "------------------------------------------------------------" << endl;
	int i = 0;
	while (array[i] != nullptr)
	{
		xcoord[i] = ((float)(array[i]->indexj ) / (float)30);
		ycoord[i] = ((float)(4-(array[i]->indexi)) / (float)5);
		float length = ((float)(array[i]->vehicle_length) / (float)30);
		float width = ((float)(array[i]->vehicle_width) / (float)20);

		//printf("%f  %f", xcoord[i], ycoord[i]);
		float r1 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r2 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r3 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));

		glPushMatrix();                     // Save model-view matrix setting
		glTranslatef(xcoord[i] - length / 2, ycoord[i] + width / 2, 0.0f);    // Translate
		//glRotatef(angle, 0.0f, 0.0f, 1.0f); // rotate by angle in degrees
		glBegin(GL_QUADS);                  // Each set of 4 vertices form a quad
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex2f((0 - length) / 2, (0 - width) / 2);
		glVertex2f(length / 2, (0 - width) / 2);
		glVertex2f(length / 2, width / 2);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex2f((0 - length) / 2, width / 2);
		glEnd();
		glPopMatrix();                      // Restore the model-view matrix
		i++;
	}
	glutSwapBuffers();   // Double buffered - swap the front and back buffers
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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
		if (::signal_state == "GREEN" || (::signal_state == "RED" && array[k]->indexj + (array[k]->speed) * 1 < ::road_signal))
		{
			array[k]->indexj = array[k]->indexj + (array[k]->speed) * 1;
		}
		else
		{
			array[k]->speed = 0;
			array[k]->indexj = ::road_signal - 1;
		}
		k++;
	}

}

void updateVelocity(Vehicle* array[])
{
	int k = 0;
	while (array[k] != nullptr)
	{
		if (::signal_state == "GREEN" || (::signal_state == "RED" && array[k]->speed != 0))
		{
			if (array[k]->speed + (array[k]->acceleration) * 1 <= array[k]->vehicle_maxspeed)
				array[k]->speed = array[k]->speed + (array[k]->acceleration) * 1;
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

bool check_lane_exchange(int* grid, Vehicle* a, Vehicle* b)   //a is the vehicle behind and b is the leading vehicle
{
	int ia = a->indexi;
	int ib = b->indexi;
	int ja = a->indexj;
	int jb = b->indexj;
	if (a->vehicle_type == "Bike" and b->vehicle_type == "Bike")
	{
		if (ia == ib)
		{
			if (((jb - ja - 1) <= (a->speed - b->speed) * 1) && (jb - ja - 1) >= 1)
			{
				if (is_empty_or_not(grid, ib - 2, jb, b->speed, ja) || is_empty_or_not(grid, ib - 1, jb, b->speed, ja) || is_empty_or_not(grid, ib + 1, jb, b->speed, ja) || (is_empty_or_not(grid, ib + 2, jb, b->speed, ja)))
				{
					if (is_empty_or_not(grid, ib - 1, jb, b->speed, ja))
					{
						a->indexi = ib - 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib + 1, jb, b->speed, ja))
					{
						a->indexi = ib + 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 2, jb, b->speed, ja))
					{
						a->indexi = ib - 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib + 2, jb, b->speed, ja))
					{
						a->indexi = ib + 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					// a would change the lane; grid update
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if (is_empty_or_not(grid, ib + 3, jb, b->speed, ja))
					{
						a->indexi = ib + 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 3, jb, b->speed, ja))
					{
						a->indexi = ib - 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					a->speed = b->speed;
					bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						if (grid[a->indexi*30 + b->indexj - m] == 1)
							temp1 = false;
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
		if (ia == ib || ia == ib + 1)
		{
			if (((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed) * 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				if ((is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && ia == ib) || is_empty_or_not(grid, ib - 1, jb, b->speed, ja) || is_empty_or_not(grid, ib + 2, jb, b->speed, ja) || (is_empty_or_not(grid, ib + 3, jb, b->speed, ja) && ia == ib + 1))
				{
					if (is_empty_or_not(grid, ib + 2, jb, b->speed, ja))
					{
						a->indexi = ib + 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && ia == ib)
					{
						a->indexi = ib - 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 1, jb, b->speed, ja))
					{
						a->indexi = ib - 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib + 3, jb, b->speed, ja) && ia == ib + 1)
					{
						a->indexi = ib + 3;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}// a would change the lane;
					//during changing lane, acceleration would be zero
					return true;
				}
				else
				{
					if ((is_empty_or_not(grid, ib + 4, jb, b->speed, ja) && ia == ib + 1) || (is_empty_or_not(grid, ib + 3, jb, b->speed, ja) && ia == ib))
					{
						a->indexi = ib + 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					else if ((is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && ia == ib + 1) || (is_empty_or_not(grid, ib - 3, jb, b->speed, ja) && ia == ib))
					{
						a->indexi = ib - 1;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
					}
					a->speed = b->speed;
					bool temp1 = true;
					for (int m = b->vehicle_length; jb - m > ja; m++)
					{
						if (grid[a->indexi*30 + b->indexj - m] == 1)
							temp1 = false;
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

	if (a->vehicle_type != "Bike" and b->vehicle_type != "Bike")
	{
		if (ia == ib || ia == ib + 1 || ia == ib - 1)
		{
			if (((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed) * 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				if ((is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && is_empty_or_not(grid, ib - 1, jb, b->speed, ja)) || (is_empty_or_not(grid, ib + 2, jb, b->speed, ja) && (is_empty_or_not(grid, ib + 3, jb, b->speed, ja))))
				{
					if (is_empty_or_not(grid, ib + 2, jb, b->speed, ja) && (is_empty_or_not(grid, ib + 3, jb, b->speed, ja)))
					{
						cout << "enter " << endl;
						a->indexi = ib + 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[(ia + 1)*30 + ja] = 0;
						grid[(ia + 1)*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
						grid[(a->indexi + 1)*30 + ja] = 1;
						grid[(a->indexi + 1)*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && is_empty_or_not(grid, ib - 1, jb, b->speed, ja))
					{
						a->indexi = ib - 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[(ia + 1)*30 + ja] = 0;
						grid[(ia + 1)*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
						grid[(a->indexi + 1)*30 + ja] = 1;
						grid[(a->indexi + 1)*30 + ja - 1] = 1;
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
						if (grid[a->indexi*30 + b->indexj - m] == 1)
							temp1 = false;
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
	if (a->vehicle_type != "Bike" and b->vehicle_type == "Bike")
	{
		if (ia == ib || ia == ib + 1 || ia == ib - 1)
		{
			if (((jb - ja - b->vehicle_length + 1) <= (a->speed - b->speed) * 1) && jb - ja - b->vehicle_length + 1 >= 1)
			{
				if ((is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && is_empty_or_not(grid, ib - 1, jb, b->speed, ja)) || (is_empty_or_not(grid, ib + 2, jb, b->speed, ja) && (is_empty_or_not(grid, ib + 3, jb, b->speed, ja))))
				{
					if (is_empty_or_not(grid, ib + 2, jb, b->speed, ja) && (is_empty_or_not(grid, ib + 3, jb, b->speed, ja)))
					{
						cout << "enter " << endl;
						a->indexi = ib + 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[(ia + 1)*30 + ja] = 0;
						grid[(ia + 1)*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
						grid[(a->indexi + 1)*30 + ja] = 1;
						grid[(a->indexi + 1)*30 + ja - 1] = 1;
					}
					else if (is_empty_or_not(grid, ib - 2, jb, b->speed, ja) && is_empty_or_not(grid, ib - 1, jb, b->speed, ja))
					{
						a->indexi = ib - 2;
						grid[ia*30 + ja] = 0;
						grid[ia*30 + ja - 1] = 0;
						grid[(ia + 1)*30 + ja] = 0;
						grid[(ia + 1)*30 + ja - 1] = 0;
						grid[a->indexi*30 + ja] = 1;
						grid[a->indexi*30 + ja - 1] = 1;
						grid[(a->indexi + 1)*30 + ja] = 1;
						grid[(a->indexi + 1)*30 + ja - 1] = 1;
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
						if (grid[a->indexi*30 + b->indexj - m] == 1)
							temp1 = false;
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
	return false;
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
	glLoadIdentity();               // Reset the model-view matrix

	if (::array[counter - 1] != nullptr)
	{
		for (int l = 0; l < 5; l++)
		{
			int temp = 1;
			for (int w = 0; w < ::array[counter - 1]->vehicle_width; w++)
			{
				if (grid[(l + w) * 30] != 0)
				{
					temp = 0;
					break;
				}
			}
			if (temp == 1)
			{
				::array[counter - 1]->indexi = l;
				::array[counter - 1]->indexj = 0;
				::array[counter - 1]->speed = 1;
				::array[counter - 1]->acceleration = ::array[counter - 1]->vehicle_acceleration;
				break;
			}
		}
	}
	
	counter++;
	execute(::array, ::grid, ::road_layout);
	


	/*for (int i = 0; (i < 100) && ((array[i]) != nullptr); i++)
	{
		xcoord[i] = ((float)(array[i]->indexj) / (float)30);
		ycoord[i] = ((float)(array[i]->indexi) / (float)5);
		float length = ((float)(array[i]->vehicle_length) / (float)30);
		float width = ((float)(array[i]->vehicle_width) / (float)5);

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
				/*road.road_length = stoi(data);
				::road_length = stoi(data);*/
			}
			if (data == "Road_Width")
			{
				file >> data;
				file >> data;
				/*5 = stoi(data);
				::road_width = stoi(data);*/
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
				for (int j = 0; j < v.size(); j++)
				{
					if (word == v[j][0])
					{
						::array[i] = new Vehicle(stoi(v[j][1]), stoi(v[j][2]), stoi(v[j][3]), stoi(v[j][4]), v[j][0], v[j][5]);
						file2 >> word;
						::array[i]->color = word;
						cout << ::array[i]->vehicle_type << " " << i << ::array[i]->vehicle_length << " " << ::array[i]->vehicle_width << " " << ::array[i]->vehicle_maxspeed << " " << ::array[i]->vehicle_acceleration << " " << ::array[i]->color << " " << ::array[i]->vehicle_type << endl;
						cout << ::array[i]->v_char << endl;
						/*for (int l = 0; l < 5; l++)
						{
							int temp = 1;
							for (int w = 0; w < ::array[i]->vehicle_width; w++)
							{
								if (grid[(l + w) * 30] != 0)
								{
									temp = 0;
									break;
								}
							}
							if (temp == 1)
							{
								::array[i]->indexi = l;
								::array[i]->indexj = 0;
								::array[i]->speed = 1;
								::array[i]->acceleration = ::array[i]->vehicle_acceleration;
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
					for (int i = 0; i < counter - 1; i++)
					{
						//execute(array, grid, road_layout);
						time++;
						cout << time << endl;;
					}
				}
				if (word == "Signal")
				{
					file2 >> word;
					cout << "Signal changed to " << word << endl;
					road.signal_state = word;
					::signal_state = word;
				}


			}
		}
	}
	file2.close();

	/*
	::array[i] = new Vehicle(2, 2, 3, 1, "Car", "C");
	::array[i + 1] = new Vehicle(2, 1, 6, 1, "Bike", "b");
	::array[i]->speed = 3;
	::array[i]->indexi = 2;
	::array[i]->indexj = 5;
	grid[2 * 30 + 5] = 1;
	grid[2 * 30 + 4] = 1;
	//grid[1*30 + 3] = 1;
	grid[3 * 30 + 5] = 1;
	grid[3 * 30 + 4] = 1;
	//grid[2*30 + 3] = 1;
	::array[i + 1]->indexi = 3;
	::array[i + 1]->indexj = 3;
	::array[i + 1]->speed = 5;
	grid[3 * 30 + 3] = 1;
	grid[3 * 30 + 2] = 1;
	//grid[3*30 + 2] = 1;
	//grid[3*30 + 1] = 1;
	grid[4 * 30 + 5] = 1;
	grid[4 * 30 + 4] = 1;
	grid[1 * 30 + 5] = 1;
	grid[1 * 30 + 4] = 1;
	cout << boolalpha; //for printing true or false;
	for (int l = 0; l < 5; l++)
	{
		for (int k = 0; k < 30; k++)
		{
			cout << grid[l*30 + k];
		}
		cout << endl;
	}
	cout << "checking lane " << check_lane_exchange(grid, ::array[i + 1], ::array[i]) << endl;
	for (int l = 0; l < 5; l++)
	{
		for (int k = 0; k < 30; k++)
		{
			cout << grid[l*30 + k];
		}
		cout << endl;
	}
	cout << "checking lane again" << check_lane_exchange(grid, ::array[i + 1], ::array[i]) << endl;*/

	//-------------------------------------------------------------------------------------------------------------------------------------------------------

	glutInit(&argc, argv);          // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
	glutInitWindowSize(1280, 960);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Animation via Idle Function");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutTimerFunc(0, Timer, 0);     // First timer call immediately
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
