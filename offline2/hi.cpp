friend istream &operator>>(istream &in, Triangle &t)
    {
        in >> t.vertices[0] >> t.vertices[1] >> t.vertices[2];
        // t.arrangeVertices();
        return in;
    }