#include <bits/stdc++.h>
#include <GL/glut.h>
#include "1905033_classes.h"
// using namespace std;
double t = 1;
Point eye(100, 100, 100, 1);
Point up(0, 0, 1, 1);
Point u(0, 0, 0, 1);
Point l(0, 0, 0, 1);
Point r(0, 0, 0, 1);
double windowWidth = 750;
double windowHeight = 750;
double viewAngle = 80;
int pixelCount;
int recursionLevel;
int imgCount = 0;
int poinLightCount;
int spotLightCount;
vector<Object *> objects;
vector<PointLight *> pointLights;
vector<SpotLight *> spotLights;
void loadData()
{
    ifstream fin("scene.txt");
    if (!fin.is_open())
    {
        cout << "Error: Unable to open file." << endl;
        return;
    }
    fin >> recursionLevel >> pixelCount;
    int objectCount;
    fin >> objectCount;
    //the floor
    Floor *o=new Floor(1000, 20);
    double *temp = new double[3];
    for (int i = 0; i < 3; i++)
        temp[i] = 0.5;
    o->setColor(temp);
    delete temp;
    temp = new double[4];
//    temp[0]=0.25;
    for (int i = 0; i < 4; i++)
        temp[i] = 0.25;
    o->setCoefficients(temp);
    delete temp;
    o->setShine(10);
    objects.push_back(o);

    for (int i = 0; i < objectCount; i++)
    {
        string str;
        fin >> str;
        // cout<<str<<endl;
        Object *o;
        if (str == "sphere")
        {
            o = new Sphere();
            fin >> *((Sphere *)o);
            objects.push_back(o);
            // cout<<*((Sphere *)o)<<endl;
        }
        else if (str == "triangle")
        {
            o = new Triangle();
            fin >> *((Triangle *)o);
            // cout<< *((Triangle*)o)<<endl;
            objects.push_back(o);
        }
        else if (str == "general")
        {
            o = new General();
            fin >> *((General *)o);
            // cout<<*((General *)o)<<endl;
            objects.push_back(o);

        }
        else
        {
            cout << str << " is not an object type" << endl;
        }
    }
    fin >> poinLightCount;
    for (int i = 0; i < poinLightCount; i++)
    {
        PointLight *pl = new PointLight();
        fin >> *pl;
        pointLights.push_back(pl);
        // cout<<*pl<<endl;
    }
    fin >> spotLightCount;
    for (int i = 0; i < spotLightCount; i++)
    {
        SpotLight *sl = new SpotLight();
        fin >> *sl;
        spotLights.push_back(sl);
        // cout<<*sl<<endl;
    }
    fin.close();
    cout << "end of load data" << endl;
}
void init()
{
    Point zero;
    l = zero - eye;
    l.normalize();
    r = l & up;
    r.normalize();
    u = r & l;

    // glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, 1, 1, 1000.0);
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
    }
    glEnd();
    glPopMatrix();
}
void capture()
{
    bitmap_image img(pixelCount, pixelCount);
    for (int i = 0; i < pixelCount; i++)
    {
        for (int j = 0; j < pixelCount; j++)
        {
            img.set_pixel(i, j, 0, 0, 0);
        }
    }
    double planeDistance = (windowHeight / 2.0) / tan(deg_to_rad(viewAngle/2.0));

    Point topLeft = eye + l * planeDistance - r * (windowWidth / 2.0) + u * (windowHeight / 2.0);

    double du = windowWidth / (double)pixelCount;
    double dv = windowHeight / (double)pixelCount;

    // Choose middle of the grid cell
    topLeft = topLeft + r * (0.5 * du) - u * (0.5 * dv);

    double t, tMin;
    // ofstream fout("out33.txt");
    for (int i = 0; i < pixelCount; i++)
    {
        for (int j = 0; j < pixelCount; j++)
        {
            Point curPixel = topLeft + r * du * i - u * dv * j;//direction is r, we will move by du amount every time, same for u
            Ray ray(eye, curPixel - eye);
            double *color = new double[3];
            for(int i=0; i<3; i++)
            {
                color[i]=0.0;
            }
            tMin = 1e9;
            Object *nearest=NULL;

            for (Object *o: objects)
            {
                // fout<<ray<<endl;
                t = o->intersect(ray, color, 0);
                //for updating t_min for the very first time, we need nearest==NULL
                //for later objects, you can use t_min
                if (t > 0 && t < tMin)
                    tMin = t, nearest = o;
            }
            // if nearest object is found, then shade the pixel
            if (nearest != NULL)
            {
                for(int i=0; i<3; i++)
                {
                    color[i]=0.0;
                }
                tMin = nearest->intersect(ray, color, 1);
                //color 1 er cheye boro hoye jay, tarpor 255 diye gun, shattik bolse
                if (color[0] > 1) color[0] = 1;
                if (color[1] > 1) color[1] = 1;
                if (color[2] > 1) color[2] = 1;
                if (color[0] < 0) color[0] = 0;
                if (color[1] < 0) color[1] = 0;
                if (color[2] < 0) color[2] = 0;

                // cout<<"After Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
                img.set_pixel(i, j, 255 * color[0], 255 * color[1], 255 * color[2]);
            }
            delete color;
        }
    }
    // fout.close();
    img.save_image("capture" + to_string(imgCount) + ".bmp");
    img.clear();
    imgCount++;
    cout<<"end of capture"<<endl;
}

void keyboardListener(unsigned char key, int x, int y)
{
    double theta = 0.01;
    int i;
    switch (key)
    {
    case '0':
        capture();
        break;
    case '1':
        r = r * cos(theta) + l * sin(theta);
        l = l * cos(theta) - r * sin(theta);
        r.normalize();
        l.normalize();
        break;

    case '2':
        r = r * cos(-theta) + l * sin(-theta);
        l = l * cos(-theta) - r * sin(-theta);
        r.normalize();
        l.normalize();
        break;

    case '3':
        l = l * cos(theta) + u * sin(theta);
        u = u * cos(theta) - l * sin(theta);
        l.normalize();
        u.normalize();
        break;

    case '4':
        l = l * cos(-theta) + u * sin(-theta);
        u = u * cos(-theta) - l * sin(-theta);
        l.normalize();
        u.normalize();
        break;

    case '5':
        u = u * cos(-theta) + r * sin(-theta);
        r = r * cos(-theta) - u * sin(-theta);
        r.normalize();
        u.normalize();
        break;

    case '6':
        u = u * cos(theta) + r * sin(theta);
        r = r * cos(theta) - u * sin(theta);
        r.normalize();
        u.normalize();
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
}
void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // down arrow key
        eye = eye + l * t;
        break;
    case GLUT_KEY_DOWN: // up arrow key
        eye = eye - l * t;
        break;

    case GLUT_KEY_RIGHT:
        eye = eye + r * t;
        break;
    case GLUT_KEY_LEFT:
        eye = eye - r * t;
        break;

    case GLUT_KEY_PAGE_UP:
        eye = eye + u * t;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eye = eye - u * t;
        break;

    default:
        break;
    }
}
void idle()
{
    glutPostRedisplay();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, eye.x + l.x, eye.y + l.y, eye.z + l.z, u.x, u.y, u.z);
    glMatrixMode(GL_MODELVIEW);
    drawAxes();
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
//    glDisable(GL_DEPTH_TEST);
    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i]->draw();
    }
    for (int i = 0; i < spotLights.size(); i++)
    {
        spotLights[i]->draw();
    }
    glutSwapBuffers();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("1905033");
    init();
    loadData();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();

    objects.clear();
    pointLights.clear();
    spotLights.clear();
    return 0;
}
