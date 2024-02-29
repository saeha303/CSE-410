#include <bits/stdc++.h>
#include <GL/glut.h>
#define pi (2 * acos(0.0))
using namespace std;
int planeFlag = 0;
int drawaxes;
int radius = 25;
int stacks = 6;
int sectors = 10;
int currentdir;
double t = 1;
double ballPos[] = {0.0, 0.0, 5.0};
double camerapos[] = {-100, 100, 100};
double up[] = {0, 0, 1};
double u[] = {0, 0, 0};
double l[] = {0, 0, 0};
double r[] = {0, 0, 0};
int boundary = 125;
double threshold = 0.01;
bool simulationMode = false;
int anglefwd;
float centerX = 0.0f;
float centerY = 0.0f;
void init()
{
    drawaxes = 1;
    currentdir = 45;
    anglefwd = 0;
    for (int i = 0; i < 3; i++)
    {
        l[i] = -camerapos[i];
    }

    r[0] = l[1] * up[2] - l[2] * up[1];
    r[1] = l[2] * up[0] - l[0] * up[2];
    r[2] = l[0] * up[1] - l[1] * up[0];

    u[0] = r[1] * l[2] - r[2] * l[1];
    u[1] = r[2] * l[0] - r[0] * l[2];
    u[2] = r[0] * l[1] - r[1] * l[0];

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 1, 1, 1000.0);
}
void unitVector(double *a)
{
    double denom = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    for (int i = 0; i < 3; i++)
    {
        a[i] = a[i] / denom;
    }
}
void drawArrow()
{
    // glBegin(GL_LINE);{
    glColor3f(0.0, 0.0, 1.0); // white
    glVertex3f(ballPos[0] + 20 * cos(currentdir * (pi / 180)), ballPos[1] + 20 * sin(currentdir * (pi / 180)), (double)radius);
    glVertex3f(ballPos[0], ballPos[1], (double)radius);
    // }glEnd();
}
void drawAxes()
{
    glLineWidth(3);
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glColor3f(0.0, 0.0, 1.0); // blue
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);
        glColor3f(1.0, 0.0, 0.0); // red
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);
        glColor3f(0.0, 1.0, 0.0); // green
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
        drawArrow();
    }
    glEnd();
    glPopMatrix();
}
void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }
    glEnd();
}
void drawCheckboard()
{

    for (int i = -100; i <= 100; i++)
    {
        for (int j = -100; j <= 100; j++)
        {
            glPushMatrix();
            if ((i + j) % 2)
            {
                glColor3f(0.0, 0.0, 0.0);
            }
            else
            {
                glColor3f(1.0, 1.0, 1.0);
            }
            glTranslatef(i * 25, j * 25, 0);
            drawSquare(50);
            glPopMatrix();
        }
    }
}
void drawSphere()
{
    double stackAngle = pi / stacks;
    double phi = -pi / 2;
    double sectorAngle = 2 * pi / sectors;

    glPushMatrix();
    glTranslatef(ballPos[0], ballPos[1], (double)radius);
    glRotatef(anglefwd, 1, -1, 0);
    // cout << anglefwd << '\n';
    for (int i = 0; i <= stacks; i++)
    {
        double theta = 0;
        glBegin(GL_QUAD_STRIP);
        {
            for (int j = 0; j <= sectors; j++)
            {
                if ((i - 1) * 2 >= stacks)
                    glColor3f(j % 2, (j + 1) % 2, 0);
                else
                    glColor3f((j + 1) % 2, j % 2, 0);
                glVertex3f(radius * cos(phi - stackAngle) * cos(theta), radius * cos(phi - stackAngle) * sin(theta), radius * sin(phi - stackAngle));
                glVertex3f(radius * cos(phi) * cos(theta), radius * cos(phi) * sin(theta), radius * sin(phi));
                theta += sectorAngle;
            }
        }
        glEnd();
        phi += stackAngle;
    }
    glPopMatrix();
}
void drawRectangle(double a, double b)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, b, 0);
        glVertex3f(a, -b, 0);
        glVertex3f(-a, -b, 0);
        glVertex3f(-a, b, 0);
    }
    glEnd();
}
void drawBoundary()
{
    int x[] = {boundary, 0, -boundary, 0};
    int y[] = {0, -boundary, 0, boundary};
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(x[i], y[i], 0);
        if (!(i % 2))
            glRotatef(90, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        drawRectangle(boundary, 5);
        glPopMatrix();
    }
}
void reflect(int flag) // 0=parallel to x
{
    double a[] = {0, 0};
    a[0] = cos(currentdir * pi / (double)180);
    a[1] = sin(currentdir * pi / (double)180);
    double n[] = {0, 0};
    if (flag == 1)
    {
        n[0] = -1;
    }
    else if (flag == 2)
    {
        n[0] = 1;
    }
    else if (flag == 3)
    {
        n[1] = -1;
    }
    else if (flag == 4)
    {
        n[1] = 1;
    }
    double an = n[0] * a[0] + n[1] * a[1];
    for (int i = 0; i < 2; i++)
    {
        n[i] = 2 * an * n[i];
        n[i] = a[i] - n[i];
    }
    if (flag == 0)
    {
        if (currentdir > 0)
        {
            currentdir -= 180;
        }
        else
        {
            currentdir += 180;
        }
    }
    else if (n[0] > 0 && n[1] >= 0)
    {
        currentdir = atan(abs(n[1]) / abs(n[0])) * 180 / pi;
    }
    else if (n[0] <= 0 && n[1] > 0)
    {
        currentdir = 180 - atan(abs(n[1]) / abs(n[0])) * 180 / pi;
    }
    else if (n[0] < 0 && n[1] <= 0)
    {
        currentdir = -180 + atan(abs(n[1]) / abs(n[0])) * 180 / pi;
    }
    else if (n[0] >= 0 && n[1] < 0)
    {
        currentdir = -atan(abs(n[1]) / abs(n[0])) * 180 / pi;
    }

    currentdir = (currentdir + 180) % 360;
    if (currentdir < 0)
        currentdir += 360;
    currentdir -= 180;
}
void moveBackward()
{
    double distanceTrav;
    anglefwd = (anglefwd + 50);
    distanceTrav = 2 * pi * (double)radius * (double)(50) / (double)(360);
    ballPos[0] -= cos(currentdir * pi / (double)180) * distanceTrav;
    ballPos[1] -= sin(currentdir * pi / (double)180) * distanceTrav;
}
void moveForward()
{
    double distanceTrav;
    anglefwd = (anglefwd - 50);
    distanceTrav = 2 * pi * (double)radius * (double)(-50) / (double)(360);
    ballPos[0] -= cos(currentdir * pi / (double)180) * distanceTrav;
    ballPos[1] -= sin(currentdir * pi / (double)180) * distanceTrav;
}
void chechCollision()
{
    int flag = -1;
    bool hit = false;
    // cout<<"before hit: "<<ballPos[0]<<","<<ballPos[1]<<'\n';
    if (abs(ballPos[0]) >= boundary - 5 - threshold)
    {
        if (ballPos[0] > 0)
        {
            if (ballPos[0] == ballPos[1])
            {
                ballPos[1] = boundary - 5 - threshold;
                flag = 0;
            }
            ballPos[0] = boundary - 5 - threshold;
            if (flag)
                flag = 1;
        }
        else
        {
            if (ballPos[0] == ballPos[1])
            {
                ballPos[1] = -boundary + 5 + threshold;
                flag = 0;
            }
            ballPos[0] = -boundary + 5 + threshold;
            if (flag)
                flag = 2;
        }
        hit = true;
    }
    else if (abs(ballPos[1]) >= boundary - 5 - threshold)
    {

        if (ballPos[1] > 0)
        {
            if (ballPos[0] == ballPos[1])
            {
                ballPos[0] = -boundary + 5 + threshold;
                flag = 0;
            }
            ballPos[1] = boundary - 5 - threshold;
            if (flag)
                flag = 3;
        }
        else
        {
            if (ballPos[0] == ballPos[1])
            {
                ballPos[0] = boundary - 5 - threshold;
                flag = 0;
            }
            ballPos[1] = -boundary + 5 + threshold;
            if (flag)
                flag = 4;
        }
        hit = true;
    }
    if (hit)
    {
        // cout<<"after hit: "<<ballPos[0]<<","<<ballPos[1]<<'\n';
        reflect(flag);
    }
}
void keyboardListener(unsigned char key, int x, int y)
{
    double distanceTrav;
    double theta = 0.1;
    int i;
    switch (key)
    {
    case ' ':
        simulationMode = !simulationMode;
        break;
    case 'i':
        moveForward();
        chechCollision();
        break;
    case 'k':
        moveBackward();
        chechCollision();
        break;
    case 'j':
        currentdir++;
        currentdir = (currentdir + 180) % 360;
        if (currentdir < 0)
            currentdir += 360;
        currentdir -= 180;
        //        cout<<currentdir<<'\n';
        break;
    case 'l':
        currentdir--;
        currentdir = (currentdir + 180) % 360;
        if (currentdir < 0)
            currentdir += 360;
        currentdir -= 180;
        // cout << "l: " << currentdir << '\n';
        break;
    case '1':
        for (i = 0; i < 3; i++)
        {
            r[i] = r[i] * cos(theta) + l[i] * sin(theta);
            l[i] = l[i] * cos(theta) - r[i] * sin(theta);
        }
        unitVector(r);
        unitVector(l);
        break;

    case '2':
        for (i = 0; i < 3; i++)
        {
            r[i] = r[i] * cos(-theta) + l[i] * sin(-theta);
            l[i] = l[i] * cos(-theta) - r[i] * sin(-theta);
        }
        unitVector(r);
        unitVector(l);
        break;

    case '3':
        for (i = 0; i < 3; i++)
        {
            l[i] = l[i] * cos(theta) + u[i] * sin(theta);
            u[i] = u[i] * cos(theta) - l[i] * sin(theta);
        }
        unitVector(u);
        unitVector(l);
        break;

    case '4':
        for (i = 0; i < 3; i++)
        {
            l[i] = l[i] * cos(-theta) + u[i] * sin(-theta);
            u[i] = u[i] * cos(-theta) - l[i] * sin(-theta);
        }
        unitVector(u);
        unitVector(l);
        break;

    case '5':
        for (i = 0; i < 3; i++)
        {
            u[i] = u[i] * cos(theta) + r[i] * sin(theta);
            r[i] = r[i] * cos(theta) - u[i] * sin(theta);
        }
        unitVector(r);
        unitVector(u);
        break;

    case '6':
        for (i = 0; i < 3; i++)
        {
            u[i] = u[i] * cos(-theta) + r[i] * sin(-theta);
            r[i] = r[i] * cos(-theta) - u[i] * sin(-theta);
        }
        unitVector(r);
        unitVector(u);
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
    glutPostRedisplay();
}
void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // down arrow key
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] += t * l[i];
        break;
    case GLUT_KEY_DOWN: // up arrow key
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] -= t * l[i];
        break;

    case GLUT_KEY_RIGHT:
        // t = 1;

        for (int i = 0; i < 3; i++)
        {
            // unitVector(r);
            camerapos[i] += t * r[i];
        }
        break;
    case GLUT_KEY_LEFT:
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] -= t * r[i];

        break;

    case GLUT_KEY_PAGE_UP:
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] += 0.01 * u[i];
        break;
    case GLUT_KEY_PAGE_DOWN:
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] -= 0.01 * u[i];
        break;

    default:
        break;
    }
    glutPostRedisplay();
}
void drawCircle()
{
    // glClear(GL_COLOR_BUFFER_BIT);

    // glBegin(GL_LINES);
    // glVertex2f(centerX, centerY);
    // glVertex2f(centerX + radius, 0);

    // glEnd();
    // Iterate through 360 degrees
    glColor3f(1,1,1);
    for (int i = 0; i < 360; ++i)
    {
        // Convert degrees to radians
        glBegin(GL_POINTS);
        float angle = i * 3.1415926f / 180.0f;

        // Calculate coordinates of points on the circumference
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        // Draw the point
        glVertex3f(x, y,0);
        glEnd();
    }

    double circ=2*pi*radius;
    double portion=circ/6;

    for(int i=0; i<6; i++)
    {
        glPushMatrix();
        double theta=circ/radius;
        float x=radius*cos(theta);
        float y=radius*sin(theta);
        glRotatef(20,0,1,0);
        glTranslatef(x,y,0);
        glBegin(GL_LINES);
        {


//        glVertex3f(50,0,0);
//        glVertex3f(48,-25,0);
//        glVertex3f(50,0,0);
//        glVertex3f(52,-25,0);
            glVertex3f(0,0,5);
            glVertex3f(-5,0,0);
            glVertex3f(0,0,5);
            glVertex3f(5,0,0);
//        glTranslatef(0,0,-25);
            
        }
        glEnd();
        glColor3f(1.0, 1.0, 1.0);
        drawSquare(5);
        glPopMatrix();
    }


}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camerapos[0], camerapos[1], camerapos[2], camerapos[0] + l[0], camerapos[1] + l[1], camerapos[2] + l[2], u[0], u[1], u[2]);

    // drawCheckboard();
    drawAxes();
    // drawBoundary();
    // drawSphere();
    drawCircle();
    glutSwapBuffers();
}

int minTime()
{
    double v = 10;
    double rb, ub, lb, db;
    int t = INT_MAX;
    rb = boundary - ballPos[0];
    ub = boundary - ballPos[1];
    lb = boundary + ballPos[0];
    db = boundary + ballPos[1];
    t = min(t, (int)(rb / v));
    t = min(t, (int)(ub / v));
    t = min(t, (int)(lb / v));
    t = min(t, (int)(db / v));
    return t;
}
void timer(int millisec)
{
    if (simulationMode)
    {
        moveForward();
        chechCollision();
        int dt = minTime();
        glutPostRedisplay();
        glutTimerFunc(dt, timer, 0);
    }
    else
    {
        glutPostRedisplay();
        glutTimerFunc(5, timer, 0);
    }
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(750, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Rolling Ball");
    init();
    glutDisplayFunc(display);
    // glutTimerFunc(7, timer, 33);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}
