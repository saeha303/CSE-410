#include <GL/glut.h>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;
// Define circle parameters
const int numPoints = 100000;
float centerX[3] = {0.0, 0.0, 0.0};
float centerY[3] = {0.0, 0.0, 0.0};
float radius[3] = {0.5, 0.3, 0.1};

int counter = 0;
int inc[3] = {0, 30, 60};
void drawCircle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,0,1);
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
        float x = centerX[0] + radius[0] * cos(angle);
        float y = centerY[0] + radius[0] * sin(angle);

        // Draw the point
        glVertex2f(x, y);
        glEnd();
    }
    glBegin(GL_LINES);
    // glColor3f(0,0,1);
    // cout<<centerX + radius * cos(counter*100* 3.1415926f / 180.0f)<<","<<centerY + radius * cos(counter*100* 3.1415926f / 180.0f)<<'\n';
    cout << counter << '\n';
    glVertex2f(centerX[0], centerY[0]);
    glVertex2f(centerX[0] + radius[0] * cos(inc[0] * 3.1415926f / 180.0f), centerY[0] + radius[0] * sin(inc[0] * 3.1415926f / 180.0f));
    glEnd();
    glColor3f(0,1,0);
    centerX[1]=centerX[0]+radius[0] * cos(inc[0] * 3.1415926f / 180.0f);
    centerY[1]=centerY[0]+radius[0] * sin(inc[0] * 3.1415926f / 180.0f);
    for (int i = 0; i < 360; ++i)
    {
        // Convert degrees to radians
        glBegin(GL_POINTS);
        float angle = i * 3.1415926f / 180.0f;

        // Calculate coordinates of points on the circumference
        float x = centerX[1] + radius[1] * cos(angle);
        float y = centerY[1] + radius[1] * sin(angle);

        // Draw the point
        glVertex2f(x, y);
        glEnd();
    }
    glBegin(GL_LINES);
    // glColor3f(0,0,1);
    // cout<<centerX + radius * cos(counter*100* 3.1415926f / 180.0f)<<","<<centerY + radius * cos(counter*100* 3.1415926f / 180.0f)<<'\n';
    glVertex2f(centerX[1], centerY[1]);
    glVertex2f(centerX[1] + radius[1] * cos(inc[1] * 3.1415926f / 180.0f), centerY[1] + radius[1] * sin(inc[1] * 3.1415926f / 180.0f));
    glEnd();
    glColor3f(1,0,0);
    centerX[2]=centerX[1]+radius[1] * cos(inc[1] * 3.1415926f / 180.0f);
    centerY[2]=centerY[1]+radius[1] * sin(inc[1] * 3.1415926f / 180.0f);
    for (int i = 0; i < 360; ++i)
    {
        // Convert degrees to radians
        glBegin(GL_POINTS);
        float angle = i * 3.1415926f / 180.0f;

        // Calculate coordinates of points on the circumference
        float x = centerX[2] + radius[2] * cos(angle);
        float y = centerY[2] + radius[2] * sin(angle);

        // Draw the point
        glVertex2f(x, y);
        glEnd();
    }
    
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
    // for(int i=0;i<3;i++)
    // inc[i]++;
    inc[0]++;
    inc[1]+=5;
    inc[2]+=10;
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Circle Drawing");
    init();
    glutDisplayFunc(drawCircle);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
