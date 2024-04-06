#include<Windows.h>    
// first include Windows.h header file which is required      
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder     
#include<cmath>
#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <fstream>

#define RES                 1
#define WIDTH               (160 * RES)
#define HEIGHT              (120 * RES)
#define SW2                 (WIDTH/2)
#define SH2                 (HEIGHT/2)
#define PIXEL_SCALE         (4/RES)
#define GLSW                (WIDTH * PIXEL_SCALE)
#define GLSH                (HEIGHT * PIXEL_SCALE)
#define M_PI                3.14159

struct mathType
{
    float sin[360];
    float cos[360];
};

struct playerType
{
    int x, y, z;
    int r;              //rotation left and right
    int l;              //look up and down
};

struct keyType
{
    int w, a, s, d;     //movement keys
    int m;              //modifier
    int sl, sr;         //strafe
};

struct vertexType
{
    int x, y, z;
};

struct wallType
{
    vertexType verts[4];
    int color;
    float distance;
};

struct sectorType
{
    std::list<wallType> walls;
    float distance;
    int numWalls;
    int topColor;
    int bottomColor;
};

int lastTickCount, startTime;
int frameCount;
std::stringstream ssFrameCount;
mathType math;
playerType player;
keyType keys;
std::list<wallType*> walls;
std::list<sectorType> sectors;

float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//to sort the list of walls
bool compareWallDist(const wallType& first, const wallType& second)
{
    return (first.distance > second.distance);
}

bool compareSectorDist(const sectorType& first, const sectorType& second)
{
    return (first.distance > second.distance);
}

void loadWallsv2()
{
    //The format of the file is
    //version of the file (this is to load version 2)
    //number of sectors
    //number of walls
    //color of the top and color of the bottom
    //color of the wall
    //x y z verts of each wall
    //repeat of everything
    sectors.clear();

    std::ifstream wallFile;
    std::string line;
    int vertCount = 0;
    int compCount = 0;
    bool colorExpected = true;
    bool firstThrough = true;
    bool numSectorsExp = true;
    bool numWallsExpected = true;
    int topColorExpected = true;
    int bottomColorExpected = true;
    int numSectors;
    int numWalls;
    int sectorCount;
    int wallCount;
    sectorType curSector;
    wallType curWall;
    int temp;
    wallFile.open("wallsv2.txt");

    if (wallFile.is_open())
    {
        while (wallFile.good())
        {
            wallFile >> temp;
            if (firstThrough)
            {
                //should be the version 2 of this file
                if (temp != 2)
                {
                    std::cout << "File version " << temp << " is incorrect, expecting version 2" << std::endl;
                    wallFile.close();
                    return;
                }
                firstThrough = false;
            }
            else if (numSectorsExp)
            {
                numSectors = temp;
                sectorCount = 0;
                numSectorsExp = false;
            }
            else if (numWallsExpected)
            {
                numWalls = temp;
                curSector.numWalls = numWalls;
                wallCount = 0;
                numWallsExpected = false;
            }
            else if (topColorExpected)
            {
                curSector.topColor = temp;
                topColorExpected = false;
            }
            else if (bottomColorExpected)
            {
                curSector.bottomColor = temp;
                bottomColorExpected = false;
            }
            else if (colorExpected)
            {
                curWall.color = temp;
                colorExpected = false;
            }
            else
            {
                compCount++;

                switch (compCount)
                {
                case 1:
                    curWall.verts[vertCount].x = temp;
                    break;
                case 2:
                    curWall.verts[vertCount].y = temp;
                    break;
                case 3:
                    curWall.verts[vertCount].z = temp;
                    vertCount++;
                    compCount = 0;

                    if (vertCount == 4)
                    {
                        vertCount = 0;
                        colorExpected = true;
                        curWall.distance = 0;
                        curSector.walls.push_back(curWall);
                        wallCount++;
                        if (wallCount >= numWalls)
                        {
                            numWallsExpected = true;
                            topColorExpected = true;
                            bottomColorExpected = true;
                            sectorCount++;
                            curSector.distance = 0;
                            sectors.push_back(curSector);
                            curSector.walls.clear();
                        }
                    }

                    break;
                }
            }
        }
        wallFile.close();
    }
}

void loadWalls()
{
    if (!walls.empty())
    {
        for (auto const& wallToDelete : walls)
        {
            delete wallToDelete;
        }

        walls.clear();
    }
    std::ifstream wallFile;
    std::string line;
    int vertCount = 0;
    int compCount = 0;
    bool colorExpected = true;
    wallType* curWall = NULL;
    int temp;
    wallFile.open("walls.txt");

    if (wallFile.is_open())
    {
        while (wallFile.good())
        {
            wallFile >> temp;

            if (colorExpected)
            {
                colorExpected = false;
                if (curWall)
                    walls.push_back(curWall);

                curWall = new wallType();
                curWall->color = temp;
                vertCount = 0;
            }
            else
            {
                compCount++;

                switch (compCount)
                {
                case 1:
                    curWall->verts[vertCount].x = temp;
                    break;
                case 2:
                    curWall->verts[vertCount].y = temp;
                    break;
                case 3:
                    curWall->verts[vertCount].z = temp;
                    vertCount++;
                    compCount = 0;

                    if (vertCount == 4)
                    {
                        vertCount = 0;
                        colorExpected = true;
                    }

                    break;
                }
            }
        }

        if (curWall)
            walls.push_back(curWall);

        wallFile.close();
    }
}
// Init_OpenGL() function    
void Init_OpenGL()
{
    // set background color to Black    
    glClearColor(0.0, 0.24, 0.51, 0.0);
    // set shade model to Flat    
    //glShadeModel(GL_FLAT);
    lastTickCount = glutGet(GLUT_ELAPSED_TIME);
    startTime = lastTickCount;
    glPointSize(PIXEL_SCALE);
    //gluOrtho2D(0, 0, WIDTH, HEIGHT);
    //THIS IS IMPORTANT, THE LINE ABOVE WAS INCORRECT ORDER
    gluOrtho2D(0, GLSW, 0, GLSH);
    frameCount = 0;
    ssFrameCount << frameCount;

    for (int i = 0; i < 360; i++)
    {
        math.sin[i] = sin(i / 180.0 * M_PI);
        math.cos[i] = cos(i / 180.0 * M_PI);
    }

    player.x = 70;
    player.y = -110;
    player.z = 20;
    player.r = 0;
    player.l = 0;

    loadWalls();
    loadWallsv2();
}

void movePlayer()
{
    //rotate left
    if (keys.a && !keys.m) { player.r -= 4; if (player.r < 0) { player.r += 360; } }
    //rotate right
    if (keys.d && !keys.m) { player.r += 4; if (player.r >= 360) { player.r -= 360; } }
    //get the amount of delta for the x and y
    int dx = math.sin[player.r] * 10.0;
    int dy = math.cos[player.r] * 10.0;
    //move forward
    if (keys.w && !keys.m) { player.x += dx; player.y += dy; }
    //move backwards
    if (keys.s && !keys.m) { player.x -= dx; player.y -= dy; }

    //strafe left
    if (keys.sl) { player.x -= dy; player.y += dx; }
    //strafe right
    if (keys.sr) { player.x += dy; player.y -= dx; }

    if (keys.a && keys.m) { player.l -= 1; }
    if (keys.d && keys.m) { player.l += 1; }
    if (keys.w && keys.m) { player.z -= 4; }
    if (keys.s && keys.m) { player.z += 4; }
}

void keyDown(unsigned char key, int x, int y)
{
    if (key == 'w') { keys.w = 1; }
    if (key == 's') { keys.s = 1; }
    if (key == 'd') { keys.d = 1; }
    if (key == 'a') { keys.a = 1; }
    if (key == 'm') { keys.m = 1; }
    if (key == ',') { keys.sl = 1; }
    if (key == '.') { keys.sr = 1; }
}

void keyUp(unsigned char key, int x, int y)
{
    if (key == 'w') { keys.w = 0; }
    if (key == 's') { keys.s = 0; }
    if (key == 'd') { keys.d = 0; }
    if (key == 'a') { keys.a = 0; }
    if (key == 'm') { keys.m = 0; }
    if (key == ',') { keys.sl = 0; }
    if (key == '.') { keys.sr = 0; }
    if (key == 'l') { loadWalls(); }
}

// pixel function that I learned from the 3DSage channel on youtube
void pixel(int x, int y, int c)
{
    int rgb[3];

    if (c == 0) { rgb[0] = 255; rgb[1] = 255; rgb[2] = 0; }         //yellow
    else if (c == 1) { rgb[0] = 160; rgb[1] = 160; rgb[2] = 0; }    //draker yellow
    else if (c == 2) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 0; }      //green
    else if (c == 3) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 0; }      //darker green
    else if (c == 4) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 255; }    //cyan
    else if (c == 5) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 160; }    //darker cyan
    else if (c == 6) { rgb[0] = 160; rgb[1] = 100; rgb[2] = 0; }    //brown
    else if (c == 7) { rgb[0] = 110; rgb[1] = 50; rgb[2] = 0; }     //darker brown
    else if (c == 8) { rgb[0] = 0; rgb[1] = 60; rgb[2] = 130; }        //background color for now
    else { rgb[0] = 0; rgb[1] = 60; rgb[2] = 130; }                    //background color for now

    glColor3ub(rgb[0], rgb[1], rgb[2]);
    glBegin(GL_POINTS);
    glVertex2i(x * PIXEL_SCALE + 2, y * PIXEL_SCALE + 2);
    glEnd();
}

void clearBackground()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            pixel(x, y, 8);
        }
    }
}

void drawWall(int x1, int x2, int b1, int b2, int t1, int t2, int color)
{
    int dyb = b2 - b1;
    int dyt = t2 - t1;
    int dx = x2 - x1;

    if (dx == 0)
        dx = 1;                 // prevent division by zero

    int xs = x1;

    //clipping the actual pixels to the screen
    if (x1 < 1) { x1 = 1; }
    if (x2 < 1) { x2 = 1; }
    if (x1 > WIDTH - 1) { x1 = WIDTH - 1; }
    if (x2 > WIDTH - 1) { x2 = WIDTH - 1; }

    for (int x = x1; x < x2; x++)
    {
        int y1 = dyb * (x - xs + 0.5) / dx + b1;
        int y2 = dyt * (x - xs + 0.5) / dx + t1;

        if (y1 < 1) { y1 = 1; }
        if (y2 < 1) { y2 = 1; }
        if (y1 > HEIGHT - 1) { y1 = HEIGHT - 1; }
        if (y2 > HEIGHT - 1) { y2 = HEIGHT - 1; }

        for (int y = y1; y < y2; y++)
            pixel(x, y, color);
        //pixel(x, y2, 0);
    }
}

void clipBehindPlayer(int* x1, int* y1, int* z1, int x2, int y2, int z2)
{
    // example:
    // *x1 = 40
    // *y1 = -10
    // *z1 = 0
    // x2 = 50
    // y2 = 200
    // z2 = 0
    float da = *y1;     //-10, -200
    float db = y2;      //200, 10
    float d = da - db;  //-210, -210 

    if (d == 0) { d = 1; }      //I found this interesting from the code on the youtube video, I think that it was meant to work on the next step
    float s = da / (da - db);   //0.04761, 0.95238

    *x1 = *x1 + s * (x2 - (*x1));   //40 + 0.04761 * (50 - 40) = 40 (integer match, anything less than zero), 49

    //so when doing some research on this what it is doing is the scale (s) is a percentage of the total number between the values in y
    //so when we come to this line if y1 is behind the camera (zero or negative value) the percentage calculation between y2 - (*y1) mutliplied
    //by the scale will be the opposite value of *y1, so in the example below if *y1 is -10 the scale * y2 - (*y1) will always be 10 which would
    //result in a value of zero.  I need to figure out what the scale does for the other two, I believe that it is just saying that if a certain
    //percent is behind the screen then you can scale back the pixel count on the other two axis as well
    *y1 = *y1 + s * (y2 - (*y1));   //-10 + 0.04761 * (200 + 10) = -10 + 10 = 0

    if (*y1 == 0) { *y1 = 1; }      //avoid divide by zero from the calling function
    *z1 = *z1 + s * (z2 - (*z1));  //0, 0
}

void draw3D2()
{
    //world coordinates of a four point wall
    int wx[4], wy[4], wz[4];
    float sin = math.sin[player.r];
    float cos = math.cos[player.r];
    float wallSum;

    //subtracting the player position, rotating the view, and dividing by the axis perpendicular to the player
    int x[4], y[4], z[4];
    sectors.sort(compareSectorDist);

    for (auto & curSector : sectors)
    {
        //real strange issue here, had to sort the walls or we would get this distored look with the further walls drawing on top of the
        //forefront walls.  This only started happening after adding in the ability to draw the backs of the walls
        curSector.walls.sort(compareWallDist);
        wallSum = 0.0f;
        for (auto& curWall : curSector.walls)
        {
            for (int j = 0; j < 2; j++)     //looping through to draw the back side of the walls
            {

                for (int i = 0; i < 4; i++)
                {
                    x[i] = curWall.verts[i].x - player.x;
                    y[i] = curWall.verts[i].y - player.y;
                    z[i] = curWall.verts[i].z - player.z;
                }

                if (j == 0)      //swapping the x's to draw the back side of the walls
                {
                    int temp = x[1];
                    x[1] = x[0];
                    x[0] = temp;
                    temp = y[1];
                    y[1] = y[0];
                    y[0] = temp;
                    temp = x[3];
                    x[3] = x[2];
                    x[2] = temp;
                    temp = y[3];
                    y[3] = y[2];
                    y[2] = temp;

                }

                for (int i = 0; i < 4; i++)
                {
                    /*
                    *
                    * [ x1 ]  [ cos(theta) -sin(theta) ]
                    * [ y1 ]  [ sin(theta) cos(theta)  ]
                    *
                    * what these calculations are doing is matrix rotation around the player position based on the above matrix
                    */
                    wx[i] = x[i] * cos - y[i] * sin;
                    wy[i] = x[i] * sin + y[i] * cos;
                    wz[i] = z[i] + ((player.l * y[i]) / 32.0);
                }

                //I thought the parameters of the x and the y coordinates were in different positions but it is actually
                //x1, y1, x2, y2
                curWall.distance = distance(0, 0, (wx[1] + wx[0]) / 2, (wy[1] + wy[0]) / 2);
                wallSum += curWall.distance;

                //at this point we would probably continue if we had multiple walls to process
                if (wy[0] < 1 && wy[1] < 1 && wy[2] < 1 && wy[3] < 1) { continue; }

                if (wy[0] < 1)
                {
                    clipBehindPlayer(&wx[0], &wy[0], &wz[0], wx[1], wy[1], wz[1]);  //bottom line
                    clipBehindPlayer(&wx[2], &wy[2], &wz[2], wx[3], wy[3], wz[3]);  //top line
                }

                if (wy[1] < 1)
                {
                    clipBehindPlayer(&wx[1], &wy[1], &wz[1], wx[0], wy[0], wz[0]);  //bottom line
                    clipBehindPlayer(&wx[3], &wy[3], &wz[3], wx[2], wy[2], wz[2]);  //top line
                }

                // I believe that this is just using the standard calculation of projecting a 3d coordinate to the 2d screen
                // d/z, since we are dividing by y coordinate is the depth in this code sample the denominator is y
                // the d is the distance from the "camera" (as the youtube author states the field of view) to the screen thus
                // we have 200/y mulitplying each coordinate
                // https://math.stackexchange.com/questions/2305792/3d-projection-on-a-2d-plane-weak-maths-ressources
                wx[0] = wx[0] * 200 / wy[0] + SW2;
                wx[1] = wx[1] * 200 / wy[1] + SW2;
                wx[2] = wx[2] * 200 / wy[2] + SW2;
                wx[3] = wx[3] * 200 / wy[3] + SW2;
                wy[0] = wz[0] * 200 / wy[0] + SH2;
                wy[1] = wz[1] * 200 / wy[1] + SH2;
                wy[2] = wz[2] * 200 / wy[2] + SH2;
                wy[3] = wz[3] * 200 / wy[3] + SH2;

                drawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], curWall.color);
            }
        }
        curSector.distance = wallSum / curSector.numWalls;
    }
}

void draw3D()
{
    //world coordinates of a four point wall
    int wx[4], wy[4], wz[4];
    float sin = math.sin[player.r];
    float cos = math.cos[player.r];

    //subtracting the player position, rotating the view, and dividing by the axis perpendicular to the player
    int x[4], y[4], z[4];
    int x1 = 40 - player.x;
    int x2 = 40 - player.x;
    int y1 = 10 - player.y;
    int y2 = 290 - player.y;

    wx[0] = x1 * cos - y1 * sin;
    wx[1] = x2 * cos - y2 * sin;
    wx[2] = wx[0];
    wx[3] = wx[1];

    wy[0] = x1 * sin + y1 * cos;
    wy[1] = x2 * sin + y2 * cos;
    wy[2] = wy[0];
    wy[3] = wy[1];                  // tried to add 50 here to the wall but I think it only deals with straight lines

    //discuss these calculations
    wz[0] = 0 - player.z + ((player.l * wy[0]) / 32.0);
    wz[1] = 0 - player.z + ((player.l * wy[1]) / 32.0);
    wz[2] = wz[0] + 40;
    wz[3] = wz[1] + 40;             // the z can handle angled walls

    if (wy[0] < 1 && wy[1] < 1) { return; }

    if (wy[0] < 1)
    {
        clipBehindPlayer(&wx[0], &wy[0], &wz[0], wx[1], wy[1], wz[1]);  //bottom line
        clipBehindPlayer(&wx[2], &wy[2], &wz[2], wx[3], wy[3], wz[3]);  //top line
    }

    if (wy[1] < 1)
    {
        clipBehindPlayer(&wx[1], &wy[1], &wz[1], wx[0], wy[0], wz[0]);  //bottom line
        clipBehindPlayer(&wx[3], &wy[3], &wz[3], wx[2], wy[2], wz[2]);  //top line
    }

    // I believe that this is just using the standard calculation of projecting a 3d coordinate to the 2d screen
    // d/z, since we are dividing by y coordinate is the depth in this code sample the denominator is y
    // the d is the distance from the "camera" (as the youtube author states the field of view) to the screen thus
    // we have 200/y mulitplying each coordinate
    // https://math.stackexchange.com/questions/2305792/3d-projection-on-a-2d-plane-weak-maths-ressources
    wx[0] = wx[0] * 200 / wy[0] + SW2;
    wx[1] = wx[1] * 200 / wy[1] + SW2;
    wx[2] = wx[2] * 200 / wy[2] + SW2;
    wx[3] = wx[3] * 200 / wy[3] + SW2;
    wy[0] = wz[0] * 200 / wy[0] + SH2;
    wy[1] = wz[1] * 200 / wy[1] + SH2;
    wy[2] = wz[2] * 200 / wy[2] + SH2;
    wy[3] = wz[3] * 200 / wy[3] + SH2;

    //if (wx[0] > 0 && wx[0] < WIDTH && wy[0] > 0 && wy[0] < HEIGHT)
    //    pixel(wx[0], wy[0], 0);
    //if (wx[1] > 0 && wx[1] < WIDTH && wy[1] > 0 && wy[1] < HEIGHT)
    //    pixel(wx[1], wy[1], 0);

    drawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], 0);
}

// Display_Objects() function    
void Display_Objects(void)
{
    int newTickCount = glutGet(GLUT_ELAPSED_TIME);;
    int timeDifference = newTickCount - lastTickCount;

    if (timeDifference >= 50)
    {
        frameCount++;
        if (newTickCount - startTime >= 1000)
        {
            ssFrameCount.str("");
            ssFrameCount << frameCount;
            frameCount = 0;
            startTime = newTickCount;
        }

        lastTickCount = newTickCount;

        std::string strFrameCount = ssFrameCount.str();
        glClear(GL_COLOR_BUFFER_BIT);
        //clearBackground();
        draw3D2();

        // this really just looks like it is drawing only a single pixel
        //for (int i = 0; i < WIDTH / 2; i++)
        //    for (int j = 0; j < HEIGHT / 2; j++)
        //        pixel(0 + i, 0 + j, 0);

        //just outputting the fps
        glRasterPos2i(0, GLSH - 25);
        const char* pstrFrameCount = strFrameCount.c_str();
        for (int i = 0; i < strFrameCount.length(); i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pstrFrameCount[i]);
        }

        //you can draw many objects here like polygons,lines,triangles etc

        movePlayer();
        glutSwapBuffers();
        glutReshapeWindow(GLSW, GLSH);
    }
    //force it to keep redrawing
    glutPostRedisplay();
}
// Reshape() function    
void Reshape(int w, int h)
{

}

// main function    
int main(int argc, char** argv)
{
    // initialize glut    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    // set window size    
    glutInitWindowSize(GLSW, GLSH);
    // set window location    
    glutInitWindowPosition(250, 50);
    // create window with window text    
    glutCreateWindow("OpenGL Demo");
    // call Init_OpenGL() function    
    Init_OpenGL();
    // call glutDisplayFunc() function & pass parameter as Display_Objects() function    
    glutDisplayFunc(Display_Objects);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    // call glutReshapeFunc() function & pass parameter as Reshape() function    
    //glutReshapeFunc(Reshape);
    //glutMainLoop() is used to redisplay the objects    
    glutMainLoop();
    return 0;
}