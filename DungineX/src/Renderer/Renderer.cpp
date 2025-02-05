#include "DgeX/Renderer/Renderer.h"

#include "DgeX/Application/Style/Color.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/RenderCommand.h"
#include "DgeX/Renderer/Shader.h"
#include "DgeX/Renderer/VertexArray.h"
#include "DgeX/Scene/Camera/Camera.h"

DGEX_BEGIN

Scope<Renderer::SceneData> Renderer::_data;

void Renderer::Init()
{
    RenderCommand::Init();
    RenderApi::Init();

    RenderApi::SetClearColor(Color::Black.ToVec4());
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
