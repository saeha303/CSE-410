#include "time.hpp"
#include <thread>

clk::time_point time::s_start_time_point;
clk::time_point time::s_frame_begin_time_point;
clk::time_point time::s_frame_end_time_point;
float time::s_delta_time_s = 0.0f;

const float &time::FPS()
{
    return s_FPS;
}

int64_t time::frame_time_ns()
{
    return (int64_t)(1000000000 / s_FPS);
}

void time::initialise()
{
    s_start_time_point = clk::now();
}

void time::start_frame()
{
    s_frame_begin_time_point = clk::now();
}

void time::end_frame()
{
    s_frame_end_time_point = clk::now();
    clk::duration nanoseconds = s_frame_end_time_point - s_frame_begin_time_point;

    if(nanoseconds.count() < frame_time_ns())
    {
        clk::duration sleep_time(frame_time_ns() - nanoseconds.count());

        std::this_thread::sleep_for(sleep_time);

        s_frame_end_time_point = clk::now();
        nanoseconds = s_frame_end_time_point - s_frame_begin_time_point;
    }

    s_delta_time_s = nanoseconds.count() / 1e9f;
}

const float &time::get_delta_time_s()
{
    return s_delta_time_s;
}

int64_t time::get_now_ns()
{
    clk::time_point now_time_point(clk::now());
    clk::duration time_point_ns(now_time_point - s_start_time_point);

    return time_point_ns.count();
}

float time::get_now_s()
{
    return get_now_ns() / 1e9f;
}