rm stage1.txt
rm stage2.txt
rm stage3.txt
rm z_buffer.txt
rm out.bmp
g++ raster.cpp -o main -lglut -lGLU -lGL
./main