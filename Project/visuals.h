


//-------- Functions --------------------------------

struct point
{
	float x;
	float y;
	float z;
};

struct face
{
	int vtx[3];
	int norm[3];
};

struct model
{
	point obj_points[10000];
	point obj_normals[10000];
	face obj_faces[20000];
	int vertices;
	int normals;
	int faces;
};

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Mouse(int button, int state, int x, int y);
// Function for handling mouse events

void Idle();
//Animation part

void Keyboard(unsigned char key, int x, int y);
// Function for handling keyboard events.

void ReadFile(model*);
//Function for reading a model file

void DisplayModel(model);
// Function for displaying a model
