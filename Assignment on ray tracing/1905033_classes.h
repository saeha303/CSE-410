#include <bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
#define pi 2.0 * acos(0.0)
#define deg_to_rad(angle) angle *pi / 180
#define epsilon 1e-6
using namespace std;
extern int recursionLevel;
class Object;
class PointLight;
class SpotLight;
extern vector<Object *> objects;
extern vector<PointLight *> pointLights;
extern vector<SpotLight *> spotLights;
class Point
{
public:
    double x, y, z, w;
    bool taken = false;
    Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
    Point()
    {
        x = y = z = 0;
        w = 1;
    }
    friend istream &operator>>(istream &in, Point &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }
    void divideByW()
    {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
        // cout<<x<<","<<y<<","<<z<<","<<w<<'\n';
    }
    double len()
    {
        return sqrt(x * x + y * y + z * z);
    }
    void normalize()
    {
        double l = len();
        x /= l;
        y /= l;
        z /= l;
    }
    Point operator*(double t) // scalar multiplication
    {
        return Point(x * t, y * t, z * t, 1);
    }
    double operator*(Point t) // dot multiplication
    {
        return x * t.x + y * t.y + z * t.z;
    }
    Point operator+(Point t)
    {
        return Point(x + t.x, y + t.y, z + t.z, 1);
    }
    Point operator-(Point t)
    {
        return Point(x - t.x, y - t.y, z - t.z, 1);
    }
    Point operator&(Point t) // cross multiplication
    {
        return Point(y * t.z - z * t.y, z * t.x - x * t.z, x * t.y - y * t.x, 1);
    }
    Point operator/(double t) // cross multiplication
    {
        return Point(x / t, y / t, z / t, 1);
    }
    friend ostream &operator<<(ostream &out, Point &p)
    {
        out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return out;
    }
};
class Ray
{
public:
    Point start;
    Point dir;
    Ray()
    {
        start = Point(0,0,0,1);
        dir = Point(0,0,0,1);
    }
    Ray(Point s, Point d)
    {
        start = s;
        dir = d;
        dir.normalize();
    }
    friend ostream &operator<<(ostream &out, Ray &r)
    {
        out << r.start << " " << r.dir;
        return out;
    }
};
class PointLight
{
public:
    Point light_pos;
    double color[3];
    PointLight()
    {
        light_pos=Point(0,0,0,1);
        color[0]=color[1]=color[2]=0;
    }
    void draw()
    {
        glPushMatrix();
        glColor3d(color[0], color[1], color[2]);
        glPushMatrix();
        glTranslatef(light_pos.x, light_pos.y, light_pos.z);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();
    }
    friend istream &operator>>(istream &in, PointLight &o)
    {
        in >> o.light_pos;
        for (int i = 0; i < 3; i++)
            in >> o.color[i];
        return in;
    }
    friend ostream &operator<<(ostream &out, PointLight &o)
    {
        out << o.light_pos;
        for (int i = 0; i < 3; i++)
            out << o.color[i] << ' ';
        return out;
    }
};
class SpotLight
{
public:
    PointLight point_light;
    Point light_direction;
    double cutoff_angle;
    SpotLight()
    {
        point_light=PointLight();
        light_direction=Point(0,0,0,1);
    }
    void draw()
    {
        glColor3d(point_light.color[0], point_light.color[1], point_light.color[2]);
        glPushMatrix();
        glTranslatef(point_light.light_pos.x, point_light.light_pos.y, point_light.light_pos.z);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();
    }
    friend istream &operator>>(istream &in, SpotLight &o)
    {
        in >> o.point_light;
        in >> o.light_direction;
        in >> o.cutoff_angle;
        return in;
    }
    friend ostream &operator<<(ostream &out, SpotLight &o)
    {
        out << o.point_light << endl;
        out << o.light_direction << endl;
        out << o.cutoff_angle << endl;
        return out;
    }
};
class Object
{
public:
    Point reference_point;// should have x, y, z
    double height, width, length;
    double color[3];
    double coefficients[4];// ambient, diffuse, specular,
//reflection coefficients
    int shine;// exponent term of specular component

    Object()
    {
        reference_point=Point(0,0,0,1);
        height=width=length=0;
        color[0]=color[1]=color[2]=0;
        coefficients[0]=coefficients[1]=coefficients[2]=coefficients[3]=0;
    }
    virtual void draw() = 0;
    virtual double getIntersectionPoint_t(Ray ray, double *color, int level){
        return -1;
    }
    virtual Ray getNormal(Point point, Ray incidentRay) = 0;
    virtual double intersect(Ray r, double *color, int level)=0;

    void setColor(double *color)
    {
        for (int i = 0; i < 3; i++)
        {
            this->color[i] = color[i];
        }
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setCoefficients(double *coefficients)
    {
        for (int i = 0; i < 4; i++)
        {
            this->coefficients[i] = coefficients[i];
        }
    }
    virtual vector<double> getColor(Point point)
    {
        vector<double> color(3, 0);
        for (int i = 0; i < 3; i++)
        {
            color[i] = this->color[i];
        }
        return color;
    }
    virtual vector<double> getCoefficients()
    {
        vector<double> coeff(4, 0);
        for (int i = 0; i < 4; i++)
        {
            coeff[i] = this->coefficients[i];
        }
        return coeff;
    }
    virtual double intersectHelper(Ray ray, double *color, int level)
    {
        double t = getIntersectionPoint_t(ray, color, level);
        if (t < 0)
            return -1;
        if (level == 0)
            return t;
        Point intersectionPoint = ray.start + ray.dir * t;
        vector<double> intersectionPointColor = getColor(intersectionPoint);
        color[0] = intersectionPointColor[0] * coefficients[0];
        color[1] = intersectionPointColor[1] * coefficients[0];
        color[2] = intersectionPointColor[2] * coefficients[0];
        for (PointLight *pl : pointLights)
        {
            Point lightDirection = intersectionPoint - pl->light_pos;
            lightDirection.normalize();
            double dist_from_light = lightDirection.len();
            // consider it zero
            if (dist_from_light < epsilon)
                continue;
            Ray ray_l = Ray(pl->light_pos, lightDirection);
            bool isInShadow = false;
            // cout << "start" << endl;
            for (Object *obj : objects)
            {
                double temp2 = obj->getIntersectionPoint_t(ray_l, color, 0);
                Point tempIntersectionPoint = ray.start + ray.dir * temp2;
                temp2=(tempIntersectionPoint-pl->light_pos).len();
                if ( temp2 + epsilon < dist_from_light)
                {
                    // cout << "isInShadow" << endl;
                    isInShadow = true;
                    break;
                }
            }
            Ray normal = getNormal(intersectionPoint, ray_l); // N
            Ray ray_r = Ray(intersectionPoint, ray_l.dir - normal.dir * 2 * (ray_l.dir * normal.dir));
            if (!isInShadow)
            {
                // lambertValue
                double lambert = max(0.0, -(ray_l.dir * normal.dir));
                // find reflected ray
                double phong = pow(max(0.0, -(ray.dir * ray_r.dir)),shine);
                color[0] += pl->color[0] * coefficients[1] * lambert * intersectionPointColor[0] + pl->color[0] * coefficients[2] * phong * intersectionPointColor[0];
                color[1] += pl->color[1] * coefficients[1] * lambert * intersectionPointColor[1] + pl->color[1] * coefficients[2] * phong * intersectionPointColor[1];
                color[2] += pl->color[2] * coefficients[1] * lambert * intersectionPointColor[2] + pl->color[2] * coefficients[2] * phong * intersectionPointColor[2];
            }
        }
        for (SpotLight *sl : spotLights)
        {
            Point lightDirection = intersectionPoint - sl->point_light.light_pos;
            lightDirection.normalize();
            double dot = lightDirection * sl->light_direction;
            double angle = acos(dot / (lightDirection.len() * sl->light_direction.len())) * (180.0 / pi);
            if (fabs(angle) < sl->cutoff_angle)
            {
                Ray ray_l = Ray(sl->point_light.light_pos, lightDirection);
                double dist_from_light = lightDirection.len();
                if (dist_from_light < epsilon)
                    continue;
                bool isInShadow = false;
                for (Object *obj : objects)
                {
                    double temp2 = obj->getIntersectionPoint_t(ray_l, color, 0);
                    Point tempIntersectionPoint = ray.start + ray.dir * temp2;
                    temp2=(tempIntersectionPoint-sl->point_light.light_pos).len();
                    if ( temp2 + epsilon < dist_from_light)
                    {
                        // cout << "isInShadow" << endl;
                        isInShadow = true;
                        break;
                    }
                }
                Ray normal = getNormal(intersectionPoint, ray_l);
                Ray ray_r = Ray(intersectionPoint, ray_l.dir - normal.dir * 2 * (ray_l.dir * normal.dir));
                if (!isInShadow)
                {
                    // lambertValue
                    double lambert = max(0.0, -(ray_l.dir * normal.dir));
                    // find reflected ray
                    double phong = pow(max(0.0, -(ray.dir * ray_r.dir)),shine);
                    color[0] += sl->point_light.color[0] * coefficients[1] * lambert * intersectionPointColor[0] + sl->point_light.color[0] * coefficients[2] * phong * intersectionPointColor[0];
                    color[1] += sl->point_light.color[1] * coefficients[1] * lambert * intersectionPointColor[1] + sl->point_light.color[1] * coefficients[2] * phong * intersectionPointColor[1];
                    color[2] += sl->point_light.color[2] * coefficients[1] * lambert * intersectionPointColor[2] + sl->point_light.color[2] * coefficients[2] * phong * intersectionPointColor[2];
                }
            }
        }
        if (level >= recursionLevel)
        {
            return t;
        }
            Ray normal = getNormal(intersectionPoint, ray);
            Ray ray_r = Ray(intersectionPoint, ray.dir - normal.dir * 2 * (ray.dir * normal.dir));
            // fix the start point a bit above the circle
            ray_r.start = ray_r.start + ray_r.dir * epsilon;
            Object *obj = NULL;
            t = -1;
            double tMin = 1e9;
            for (Object *o : objects)
            {
                t = o->intersect(ray_r, color, 0);
                if (t > 0 && t < tMin)
                    tMin = t, obj = o;
            }
            if (obj != NULL)
            {
                double *colorReflected = new double[3];
                for (int i = 0; i < 3; i++)
                    colorReflected[i] = 0;
                double t = obj->intersect(ray_r, colorReflected, level + 1);
                color[0] += colorReflected[0] * coefficients[3];
                color[1] += colorReflected[1] * coefficients[3];
                color[2] += colorReflected[2] * coefficients[3];
            }
        return t;
    }
};
class Sphere : public Object
{
public:
    Sphere() {
        reference_point=Point(0,0,0,1);
        length = 5;
    }
    Sphere(Point center, double radius)
    {
        reference_point = center;
        length = radius;
    }
    virtual void draw()
    {
        int slices = 24, stacks = 20;
        Point points[100][100];
        double h, r;
        // generate points
        for (int i = 0; i <= stacks; i++)
        {
            h = length * sin(((double)i / (double)stacks) * (pi / 2));
            r = length * cos(((double)i / (double)stacks) * (pi / 2));
            for (int j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = h;
            }
        }
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        // draw quads using generated points
        for (int i = 0; i < stacks; i++)
        {
            glColor3f(color[0], color[1], color[2]);
            for (int j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }
    virtual Ray getNormal(Point point, Ray ray)
    {
        return Ray(point, point - reference_point);
    }

    virtual double getIntersectionPoint_t(Ray ray, double *color, int level)
    {
        // cout << "madness in sphere" << endl;
        Point r_o = ray.start - reference_point;
        Point r_d = ray.dir;
        double a = 1;
        double b = (r_d * r_o) * 2.0;
        double c = r_o * r_o - length * length;
        double discriminant = b * b - 4 * a * c;
        double t = -1;
        if (discriminant > 0)
        {
            double t1 = (-b + sqrt(discriminant)) / (2 * a);
            double t2 = (-b - sqrt(discriminant)) / (2 * a);
            if (t1 < 0 && t2 > 0)
            {
                t = t2;
            }
            else if (t2 < 0 && t1 > 0)
            {
                t = t1;
            }
            else
            {
                t = min(t1, t2);
            }
        }
        else if (discriminant == 0.0)
        {
            t = -b;
        }
        return t;
    }
    virtual double intersect(Ray r, double *color, int level){
        return intersectHelper(r,color,level);
    }
    friend istream &operator>>(istream &in, Sphere &o)
    {
        in >> o.reference_point >> o.length;
        for (int i = 0; i < 3; i++)
            in >> o.color[i];
        for (int i = 0; i < 4; i++)
        {
            in >> o.coefficients[i];
        }
        in >> o.shine;
        return in;
    }
    friend ostream &operator<<(ostream &out, Sphere &o)
    {
        out << o.reference_point << ' ' << o.length;
        for (int i = 0; i < 3; i++)
            out << o.color[i] << ' ';
        out << endl;
        for (int i = 0; i < 4; i++)
        {
            out << o.coefficients[i] << ' ';
        }
        out << endl;
        out << o.shine;
        return out;
    }
};
class Triangle : public Object
{
public:
    Point vertices[3];

    Triangle() {}

    Triangle(Point *arr)
    {
        for (int i = 0; i < 3; i++)
        {
            vertices[i] = arr[i];
        }
    }

    virtual Ray getNormal(Point point, Ray ray)
    {
        Point normal = (vertices[1] - vertices[0]) & (vertices[2] - vertices[0]);
        // normal.normalize();

        if (ray.dir * normal < 0)
        {
            return Ray(point, normal * -1.0);
        }
        return Ray(point, normal);
    }

    virtual void draw()
    {
        glEnable(GL_DEPTH_TEST); //:)
        // glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
            glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
            glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
        }
        glEnd();
        glPopMatrix();
    }
    double determinant(double a[3][3])
    {
        return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
    }
    virtual double getIntersectionPoint_t(Ray ray, double *color, int level)
    {
        // cout << "madness in triangle" << endl;
        Point r_o = ray.start;
        Point r_d = ray.dir;
        double beta[3][3] =
        {
            {vertices[0].x - r_o.x, vertices[0].x - vertices[2].x, r_d.x},
            {vertices[0].y - r_o.y, vertices[0].y - vertices[2].y, r_d.y},
            {vertices[0].z - r_o.z, vertices[0].z - vertices[2].z, r_d.z}
        };
        double gamma[3][3] =
        {
            {vertices[0].x - vertices[1].x, vertices[0].x - r_o.x, r_d.x},
            {vertices[0].y - vertices[1].y, vertices[0].y - r_o.y, r_d.y},
            {vertices[0].z - vertices[1].z, vertices[0].z - r_o.z, r_d.z}
        };
        double t[3][3] =
        {
            {vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, vertices[0].x - r_o.x},
            {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, vertices[0].y - r_o.y},
            {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, vertices[0].z - r_o.z}
        };
        double A[3][3]
        {
            {vertices[0].x - vertices[1].x, vertices[0].x - vertices[2].x, r_d.x},
            {vertices[0].y - vertices[1].y, vertices[0].y - vertices[2].y, r_d.y},
            {vertices[0].z - vertices[1].z, vertices[0].z - vertices[2].z, r_d.z}
        };
        double A_det = determinant(A);
        double beta_val = determinant(beta) / A_det;
        double gamma_val = determinant(gamma) / A_det;
        double t_val = determinant(t) / A_det;
        double ret = -1;
        if (beta_val + gamma_val < 1 && beta_val > 0 && gamma_val > 0 && t_val > 0)
        {
            ret = t_val;
        }
        return ret;
    }
    virtual double intersect(Ray r, double *color, int level){
        return intersectHelper(r,color,level);
    }
    friend istream &operator>>(istream &in, Triangle &o)
    {
        in >> o.vertices[0] >> o.vertices[1] >> o.vertices[2];
        for (int i = 0; i < 3; i++)
            in >> o.color[i];
        for (int i = 0; i < 4; i++)
            in >> o.coefficients[i];
        in >> o.shine;
        return in;
    }
    friend ostream &operator<<(ostream &out, Triangle &o)
    {
        out << o.vertices[0] << ' ' << o.vertices[1] << ' ' << o.vertices[2] << endl;
        for (int i = 0; i < 3; i++)
            out << o.color[i] << ' ';
        out << endl;
        for (int i = 0; i < 4; i++)
            out << o.coefficients[i] << ' ';
        out << endl;
        out << o.shine << endl;
        return out;
    }
};
class General : public Object
{
public:
    double A, B, C, D, E, F, G, H, I, J;
    General() {
        A=B=C=D=E=F=G=H=I=J=0.0;
    }
    virtual void draw()
    {
        return;
    }

    virtual Ray getNormal(Point point, Ray ray)
    {
        double x = A * point.x * 2.0 + D * point.y + E * point.z + G;
        double y = B * point.y * 2.0 + D * point.x + F * point.z + H;
        double z = C * point.z * 2.0 + E * point.x + F * point.y + I;
        Point dir(x, y, z, 1.0);
        return Ray(point, dir);
    }

    bool insideReferenceCube(Point point)
    {
        if (length > 0)
        {
            if (point.x < reference_point.x || point.x > reference_point.x + length)
                return false;
        }
        if (width > 0)
        {
            if (point.y < reference_point.y || point.y > reference_point.y + width)
                return false;
        }
        if (height > 0)
        {
            if (point.z < reference_point.z || point.z > reference_point.z + height)
                return false;
        }
        return true;
    }

    virtual double getIntersectionPoint_t(Ray ray, double *color, int level)
    {
        // cout << "madness in general" << endl;
        Point r_o = ray.start - reference_point;
        Point r_d = ray.dir;
        double a = A * r_d.x * r_d.x + B * r_d.y * r_d.y + C * r_d.z * r_d.z + D * r_d.x * r_d.y + E * r_d.y * r_d.z + F * r_d.x * r_d.z;
        double b = 2 * A * r_o.x * r_d.x + 2 * B * r_o.y * r_d.y + 2 * C * r_o.z * r_d.z + D * r_o.x * r_d.y + D * r_d.x * r_o.y + E * r_o.y * r_d.z + E * r_o.z * r_d.y + F * r_o.x * r_d.z + F * r_d.x * r_o.z + G * r_d.x + H * r_d.y + I * r_d.z;
        double c = A * r_o.x * r_o.x + B * r_o.y * r_o.y + C * r_o.z * r_o.z + D * r_o.x * r_o.y + E * r_o.y * r_o.z + F * r_o.x * r_o.z + G * r_o.x + H * r_o.y + I * r_o.z + J;
        double discriminant = b * b - 4 * a * c;
        double t = -1;
        if (discriminant >= 0)
        {
            double t1 = (-b + sqrt(discriminant)) / (2 * a);
            double t2 = (-b - sqrt(discriminant)) / (2 * a);
            if (t1 < 0 && t2 < 0)
                return -1;

            // cout<<"t1 "<<t1<<" t2 "<<t2<<endl;

            if (t2 < t1)
                swap(t1, t2);

            if (t1 > 0)
            {
                // cout<<"t1 "<<t1<<endl;
                Point intersectionPoint = ray.start + ray.dir * t1;
                // if (length == 0 || width == 0 || height == 0)
                // {
                //     return -1;
                // }
                if (insideReferenceCube(intersectionPoint))
                {
                    return t1;
                }
            }
            if (t2 > 0)
            {
                // cout<<"t2 "<<t2<<endl;
                Point intersectionPoint = ray.start + ray.dir * t2;
                // if (length == 0 || width == 0 || height == 0)
                // {
                //     return -1;
                // }
                if (insideReferenceCube(intersectionPoint))
                {
                    return t2;
                }
            }
            return -1;
        }
        return t;
    }
    virtual double intersect(Ray r, double *color, int level){
        return intersectHelper(r,color,level);
    }
    friend istream &operator>>(istream &in, General &o)
    {
        in >> o.A >> o.B >> o.C >> o.D >> o.E >> o.F >> o.G >> o.H >> o.I >> o.J;
        in >> o.reference_point >> o.length >> o.width >> o.height;
        for (int i = 0; i < 3; i++)
            in >> o.color[i];
        for (int i = 0; i < 4; i++)
            in >> o.coefficients[i];
        in >> o.shine;
        return in;
    }
    friend ostream &operator<<(ostream &out, General &o)
    {
        out << o.A << ' ' << o.B << ' ' << o.C << ' ' << o.D << ' ' << o.E << ' ' << o.F << ' ' << o.G << ' ' << o.H << ' ' << o.I << ' ' << o.J << endl;
        out << o.reference_point << ' ' << o.length << ' ' << o.width << ' ' << o.height << endl;
        for (int i = 0; i < 3; i++)
            out << o.color[i] << ' ';
        out << endl;
        for (int i = 0; i < 4; i++)
            out << o.coefficients[i] << ' ';
        out << endl;
        out << o.shine << endl;
        return out;
    }
};

class Floor : public Object
{
public:

    Floor(int floorWidth, int tileWidth)
    {
        reference_point = Point(-floorWidth / 2, -floorWidth / 2, 0, 1);
        length = tileWidth;
        width=floorWidth;
    }

    virtual vector<double> getColor(Point point)
    {
        int x_side_tile = (point.x - reference_point.x) / length;
        int y_side_tile = (point.y - reference_point.y) / length;
        vector<double> color(3, 0);
        if (point.x < reference_point.x || point.x > -reference_point.x || point.y < reference_point.y || point.y > -reference_point.y)
        {
            // checker board er baire hole kalo
            for (int i = 0; i < 3; i++)
            {
                color[i] = 0.0;
            }
            return color;
        }

        if (((x_side_tile + y_side_tile) % 2) == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                color[i] = 1.0;
            }
            return color;
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                color[i] = 0.0;
            }
            return color;
        }
    }

    virtual Ray getNormal(Point point, Ray ray)
    {
        if (ray.dir.z > 0)
            return Ray(point, Point(0, 0, 1, 1));
        else
            return Ray(point, Point(0, 0, -1, 1));
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
    virtual void draw()
    {
        int tiles_count = width / length;
        for (int i = -tiles_count / 2; i < tiles_count / 2; i++)
        {
            for (int j = -tiles_count / 2; j < tiles_count / 2; j++)
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
                glTranslatef((2 * i + 1) * (length / 2), (2 * j + 1) * (length / 2), 0);
                drawSquare(length / 2);
                glPopMatrix();
            }
        }
    }

    virtual double getIntersectionPoint_t(Ray ray, double *color, int level)
    {
        Point n(0, 0, 1, 1);
        double denom=n * ray.dir;
        if(denom==0)
        {
            return -1;
        }

        double t = -(n * ray.start) / (n * ray.dir);
        Point p = ray.start + ray.dir * t;
        if (p.x <= reference_point.x || p.x >= -(reference_point.x) || p.y <= reference_point.y || p.y >= -(reference_point.y))
        {
            return -1;
        }
        return t;
    }
    virtual double intersect(Ray r, double *color, int level){
        return intersectHelper(r,color,level);
    }
};
//
//void Refract(Point out, Point incidentVec, Point normal, double eta)
//{
//    double N_dot_I = normal * incidentVec;
//    double k = 1.0 - eta * eta * (1.f - N_dot_I * N_dot_I);
//    if (k < 0.f)
//        out = Point(0.0, 0.0, 0.0, 1.0);
//    else
//        out = incidentVec * eta - normal * (eta * N_dot_I + sqrt(k));
//}
