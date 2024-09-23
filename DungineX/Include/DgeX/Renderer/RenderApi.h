
// Provide basic 2D rendering methods.

#pragma once

#include "DgeX/Renderer/Camera/Camera.h"
#include "DgeX/Renderer/Texture.h"

DGEX_BEGIN

namespace RenderApi
{

void Init();
void Shutdown();

void SetClearColor(const glm::vec4& color, glm::vec4* old = nullptr);

void BeginScene(const Camera& camera, const glm::mat4& transform);
void BeginScene(const Camera& camera);
void EndScene();
void Flush();

void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,
                    float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,
                    float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

void DrawFilledRect(const glm::mat4& transform, const glm::vec4& color);
void DrawFilledRect(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0f,
                    const glm::vec4& tintColor = glm::vec4(1.0f));

void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                           float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                           float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::mat4& transform, const glm::vec4& color);

void DrawRotatedRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color);

void DrawFilledCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawFilledCircle(const glm::vec3& position, float radius, const glm::vec4& color);

void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color);

void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

// static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

struct TextParams
{
    glm::vec4 Color{ 1.0f }; // { 1.0f, 1.0f, 1.0f, 1.0f }
    float LetterSpacing = 0.0f;
    float LineSpacing = 0.0f;
};

// static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform,
//                        const TextParams& textParams);
// static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component,
//                        );

float GetLineWidth();
void SetLineWidth(float width);

// Stats
struct Statistics
{
    uint32_t DrawCalls = 0;
    uint32_t QuadCount = 0;

    uint32_t GetTotalVertexCount() const
    {
        return QuadCount * 4;
    }

    uint32_t GetTotalIndexCount() const
    {
        return QuadCount * 6;
    }
};

void ResetStats();
Statistics GetStats();

} // namespace RenderApi

DGEX_END
