#include <GL/glut.h>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;
// Define circle parameters
const int numPoints = 1000;
float centerX = 0.0f;
float centerY = 0.0f;
float radius = 0.5f;
bool flag=true;
int counter = 0;
int inc = 0;
void drawCircle()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // glBegin(GL_LINES);
    // glVertex2f(centerX, centerY);
    // glVertex2f(centerX + radius, 0);

    // glEnd();
    // Iterate through 360 degrees
    for (int i = 0; i < 360; ++i)
    {
        // Convert degrees to radians
        glBegin(GL_POINTS);
        float angle = i * 3.1415926f / 180.0f;

        // Calculate coordinates of points on the circumference
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        // Draw the point
        glVertex2f(x, y);
        glEnd();
    }
    glPushMatrix();
    // glRotatef(counter, 0, 0, 1);
    glPopMatrix();
    glBegin(GL_LINES);
    // glColor3f(0,0,1);
    // cout<<centerX + radius * cos(counter*100* 3.1415926f / 180.0f)<<","<<centerY + radius * cos(counter*100* 3.1415926f / 180.0f)<<'\n';
    cout << counter << '\n';
    glVertex2f(centerX, centerY);
    glVertex2f(centerX + radius * cos(inc * 3.1415926f / 180.0f), centerY + radius * sin(inc * 3.1415926f / 180.0f));
    glVertex2f(centerX + radius * cos(inc * 3.1415926f / 180.0f), centerY + radius * sin(inc * 3.1415926f / 180.0f));
    glVertex2f(centerX + radius + 0.1, centerY + radius * sin(inc * 3.1415926f / 180.0f));
    // if(flag)
    // cout<<"matha:"<<centerX + radius + 0.1<<","<<(double)(centerY + radius * sin(inc * 3.1415926f / 180.0f))<<'\n';
    if(centerY + radius * sin(inc * 3.1415926f / 180.0f)>=0.5){
        flag=!flag;
    }
    glEnd();
    float startX = (centerX + radius + 0.1);
    // float startX = 0;
    float startY = centerY + radius * sin(inc * 3.1415926f / 180.0f);
    float step = (1 - startX) / numPoints;
    glPushMatrix();
    glBegin(GL_POINTS);
    double phi;
    // if(flag)
    phi=asin(startY/0.5);
    // else
    // phi=asin(startY/0.5);
    cout<<phi<<'\n';
    for (int i = 0; i < numPoints; i++)
    {
        float x = (startX + i * step);
        float y;
        if(flag)
        y = 0.5 * sin(10 * (x-startX)+phi); // Compute the y-coordinate using the sine function
                                                // if(x>centerX + radius + 0.1)
        else
        y = 0.5 * sin(10 * (x-startX)+phi);                        
        // if(flag)
        // cout<<x<<","<<y<<'\n';

        glVertex2f(x, y);
    }
    // flag=!flag;
    glEnd();
    
    glPopMatrix();
    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}
void idle()
{

    counter += 1;
    inc++;
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Drawing");
    init();
    glutDisplayFunc(drawCircle);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
