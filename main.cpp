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
#include "functions.h"

#include "pthread.h"
using namespace std;
using std::string;

 // global variable
GLfloat angle = 0.0f;  // rotational angle of the shapes
GLfloat xcoord[100];
GLfloat ycoord[100];
int refreshMills = 10; // refresh interval in milliseconds
int frame_num = 0;
int frame = 1;
int def_maxspeed;
int def_acceleration;
bool blink = true;
string signal_state = "GREEN";
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

Vehicle** array2;
//Vehicle* array2[100] = { nullptr };
//string road_layout[(::road_length)*(::road_width)] = { "-" };
//int grid[road_length * road_width] = { 0 };

/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // Black and opaque
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

		if (array[i]->v_char == "A" && signal_state == "RED" && array[i]->speed == 0)
		{
			int ia = array[i]->indexi;
			int ja = array[i]->indexj;
			bool temp1 = true;
			bool temp2 = true;
			bool temp3 = true;
			bool temp4 = true;
			bool temp5 = true;
			for (int z = 1; z < array[i]->vehicle_length / 3; z++)
			{
				if (ia*road_length + ja + z > 0 && ia*road_length + ja + z < road_length*road_width && (grid[ia*road_length + ja + z] != 0 ))
				{
					cout << "loop1" << endl;
					temp1 = false;
				}
				if (ia*road_length + ja + z > 0 && ia*road_length + ja + z < road_length*road_width && (grid[(ia + 2)*road_length + ja + z] != 0 ))
				{
					temp2 = false;
				}
				if (ia*road_length + ja + z > 0 && ia*road_length + ja + z < road_length*road_width && (grid[(ia + 1)*road_length + ja + z] != 0))
				{
					temp3 = false;
				}
				if (ia*road_length + ja + z > 0 && ia*road_length + ja + z < road_length*road_width && (grid[(ia - 1)*road_length + ja + z] != 0))
				{
					temp4 = false;
				}
				if (ia*road_length + ja + z > 0 && ia*road_length + ja + z < road_length*road_width && (grid[(ia + 3)*road_length + ja + z] != 0))
				{
					temp5 = false;
				}
				if (temp1 == temp2 == temp3 == temp4 == temp5 == false)
				{
					break;
				}
			}
			if (temp1 == true)
			{
				if(temp3 == true)
				{
					if(array[i]->indexi >= 1)
					array[i]->indexi = array[i]->indexi - 1;
				}
				xcoord[i] = (2 * (float)((array[i]->indexj + array[i]->vehicle_length / 3)) / (float)road_length) - 1;
				ycoord[i] = ((road_width - 1) / 2 - array[i]->indexi)*gap;
				::angle = 35;

			}
			else if (temp2 == true)
			{
				if(temp5 == true)
				{
					if(array[i]->indexi < road_width - 1)
					array[i]->indexi = array[i]->indexi + 1;
				}
				xcoord[i] = (2 * (float)((array[i]->indexj + array[i]->vehicle_length / 3)) / (float)road_length) - 1;
				ycoord[i] = ((road_width - 1) / 2 - array[i]->indexi)*gap;
				::angle = -35;
			}
			else if (temp3 == true)
			{
				xcoord[i] = (2 * (float)((array[i]->indexj + array[i]->vehicle_length / 3)) / (float)road_length) - 1;
				ycoord[i] = ((road_width - 1) / 2 - array[i]->indexi)*gap;
				::angle = 0;
			}
			else
			{
				xcoord[i] = (2 * (float)(array[i]->indexj) / (float)road_length) - 1;
				ycoord[i] = ((road_width - 1) / 2 - array[i]->indexi)*gap;
				::angle = 0;
			}
		}
		else
		{
			xcoord[i] = (2 * (float)(array[i]->indexj) / (float)road_length) - 1;
			ycoord[i] = ((road_width - 1) / 2 - array[i]->indexi)*gap;
			::angle = 0;
		}

		float length = ((2 * (float)(array[i]->vehicle_length) / (float)road_length) * 2)*0.8;	//factor of two for overall scaling of x axis and 0.8 to prevent gaps
		float width = (((array[i]->vehicle_width)*gap) - gap / 4)*0.8;	//window scaled by 0.8
		//printf("%f  %f", xcoord[i], ycoord[i]);
		float r1 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r2 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		float r3 = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0)));
		if (array[i]->vehicle_type == "RED") { int nis = 0; }
		glPushMatrix();                     // Save model-view matrix setting
		glTranslatef((xcoord[i] * 2 - length / 2), (ycoord[i] * 0.8 - (width - gap * 0.8) / 2), 0.0f);    // Translate
		glRotatef(angle, 0.0f, 0.0f, 1.0f); // rotate by angle in degrees
		glBegin(GL_POLYGON);                  // Each set of 4 vertices form a quad
		if (array[i]->color == "BLUE")
		{
			glColor3f(0.0f, 0.0f, 1.0f);     // BLue
		}
		else if (array[i]->color == "GREEN")
		{
			glColor3f(0.0f, 1.0f, 0.0f);     // Green
		}
		else if (array[i]->color == "RED")
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

		if (array[i]->v_char != "A")
		{
			if ((array[i]->v_char == "RED" && blink) || (array[i]->v_char != "RED"))
			{
				glVertex2f((0 - length) / 2, (0 - width) / 2);
				glVertex2f(length / 2, (0 - width) / 2);
				glVertex2f(length / 2, width / 2);
				glColor3f(1.0f, 1.0f, 1.0f);     // white
				glVertex2f((0 - length) / 2, width / 2);
			}
		}
		else
		{																	//vertices for the auto
			glVertex2f((0 - length) / 2, (0 - width) / 2);
			glVertex2f((0 - length) / 2, width / 2);
			glColor3f(1.0f, 1.0f, 1.0f);     // white
			glVertex2f(length / 2, 0);
		}
		glEnd();
		//glRotatef(20,1.0f,1.0f,1.0f);
		glPopMatrix();                      // Restore the model-view matrix
		i++;
	}

	glutSwapBuffers();   // Double buffered - swap the front and back buffers
	Sleep(300);
	blink = not blink;
	// std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
void execute(Vehicle* array[], int* grid, string* layout)
{
	updateGrid(array, grid, layout);
	printLayout(layout,array);
	updatePosition(array, grid);
	updateVelocity(array, grid);
	// updateAcceleration(array);

}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
   void display() {
   	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
   	glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
   	glLoadIdentity();               // Reset the model-view matrix
   	int passcounter = 0;

   	if (array2[frame - 1] != nullptr)
   	{
   		int temp = 1;
   		if (array2[frame - 1]->v_char == "P")
   		{
   			passcounter = array2[frame - 1]->vehicle_maxspeed - 1;
   			//no changes for arra2[frame-1]->vehicle_maxspeed time units.
   			array2[frame - 1]->vehicle_maxspeed--;
   			temp = 0;
   			if (passcounter == 0)
   			{
   				int l;
   				/*for (l = frame - 1; array2[l] != nullptr; l++)
   				{
   					//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
   					array2[l] = array2[l + 1];
   				}*/
   				temp = 1;
   			}

   		}
   		else if (array2[frame - 1]->v_char == "RED")
   		{
   			array2[frame - 1]->indexi = 0;
   			array2[frame - 1]->indexj = road_signal - 1;
   			array2[frame - 1]->speed = 0;
   			array2[frame - 1]->acceleration = 0;
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
   					array2[frame - 1]->speed = def_maxspeed;
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
   			if (passcounter == 0)
   			{
   				int l;
   				for (l = frame - 1; array2[l] != nullptr; l++)
   				{
   					//cout << "went ouuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuut" << endl;
   					array2[l] = array2[l + 1];
   				}
   			}
   		}
   	}


   	for (int i = 0; i < road_width; i++)
   	{
   		glColor3f(1.0f, 1.0f, 1.0f);		//white
   		glBegin(GL_LINES);
   		/*for (int i = 0; i < 2 * road_width - 1; i++)
   		{
   			glVertex2f(-2.0f, ((i-road_width+1)*gap)*0.8);
   			glVertex2f(2.0f, ((i-road_width+1)*gap)*0.8);
   		}*/
   		glVertex2f(-2.0f, -0.85);
   		glVertex2f(2.0f, -0.85);
   		glVertex2f(-2.0f, 0.8);
   		glVertex2f(2.0f, 0.8);
   		glEnd();

		float zebra_length = ((2 * (float)(road_length/30) / (float)road_length) * 2)*0.8;	//factor of two for overall scaling of x axis and 0.8 to prevent gaps
		float zabra_width = ((gap*gap) - gap / 4)*0.8;	//window scaled by 0.8
		glColor3f(0.7f, 0.7f, 0.7f);		//trans white
		glBegin(GL_QUADS);
		for (int i = 0; i < road_width/2; i++)
		{
			glVertex2f((2 * (float)(road_signal + road_length / 5) / (float)road_length) - 1, ((road_width - 1) / 2 - 2*i)*gap);
			glVertex2f(0, ((road_width - 1) / 2 - 2 * i)*gap);
			glVertex2f(0, ((road_width - 1) / 2 - 2 * i - 1)*gap);
			glVertex2f((2 * (float)(road_signal + road_length / 5) / (float)road_length) - 1, ((road_width - 1) / 2 - 2 * i - 1)*gap);
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
   //	angle += 2.0f;
   	//printf("%f   %f", xcoord, ycoord);
   }

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
	Road road;
	string inifile = argv[1];
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
				::def_maxspeed = stoi(data);
			}
			if (data == "Default_Acceleration")
			{
				file >> data;
				file >> data;
				::def_acceleration = stoi(data);
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
				if(word == "#")
				{
					string line;
					getline(file2, line);
				}
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

					array2[i] = new Vehicle(stoi(v[0][1]), stoi(v[0][2]), counter, stoi(v[0][4]), v[0][0], v[0][5]);//execute(array, grid, road_layout);
					i++;
				}
				if (word == "Signal")
				{
					file2 >> word;
					cout << "Signal changed to " << word << endl;
					if (word == "RED")
					{
						array2[i] = new Vehicle(road_length/60, road_width, 0, 0, "RED", "RED");
						array2[i]->color = "RED";
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

	//-------------------------------------------------------------------------------------------------------------------------------------------------------

	glutInit(&argc, argv);          // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
	glutInitWindowSize(1560, 960);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
	glutCreateWindow("Indian Road Traffic Simulator");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutTimerFunc(0, Timer, 0);     // First timer call immediately
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
