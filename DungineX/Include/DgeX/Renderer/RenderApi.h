
// Provide basic 2D rendering methods.

#pragma once

#include "DgeX/Common/Common.h"

#include <glm/glm.hpp>

DGEX_BEGIN

class Texture;
class Camera;
class Font;

namespace RenderApi
{

void Init();
void Shutdown();

void SetClearColor(const glm::vec4& color, glm::vec4* old = nullptr);

void BeginScene(const Camera& camera, const glm::mat4& transform);
void BeginScene(const Camera& camera);
void EndScene();
void Flush();

/*
 * ===================================================================
 * ------------------------- Draw Rectangle --------------------------
 * ===================================================================
 */

void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::mat4& transform, const glm::vec4& color);

void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color);

void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::mat4& transform, const glm::vec4& color);

void DrawRotatedRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color);

/*
 * ===================================================================
 * ------------------------- Draw Texture ----------------------------
 * ===================================================================
 */

void DrawTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,
                 float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
void DrawTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,
                 float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

void DrawRotatedTexture(const glm::vec2& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                        float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
void DrawRotatedTexture(const glm::vec3& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                        float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

void DrawTexture(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0f,
                 const glm::vec4& tintColor = glm::vec4(1.0f));

/*
 * ===================================================================
 * --------------------------- Draw Circle ---------------------------
 * ===================================================================
 */

void DrawFilledCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawFilledCircle(const glm::vec3& position, float radius, const glm::vec4& color);

void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color);

/*
 * ===================================================================
 * --------------------------- Draw Line -----------------------------
 * ===================================================================
 */

float GetLineWidth();
void SetLineWidth(float width);

void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

/*
 * ===================================================================
 * --------------------------- Draw Text -----------------------------
 * ===================================================================
 */

struct FontStyle
{
    glm::vec4 Color;
    float FontSize;
    float LetterSpacing = 0.0f;
    float LineSpacing = 0.0f;
};

void SetFontFamily(float fontSize, const Ref<Font>& font = nullptr);
bool InvertFont();
void InvertFont(bool invert);
void GetFontColor(glm::vec4* color);
void SetFontColor(const glm::vec4& color, glm::vec4* old);
void GetFontStyle(FontStyle* style);
void SetFontStyle(const FontStyle* style, FontStyle* old);

// clang-format off
using StringAlign = uint8_t;
enum StringAlignEnum : StringAlign
{
    SA_Default  = 0,
    SA_Left     = 0,
    SA_Top      = 0,

    SA_Right    = BIT(0),
    SA_Center   = BIT(1),
    SA_Bottom   = BIT(2),
    SA_VCenter  = BIT(3),
};

// clang-format on

void DrawString(const std::string& string, const glm::vec2& position, StringAlign align = SA_Default);
void DrawString(const std::string& string, const glm::vec3& position, StringAlign align = SA_Default);

void DrawString(const std::string& string, const glm::vec2& position, const glm::vec4& color,
                StringAlign align = SA_Default);
void DrawString(const std::string& string, const glm::vec3& position, const glm::vec4& color,
                StringAlign align = SA_Default);

void DrawRotatedString(const std::string& string, const glm::vec2& position, float radian,
                       StringAlign align = SA_Default);
void DrawRotatedString(const std::string& string, const glm::vec3& position, float radian,
                       StringAlign align = SA_Default);

void DrawRotatedString(const std::string& string, const glm::vec2& position, float radian, const glm::vec4& color,
                       StringAlign align = SA_Default);
void DrawRotatedString(const std::string& string, const glm::vec3& position, float radian, const glm::vec4& color,
                       StringAlign align = SA_Default);

void DrawString(const glm::mat4& transform, const std::string& string, StringAlign align = SA_Default);
void DrawString(const glm::mat4& transform, const std::string& string, const glm::vec4& color,
                StringAlign align = SA_Default);

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
