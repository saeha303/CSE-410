
#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
    #include <GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>

#endif

struct Point
{
    double x, y;
  
};
double r=0.5;
double rotation=0;
Point trace;
double sinemove=180;
void axes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1, 1, 1);

        // x axis

        glVertex2f(-1, 0);
        glVertex2f(1, 0);

        // y axis

        glVertex2f(0, -1);
        glVertex2f(0, 1);

       
    }
    glEnd();
}

void DrawSineWave()
{
    double x = 0;
    double theta=sinemove*3.1416/180;
    double y = sin(theta);
    glBegin(GL_POINTS);
    {
        glColor3f(1, 1, 1);
        for (x =0; x <=2*3.1416; x += 0.01)
        {   
            y = r*sin(theta);
            glVertex2f(0.15*x+0.3, y);
            theta+=0.01;
        }
    }
    glEnd();
}

void MiniCircle()
{
     double theta=0;
     double minir=0.015;
    double prevx=minir-0.3+trace.x;
    double prevy=0+trace.y;
   while (theta<=360)
   {
         double thetainrad=theta*3.1416/180;
         double x=minir*cos(thetainrad)-0.3+trace.x;
         double y=minir*sin(thetainrad)+trace.y;
            glBegin(GL_LINES);
            {
                glColor3f(1,0,0);
                glVertex2f(prevx,prevy);
                glVertex2f(x,y);
            }
            glEnd();
            prevx=x;
            prevy=y;
            theta+=5;
   }
}

void DrawCircle()
{
    double theta=0;
    double prevx=r-0.3;
    double prevy=0;
   while (theta<=360)
   {
         double thetainrad=theta*3.1416/180;
         double x=r*cos(thetainrad)-0.3;
         double y=r*sin(thetainrad);
            glBegin(GL_LINES);
            {
                glColor3f(0,0,1);
                glVertex2f(prevx,prevy);
                glVertex2f(x,y);
            }
            glEnd();
            prevx=x;
            prevy=y;
            theta+=5;
   }
   
    
}
void Drawline()
{

    
    glBegin(GL_LINES);
    {
        glColor3f(1, 1, 1);
        glVertex2f(-0.3, 0);
        glVertex2f(trace.x-0.3,trace.y);
    }
    glEnd();
    glBegin(GL_LINES);
    {
        glColor3f(1,1,1);
        glVertex2f(trace.x-0.3,trace.y);
        glVertex2f(r-0.2,trace.y);
    }
    glEnd();
}
void display()
{

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
    // axes();
    
    // glTranslatef(-0.3,0,0);
    DrawCircle();
    Drawline();
    DrawSineWave();
    MiniCircle();
    glFlush();
}

void init()
{

    glClearColor(0, 0,0 , 1);
    trace.x=r;
    trace.y=0;
   
}

void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(80,Timer,0);
    rotation+=5;
    sinemove-=5;
    if(rotation>=360)
    {
        rotation=0;
    }
    double x= r*cos(rotation*3.1416/180);
    double y= r*sin(rotation*3.1416/180);
    trace.x=x;
    trace.y=y;
   
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Magic Cube");

    init();

   
    glutDisplayFunc(display);
  
    glutTimerFunc(0,Timer,0);
    glutMainLoop();

    return 0;
}
