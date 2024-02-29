#include <iostream>
#include <GL/freeglut.h>
#include <optional>
#include <chrono>
#include <set>
#include <functional>
#include <thread>
#include <cmath>
#include "time.hpp"

constexpr size_t circle_segments = 50;
constexpr size_t wave_segments = 100;
constexpr float radius = 0.4f;
constexpr float center_x = -0.5f;
constexpr float center_y = 0.0f;
constexpr float frequency = 0.5f;
constexpr float wave_speed = 2.0f;
size_t frame_count = 0;
float end_x;
float end_y;

void draw_circle()
{
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);

    for(size_t i = 0; i < circle_segments; ++i)
    {
        const size_t start_idx = i;
        const size_t end_idx = (i + 1) % circle_segments;
        const float line_start_x = std::cos(start_idx * 2.0f * M_PI / circle_segments) * radius + center_x;
        const float line_start_y = std::sin(start_idx * 2.0f * M_PI / circle_segments) * radius + center_y;
        const float line_end_x = std::cos(end_idx * 2.0f * M_PI / circle_segments) * radius + center_x;
        const float line_end_y = std::sin(end_idx * 2.0f * M_PI / circle_segments) * radius + center_y;
        glVertex2f(line_start_x, line_start_y);
        glVertex2f(line_end_x, line_end_y);
    }

    glEnd();
}

void draw_circle_line()
{
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(center_x, center_y);
    glVertex2f(end_x, end_y);
    glEnd();
}

void draw_up_down_line()
{
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0, end_y);
    glVertex2f(end_x, end_y);
    glEnd();
}

void draw_wave()
{    
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);

    for(size_t i = 0; i < wave_segments - 1; ++i)
    {
        const size_t start_idx = i;
        const size_t end_idx = i + 1;
        const float line_start_x = (float)start_idx / wave_segments;
        const float line_start_y = radius * std::sin(frame_count * frequency * 2.0f * M_PI / time::FPS() - start_idx * wave_speed * 2.0f * M_PI / wave_segments);
        const float line_end_x = (float)end_idx / wave_segments;
        const float line_end_y = radius * std::sin(frame_count * frequency * 2.0f * M_PI / time::FPS() - end_idx * wave_speed * 2.0f * M_PI / wave_segments);

        glVertex2f(line_start_x, line_start_y);
        glVertex2f(line_end_x, line_end_y);
    }

    glEnd();
}

void display_callback()  // draw each frame
{
    time::start_frame();
    glClear(GL_COLOR_BUFFER_BIT);

    end_x = center_x + std::cos(frame_count * frequency * 2.0 * M_PI / time::FPS()) * radius;
    end_y = center_y + std::sin(frame_count * frequency * 2.0 * M_PI / time::FPS()) * radius;

    draw_circle();
    draw_circle_line();
    draw_up_down_line();
    draw_wave();

    glutSwapBuffers();
    glutPostRedisplay();    // draw next frame
    time::end_frame();

    ++frame_count;
}

int main(int argc, char **argv)
{
    time::initialise();

    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutCreateWindow("online-1-a1");
    glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glutDisplayFunc(display_callback);
    glutMainLoop();

    return 0;
}