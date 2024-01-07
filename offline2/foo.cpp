#include <bits/stdc++.h>
#define pi 2.0 * acos(0.0)
using namespace std;
int counter = 0;
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
    friend ostream &operator<<(ostream &out, Point &p)
    {
        out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return out;
    }

    ~Point()
    {
    }
};
class Matrix
{
public:
    vector<Point> mat;
    int order;
    Matrix()
    {
        order = 4;
        mat.resize(order);
    }
    Matrix(int order)
    {
        this->order = order;
        mat.resize(order);
    }
    Matrix operator*(Matrix other)
    {
        Matrix temp(order);
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                for (int k = 0; k < order; k++)
                {
                    temp.mat[i][j] += mat[i][k] * other.mat[k][j];
                }
            }
        }
        return temp;
    }
    Point operator*(Point other)
    {
        double sum[4], temp[4];
        temp[0] = other.x;
        temp[1] = other.y;
        temp[2] = other.z;
        temp[3] = other.w;
        for (int i = 0; i < order; i++)
        {
            sum[i] = 0;
            for (int j = 0; j < order; j++)
            {
                sum[i] += mat[i][j] * temp[j];
            }
        }
        Point result(sum[0], sum[1], sum[2], sum[3]);
        // cout<<"in *: ";
        // cout<<result<<endl;
        result.divideByW();
        // cout<<"after divide: ";
        // cout<<result<<endl;
        return result;
    }
    void buildIdentityMat()
    {
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                if (i == j)
                    mat[i][j] = 1;
                else
                    mat[i][j] = 0;
            }
        }
    }
    void prepareMat()
    {
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                mat[i][j] = 0;
            }
        }
        mat[3][3] = 1;
    }
    void buildTranslationMat(Point p)
    {
        // prepareMat();
        buildIdentityMat();
        mat[0][3] = p.x;
        mat[1][3] = p.y;
        mat[2][3] = p.z;
    }
    void buildScalingMat(Point p)
    {
        // prepareMat();
        buildIdentityMat();
        mat[0][0] = p.x;
        mat[1][1] = p.y;
        mat[2][2] = p.z;
    }
    Point RodriguesFormula(Point x, Point a, double angle)
    {
        double angleRad = angle * pi / 180;
        // Point t=a&x;
        // cout<<t<<'\n';
        return x * cos(angleRad) + a * (a * x) * (1 - cos(angleRad)) + (a & x) * sin(angleRad);
    }
    void buildRotationMat(double angle, Point p)
    {
        // prepareMat();
        buildIdentityMat();
        Point a(p.x, p.y, p.z, 1);
        a.normalize();
        Point i(1, 0, 0, 1);
        Point j(0, 1, 0, 1);
        Point k(0, 0, 1, 1);
        Point c1 = RodriguesFormula(i, a, angle);
        Point c2 = RodriguesFormula(j, a, angle);
        Point c3 = RodriguesFormula(k, a, angle);

        mat[0][0] = c1.x;
        mat[1][0] = c1.y;
        mat[2][0] = c1.z;

        mat[0][1] = c2.x;
        mat[1][1] = c2.y;
        mat[2][1] = c2.z;

        mat[0][2] = c3.x;
        mat[1][2] = c3.y;
        mat[2][2] = c3.z;
    }
    void buildViewMat(Point eye, Point r, Point u, Point l)
    {
        buildIdentityMat();
        mat[0][0] = r.x;
        mat[0][1] = r.y;
        mat[0][2] = r.z;

        mat[1][0] = u.x;
        mat[1][1] = u.y;
        mat[1][2] = u.z;

        mat[2][0] = -l.x;
        mat[2][1] = -l.y;
        mat[2][2] = -l.z;
        Matrix t;
        Point p(-eye.x, -eye.y, -eye.z, 1);
        t.buildTranslationMat(p);
        *this = (*this) * t;
    }
    void buildProjectionMat(double near, double far, double r, double t)
    {
        buildIdentityMat();
        mat[0][0] = near / r;
        mat[1][1] = near / t;
        mat[2][2] = -(far + near) / (far - near);
        mat[3][2] = -1;
        mat[2][3] = -2.0 * far * near / (far - near);
        mat[3][3] = 0;
    }
    ~Matrix()
    {
    }
};
static unsigned long int g_seed = 1;
inline int random_()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

class Triangle
{
public:
    int id;
    Point vertices[3];
    int color[3];

    Triangle()
    {
        id=counter++;
        pickColor();
    }

    Triangle(Point p1, Point p2, Point p3)
    {
        id=counter++;
        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;
        // // debugging
        // cout << p1 << endl;
        // cout << p2 << endl;
        // cout << p3 << endl;
        // cout << endl;
        // cout<<*this<<'\n';
        arrangeVertices();
        pickColor();
    }
    void pickColor()
    {
        color[0] = random_();
        color[1] = random_();
        color[2] = random_();
    }
    int maxY()
    {
        int idx = -1;
        double maxY = (double)INT_MIN;
        for (int i = 0; i < 3; i++)
        {
            if ((vertices[i].y > maxY && !vertices[i].taken) || (vertices[i].y == maxY && vertices[i].x < vertices[idx].x && !vertices[i].taken))
            {
                idx = i;
                maxY = vertices[i].y;
            }
        }
        return idx;
    }
    int minX()
    {
        int idx = -1;
        double minX = (double)INT_MAX;
        for (int i = 0; i < 3; i++)
        {
            if (vertices[i].x < minX && !vertices[i].taken)
            {
                idx = i;
                minX = vertices[i].x;
            }
        }
        return idx;
    }
    void arrangeVertices()
    {
        int count = 0;
        int idx = maxY();
        Point maxY = vertices[idx];
        vertices[idx].taken = true;
        idx = minX();
        Point minX = vertices[idx];
        vertices[idx].taken = true;
        Point other;
        for (int i = 0; i < 3; i++)
        {
            if (!vertices[i].taken)
            {
                count++;
                other = vertices[i];
            }
        }
        assert(count == 1);
        vertices[0] = maxY;
        vertices[1] = minX;
        vertices[2] = other;
        // cout<<"***********"<<'\n';
        // for(int i=0;i<3;i++){
        //     cout<<vertices[i]<<'\n';
        // }
        // cout<<"^^^^^^^^^^^"<<'\n';
    }
    friend ostream &operator<<(ostream &out, Triangle &p)
    {
        out << fixed << setprecision(7) << p.vertices[0].x << " " << p.vertices[0].y << " " << p.vertices[0].z << "//";
        out << fixed << setprecision(7) << p.vertices[1].x << " " << p.vertices[1].y << " " << p.vertices[1].z << "//";
        out << fixed << setprecision(7) << p.vertices[2].x << " " << p.vertices[2].y << " " << p.vertices[2].z;
        return out;
    }
};