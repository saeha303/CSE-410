#include <bits/stdc++.h>
#include <GL/glut.h>
#define pi (2 * acos(0.0))
#define dihedral_angle 70.5287794
using namespace std;

double maxLen,triLen,maxRad,radius,increment,t;
int current_dir;
double camerapos[]={6,0,4};
double l[]={-1, 0, 0};
double r[]={0, 1, 0};
double u[]={0, 0, 1};
bool f=true;

void init()
{
    maxLen = 2.5;
    triLen = 2.5;
    maxRad = maxLen / sqrt(3.0);
    radius = 0;
    increment = maxRad / 25.0;
    current_dir = 45;
    t=1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);//:)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 1, 1, 1000.0);
}

void drawAxes()
{
    glLineWidth(3);
    glPushMatrix();
    glBegin(GL_LINES);
    {
        glColor3f(0.0, 0.0, 1.0); // blue
        glVertex3f(5, 0, 0);
        glVertex3f(-5, 0, 0);
        glColor3f(1.0, 0.0, 0.0); // red
        glVertex3f(0, -5, 0);
        glVertex3f(0, 5, 0);
        glColor3f(0.0, 1.0, 0.0); // green
        glVertex3f(0, 0, 5);
        glVertex3f(0, 0, -5);
    }
    glEnd();
    glPopMatrix();
}

void drawTriangle(double triLen)
{
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(triLen, 0, 0);
        glVertex3f(0, triLen, 0);
        glVertex3f(0, 0, triLen);
    }
    glEnd();
    glPopMatrix();
}

void drawCylinderPart(double height, double radius, int segments)
{
    double center_angle = dihedral_angle * pi / 180.0;
    double constantAngle=center_angle / segments;
    glPushMatrix();
    glBegin(GL_QUADS);
    for (int i = 0; i < segments; i++)
    {
        double theta = i * constantAngle;
        glVertex3f(radius * cos(theta), radius * sin(theta), height / 2);
        glVertex3f(radius * cos(theta), radius * sin(theta), -height / 2);
        glVertex3f(radius * cos(theta+constantAngle), radius * sin(theta+constantAngle), -height / 2);
        glVertex3f(radius * cos(theta+constantAngle), radius * sin(theta+constantAngle), height / 2);
    }
    glEnd();
    glPopMatrix();
}

void drawCylinders()
{
    double center = dihedral_angle * pi / 180.0;
    glColor3f(1.0f, 1.0f, 0.0f);
    // XZ plane
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triLen / sqrt(2), 0, 0);
            glRotatef(-dihedral_angle / 2, 0, 0, 1);
            drawCylinderPart(triLen * sqrt(2), radius, 100);
        }
        glPopMatrix();
    }
    // XY pane
    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            glRotatef(90, 1, 0, 0);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triLen / sqrt(2), 0, 0);
            glRotatef(-dihedral_angle / 2, 0, 0, 1);
            drawCylinderPart(triLen * sqrt(2), radius, 100);
        }
        glPopMatrix();
    }
    // YZ plane
    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            glRotatef(90, 0, 0, 1);
            glRotatef(45 + i * 90, 0, 1, 0);
            glTranslatef(triLen / sqrt(2), 0, 0);
            glRotatef(-dihedral_angle / 2, 0, 0, 1);
            drawCylinderPart(triLen * sqrt(2), radius, 100);
        }
        glPopMatrix();
    }
}
void unitVector(double *a)
{
	double denom = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	for (int i = 0; i < 3; i++)
	{
		a[i] = a[i] / denom;
	}
}
void drawSphereSegments(double radius, int subdivision)
{
    int segments = (int)pow(2, subdivision) + 1;
    double portion = (double)90 / (segments - 1);
    double a[]={0,0,0};
    glPushMatrix();
    for (int i = 0; i < segments - 1; i++)
    {
        double theta = 45 - i * portion;
        for (int j = 0; j < segments - 1; j++)
        {
            double phi = 45 - j * portion;

            glBegin(GL_QUADS);
            {
                a[0]=cos(theta * (double)pi / 180) * cos(phi * (double)pi / 180);
                a[1]=sin(theta * (double)pi / 180) * cos(phi * (double)pi / 180);
                a[2]=-sin(phi * (double)pi / 180) * cos(theta * (double)pi / 180);
                unitVector(a);
                glVertex3f(a[0]*radius,a[1]*radius,a[2]*radius);
                a[0]=cos(theta * (double)pi / 180) * cos((phi-portion) * (double)pi / 180);
                a[1]=sin(theta * (double)pi / 180) * cos((phi-portion) * (double)pi / 180);
                a[2]=-sin((phi-portion) * (double)pi / 180) * cos(theta * (double)pi / 180);
                unitVector(a);
                glVertex3f(a[0]*radius,a[1]*radius,a[2]*radius);
                a[0]=cos((theta-portion) * (double)pi / 180) * cos((phi-portion) * (double)pi / 180);
                a[1]=sin((theta-portion) * (double)pi / 180) * cos((phi-portion) * (double)pi / 180);
                a[2]=-sin((phi-portion) * (double)pi / 180) * cos((theta-portion) * (double)pi / 180);
                unitVector(a);
                glVertex3f(a[0]*radius,a[1]*radius,a[2]*radius);
                a[0]=cos((theta-portion) * (double)pi / 180) * cos(phi * (double)pi / 180);
                a[1]=sin((theta-portion) * (double)pi / 180) * cos(phi * (double)pi / 180);
                a[2]=-sin(phi * (double)pi / 180) * cos((theta-portion) * (double)pi / 180);
                unitVector(a);
                glVertex3f(a[0]*radius,a[1]*radius,a[2]*radius);
            }
            glEnd();
        }
    }
    glPopMatrix();
}
// transforming a cube, so at first 4 planes, then 2 planes
void drawSpheres()
{
    //4 sides perpendicular to xy plane
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f(0, i % 2, (i + 1) % 2); // blue / green
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(triLen, 0, 0);
            drawSphereSegments(radius, 6);
        }
        glPopMatrix();
    }
    //perpendicular to z plane
    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
        {
            glColor3f(1.0f, 0.0f, 0.0f); // red
            glRotatef(180 * i, 1, 0, 0);
            glRotatef(90, 0, 0, 1);
            glTranslatef(triLen, 0, 0);
            drawSphereSegments(radius, 6);
        }
        glPopMatrix();
    }
}

void drawOctahedron()
{
    double diff = maxLen - triLen;
    diff = diff / 3.0;
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glColor3f((i + 1) % 2, i % 2, 1.0f);
            glRotatef(90 * i, 0, 0, 1);
            glTranslatef(diff, diff, diff);
            drawTriangle(triLen);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {

        glPushMatrix();
        {
            glColor3f(i % 2, (i + 1) % 2, 1.0f);
            glRotatef(90 * i, 0, 0, 1);
            glRotatef(180, 1, 1, 0);
            glTranslatef(diff, diff, diff);
            drawTriangle(triLen);
        }
        glPopMatrix();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camerapos[0], camerapos[1], camerapos[2], camerapos[0] + l[0], camerapos[1] + l[1], camerapos[2] + l[2], u[0], u[1], u[2]);
    glRotatef(current_dir, 0, 0, 1);

    drawAxes();
    drawOctahedron();
    drawSpheres();
    drawCylinders();
    glutSwapBuffers();
}
void keyboardListener(unsigned char key, int x, int y)
{
    double theta = 0.1;
    int i;
    switch (key)
    {
    case 'a':
        current_dir -= 5;
        current_dir=current_dir%360;
        break;
    case 'd':
        current_dir += 5;
        current_dir=current_dir%360;
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
    case ',':
        triLen -= 0.1;
        radius += increment;
        if (triLen < 0)
        {
            triLen = 0;
            radius = maxRad;
        }
        break;
    case '.':
        triLen += 0.1;
        radius -= increment;
        if (triLen > maxLen)
        {
            triLen = maxLen;
            radius = 0;
        }
        break;
    default:
        printf("We don't know what you pressed\n");
        break;
    }
    glutPostRedisplay();// why don't u work?
}

void specialKeyListener(int key, int x, int y)
{
    // double t;
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
            camerapos[i] += t * u[i];
        break;
    case GLUT_KEY_PAGE_DOWN:
        // t = 1;
        for (int i = 0; i < 3; i++)
            camerapos[i] -= t * u[i];
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Magic Cube");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}
