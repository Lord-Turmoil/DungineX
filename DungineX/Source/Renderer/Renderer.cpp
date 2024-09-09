#include "DgeX/Renderer/Renderer.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/RenderCommand.h"

DGEX_BEGIN

Scope<Renderer::SceneData> Renderer::_data;

void Renderer::Init()
{
    RenderCommand::Init();
    RenderApi::Init();

    RenderCommand::SetClearColor(Color::FromUInt32(0xFF000000));
}

void Renderer::Shutdown()
{
    RenderApi::Shutdown();
}

void Renderer::OnWindowResize(int width, int height)
{
    RenderCommand::SetViewport(0, 0, width, height);
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
    RenderCommand::DrawIndexed(vertexArray);
}

DGEX_END
