#include "DgeX/Renderer/RenderCommand.h"

#include <glad/glad.h>

#ifdef DGEX_OPENGL

DGEX_BEGIN

namespace RenderCommand
{

void Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void SetClearColor(const Color& color)
{
    glClearColor(color.R, color.G, color.B, color.A);
}

void ClearDevice()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SetLineWidth(float width)
{
    glLineWidth(width);
}

void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
{
    vertexArray->Bind();
    uint32_t count = indexCount ? indexCount : vertexArray->GetIndexCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void DrawCircle(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, uint32_t step)
{
    vertexArray->Bind();
    if (vertexCount == 0)
    {
        vertexCount = vertexArray->GetVertexCount();
    }
    if (step == 0)
    {
        step = vertexCount;
    }
    for (uint32_t offset = 0; offset < vertexCount; offset += step)
    {
        glDrawArrays(GL_TRIANGLE_FAN, offset, step);
    }
}

void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
{
    vertexArray->Bind();
    glDrawArrays(GL_LINES, 0, vertexCount ? vertexCount : vertexArray->GetVertexCount());
}

} // namespace RenderCommand

DGEX_END

#endif
