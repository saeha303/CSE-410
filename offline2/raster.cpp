#include "utils.cpp"
#include "bitmap_image.hpp"
using namespace std;
// int counter=0;
int main()
{
    Point eye, look, up;
    double fovY, aspectRatio, near, far;
    int count = 0;
    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");
    fin >> eye >> look >> up;
    fin >> fovY >> aspectRatio >> near >> far;

    // stage 1
    Matrix M;
    M.buildIdentityMat();
    stack<Matrix> st;
    st.push(M);
    while (true)
    {
        string cmd;
        fin >> cmd;
        if (cmd == "triangle")
        {
            count++;
            Point p1, p2, p3;
            fin >> p1 >> p2 >> p3;
            p1 = st.top() * p1;
            p2 = st.top() * p2;
            p3 = st.top() * p3;
            fout << p1 << endl;
            fout << p2 << endl;
            fout << p3 << endl;
            fout << endl;
        }
        else if (cmd == "translate")
        {
            Point p1;
            fin >> p1;
            Matrix temp;
            temp.buildTranslationMat(p1);
            Matrix mat = st.top() * temp;
            st.pop();
            st.push(mat);
        }
        else if (cmd == "scale")
        {
            Point p1;
            fin >> p1;
            Matrix temp;
            temp.buildScalingMat(p1);
            Matrix mat = st.top() * temp;
            st.pop();
            st.push(mat);
        }
        else if (cmd == "rotate")
        {
            double angle;
            Point p1;
            fin >> angle;
            fin >> p1;
            Matrix temp;
            temp.buildRotationMat(angle, p1);
            Matrix mat = st.top() * temp;
            st.pop();
            st.push(mat);
        }
        else if (cmd == "push")
        {
            st.push(st.top());
        }
        else if (cmd == "pop")
        {
            if (st.empty())
            {
                cout << "Stack is empty" << endl;
                return 0;
            }

            st.pop();
        }
        else if (cmd == "end")
        {
            break;
        }
        else
        {
            cout << cmd << ": No such command" << endl;
            break;
        }
    }

    fin.close();
    fout.close();

    Matrix temp;
    // stage 2
    fin.open("stage1.txt");
    fout.open("stage2.txt");
    Point l = look - eye;
    l.normalize();
    Point r = l & up;
    r.normalize();
    Point u = r & l;
    u.normalize();
    temp.buildViewMat(eye, r, u, l);
    for (int i = 0; i < count; i++)
    {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        p1 = temp * p1;
        p2 = temp * p2;
        p3 = temp * p3;
        fout << p1 << endl;
        fout << p2 << endl;
        fout << p3 << endl;
        fout << endl;
    }
    fin.close();
    fout.close();

    // stage 3
    fin.open("stage2.txt");
    fout.open("stage3.txt");
    double fovX = fovY * aspectRatio;
    double rad_angle = (fovY / 2) * pi / 180.0;
    double t = near * tan(rad_angle);
    rad_angle = (fovX / 2) * pi / 180.0;
    double r_ = near * tan(rad_angle);
    temp.buildProjectionMat(near, far, r_, t);
    for (int i = 0; i < count; i++)
    {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        p1 = temp * p1;
        p2 = temp * p2;
        p3 = temp * p3;
        fout << p1 << endl;
        fout << p2 << endl;
        fout << p3 << endl;
        fout << endl;
    }
    fin.close();
    fout.close();

    // stage 4
    // 1. Read data
    fin.open("config.txt");
    int screen_width, screen_height;
    fin >> screen_width >> screen_height;
    // cout<<screen_width<<" "<<screen_height<<'\n';
    vector<Triangle> triMat;
    fin.close();
    fin.open("stage3.txt");
    for (int i = 0; i < count; i++)
    {
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        // //debugging
        // cout << p1 << endl;
        // cout << p2 << endl;
        // cout << p3 << endl;
        // cout << endl;
        Triangle t(p1, p2, p3);
        triMat.push_back(t);
        // cout<<t<<'\n';
    }
    fin.close();
    // 2. Initialize z-buffer and frame buffer
    double xLeft = -1, xRight = 1, yUp = 1, yDown = -1, zFront = -1, zRear = 1;
    double dx = (xRight - xLeft) / screen_width;
    double dy = (yUp - yDown) / screen_height;
    // center of attention
    double yMaxCenter = yUp - dy / 2;
    double yMinCenter = yDown + dy / 2;
    double xMinCenter = xLeft + dx / 2;
    double xMaxCenter = xRight - dx / 2;
    // cout<<
    vector<vector<double>> zBuffer(screen_height, vector<double>(screen_width, zRear));
    bitmap_image img(screen_width, screen_height);
    for (int i = 0; i < screen_width; i++)
    {
        for (int j = 0; j < screen_height; j++)
        {
            img.set_pixel(i, j, 0, 0, 0);
        }
    }
    // 3. Apply procedure
    fout.open("namumkin.txt");
    for (int i = 0; i < count; i++)
    {
        Triangle t = triMat[i];
        double minX = min(min(t.vertices[0].x, t.vertices[1].x), t.vertices[2].x);
        double maxX = max(max(t.vertices[0].x, t.vertices[1].x), t.vertices[2].x);
        double minY = min(min(t.vertices[0].y, t.vertices[1].y), t.vertices[2].y);
        double maxY = max(max(t.vertices[0].y, t.vertices[1].y), t.vertices[2].y);
        // cout<<minX<<" "<<maxX<<" "<<minY<<" "<<maxY<<'\n';
        // after clipping, we need clipping because in case the object is outside our bounded box
        minX = max(minX, xMinCenter);
        maxX = min(maxX, xMaxCenter);
        minY = max(minY, yMinCenter);
        maxY = min(maxY, yMaxCenter);
        // fout<<minY<<" "<<maxY<<'\n';
        // cout<<minX<<" "<<maxX<<" "<<minY<<" "<<maxY<<'\n';
        // what is the center of the box where minY, maxY are located?
        int topScanLineRow = round((yMaxCenter - maxY) / dy);
        int bottomScanLineRow = round((yMaxCenter - minY) / dy);
        // cout<<topScanLineRow<<" "<<bottomScanLineRow<<'\n';

        for (int top = topScanLineRow; top <= bottomScanLineRow; top++)
        {
            if(topScanLineRow==bottomScanLineRow) continue;
            double ys = yMaxCenter - top * dy;
            double left_intersecting_x=0, left_intersecting_z=0, right_intersecting_x=0, right_intersecting_z=0;
            // cout<<ys<<'\n';
            if (t.vertices[0].y == t.vertices[1].y)
                continue;
            if (ys >= t.vertices[1].y && ys <= t.vertices[0].y)
            {
                left_intersecting_x = t.vertices[0].x + ((t.vertices[0].x - t.vertices[1].x) * (t.vertices[0].y - ys)) / (t.vertices[1].y - t.vertices[0].y);
                left_intersecting_z = t.vertices[0].z + ((t.vertices[0].z - t.vertices[1].z) * (t.vertices[0].y - ys)) / (t.vertices[1].y - t.vertices[0].y);
            }
            if (t.vertices[0].y == t.vertices[2].y)
                continue;
            if (ys >= t.vertices[2].y && ys <= t.vertices[0].y)
            {
                right_intersecting_x = t.vertices[0].x + ((t.vertices[0].x - t.vertices[2].x) * (t.vertices[0].y - ys)) / (t.vertices[2].y - t.vertices[0].y);
                right_intersecting_z = t.vertices[0].z + ((t.vertices[0].z - t.vertices[2].z) * (t.vertices[0].y - ys)) / (t.vertices[2].y - t.vertices[0].y);
            }
            double org_left_x = left_intersecting_x, org_right_x = right_intersecting_x;
            if (left_intersecting_x < minX)
            {
                left_intersecting_x = minX;
            }
            if (left_intersecting_x > maxX)
            {
                left_intersecting_x = maxX;
            }
            if (right_intersecting_x < minX)
            {
                right_intersecting_x = minX;
            }
            if (right_intersecting_x > maxX)
            {
                right_intersecting_x = maxX;
            }
            
            // left_intersecting_z = right_intersecting_z + ((left_intersecting_z - right_intersecting_z) * (left_intersecting_x - org_left_x)) / (org_left_x - org_right_x);
            // right_intersecting_z = right_intersecting_z + ((left_intersecting_z - right_intersecting_z) * (right_intersecting_x - org_right_x)) / (org_left_x - org_right_x);
            // if(left_intersecting_x>right_intersecting_x){
            //     swap(left_intersecting_x,right_intersecting_x);
            //     swap(left_intersecting_z,right_intersecting_z);
            // }
            // fout<<left_intersecting_x<<" "<<right_intersecting_x<<" "<<left_intersecting_z<<" "<<right_intersecting_z<<'\n';
            
            int leftScanLineCol = round((left_intersecting_x - xMinCenter) / dx);
            int rightScanLineCol = round((right_intersecting_x - xMinCenter) / dx);
            // fout << leftScanLineCol << " " << rightScanLineCol << endl;
            
            for (int left = leftScanLineCol; left <= rightScanLineCol; left++)
            {
                if(leftScanLineCol==rightScanLineCol) continue;
                // counter++;
                // cout<<counter<<":";
                double xp = xMinCenter + left * dx;
                // fout<<xp<<'\n';
                double temp = ((left_intersecting_z - right_intersecting_z) * (xp - left_intersecting_x)) / (left_intersecting_x - right_intersecting_x);
                double zp = temp + left_intersecting_z;
                fout<<zp<<" "<<t.color[0]<<" "<<t.color[1]<<" "<<t.color[2]<<" "<<t.vertices[0]<<'\n';
                // fout<<"********************"<<'\n';
                if (zp < zBuffer[top][left] && zp >= zFront)
                {
                    zBuffer[top][left] = zp;
                    // fout<<zp<<" "<<t.color[0]<<" "<<t.color[1]<<" "<<t.color[2]<<" "<<t.vertices[0]<<'\n';
                    img.set_pixel(left, top, t.color[0], t.color[1], t.color[2]);
                }
            }
        }
    }
    fout.close();
    fout.open("z_buffer.txt");
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            if (zBuffer[i][j] < zRear)
            {
                fout << setprecision(6) << fixed << zBuffer[i][j] << "\t";
            }
        }
        fout << endl;
    }

    fout.close();

    img.save_image("out.bmp");

    zBuffer.clear();
    zBuffer.shrink_to_fit();

    return 0;
}