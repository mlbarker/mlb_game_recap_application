#ifndef _RENDER_COORDS_H
#define _RENDER_COORDS_H

struct RenderCoords
{
    int32_t x{ 0 };
    int32_t y{ 0 };
    int32_t width{ 0 };
    int32_t height{ 0 };
    bool scaled{ false };
};

#endif // _RENDER_COORDS_H
