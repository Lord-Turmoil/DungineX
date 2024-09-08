#include "DgeX/Renderer/Renderer.h"

#include "DgeX/Renderer/RenderApi.h"

DGEX_BEGIN

void Renderer::Init()
{
    RenderApi::Init();
}

void Renderer::Shutdown()
{
}

void Renderer::OnWindowResize(int width, int height)
{
    RenderApi::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(Camera& camera)
{
    _data->ViewProjection = camera.GetProjection();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
{
    shader->Bind();
    shader->SetMat4("u_ViewProjection", _data->ViewProjection);
    shader->SetMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderApi::(vertexArray);
}

DGEX_END
