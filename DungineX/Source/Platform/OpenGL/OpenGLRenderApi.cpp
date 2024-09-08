#include <GLFW/glfw3.h>

// TODO: some functions in glad is too advanced for me to understand for now.
// #include <glad/glad.h>

#include "DgeX/Renderer/RenderApi.h"

#if DGEX_OPENGL

DGEX_BEGIN

namespace RenderApi
{

void SetClearColor(const Color& color)
{
    glClearColor(color.Red, color.Green, color.Blue, color.Alpha);
}

void ClearDevice()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SetColor(const Color& color)
{
    glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
}

void SetLineWidth(float width)
{
    glLineWidth(width);
}

void DrawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void DrawRectangle(float x, float y, float width, float height)
{
    glRectf(x, y, x + width, y + height);
}

} // namespace RenderApi

DGEX_END

#endif
