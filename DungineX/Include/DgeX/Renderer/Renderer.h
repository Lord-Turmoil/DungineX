#pragma once

#include "DgeX/Common/Common.h"

#include <glm/glm.hpp>

DGEX_BEGIN

class VertexArray;
class Shader;
class Camera;

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
