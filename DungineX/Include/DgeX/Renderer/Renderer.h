#pragma once

#include "DgeX/dgexpch.h"

#include "DgeX/Renderer/Camera/Camera.h"
#include "DgeX/Renderer/Shader.h"
#include "Dgex/Renderer/VertexArray.h"

DGEX_BEGIN

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(int width, int height);

    static void BeginScene(Camera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
                       const glm::mat4& transform = glm::mat4(1.0f));

private:
    struct SceneData
    {
        glm::mat4 ViewProjection;
    };

    static Scope<SceneData> _data;
};

DGEX_END
