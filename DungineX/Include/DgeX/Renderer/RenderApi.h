
// Provide basic 2D rendering methods.

#pragma once

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

/**
 * @brief Draw a filled rectangle without border.
 * @param position center of the rectangle
 * @param size size of the rectangle
 * @param color color of the rectangle
 */
void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawFilledRect(const glm::mat4& transform, const glm::vec4& color);

/**
 * @brief Draw a rotated filled rectangle without border.
 * @param position position of the rectangle
 * @param size size of the rectangle
 * @param radian rotation in radian
 * @param color color of the rectangle
 */
void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color);
void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color);

/**
 * @brief Draw a border rectangle without filling.
 * @param position position of the rectangle
 * @param size size of the rectangle
 * @param color color of the rectangle
 * @note Use SetLineWidth to set the width of the border.
 */
void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
void DrawRect(const glm::mat4& transform, const glm::vec4& color);

/**
 * @brief Draw rotated border rectangle without filling.
 * @param position ...
 * @param size ...
 * @param radian ...
 * @param color ...
 */
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

/**
 * @brief Draw a filled circle.
 * @param position position of the circle
 * @param radius radius of the circle
 * @param color color of the circle
 */
void DrawFilledCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawFilledCircle(const glm::vec3& position, float radius, const glm::vec4& color);

/**
 * @brief Draw a border circle.
 * @param position position of the circle;
 * @param radius radius of the circle
 * @param color color of the circle
 * @note Use SetLineWidth to set the width of the border.
 */
void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);
void DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color);

/*
 * ===================================================================
 * --------------------------- Draw Line -----------------------------
 * ===================================================================
 */

/**
 * @brief Get the current line width.
 * @return current line width
 */
float GetLineWidth();

/**
 * @brief Set the current line width.
 * @param width line width
 * @warning Line width will not automatically adjust by the camera perspective.
 */
void SetLineWidth(float width);

/**
 * @brief Draw a line segment.
 * @param p0 starting point
 * @param p1 ending point
 * @param color color of the segment
 */
void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color);
void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

/**
 * @brief Draw line strips.
 * @note
 * Using this set of functions to draw lines is more efficient than using DrawLine.
 * @warning There must not be other rendering calls between BeginLine and EndLine.
 */
void BeginLine();
void BeginLine(const glm::vec4& color);
void AddPoint(const glm::vec2& point, const glm::vec4& color);
void AddPoint(const glm::vec3& point, const glm::vec4& color);
void AddPoint(const glm::vec2& point);
void AddPoint(const glm::vec3& point);
void EndLine();

/*
 * ===================================================================
 * --------------------------- Draw Text -----------------------------
 * ===================================================================
 */

struct TextStyle
{
    float FontSize = 48.0f;
    float LetterSpacing = 0.0f;
    float LineSpacing = 0.0f;
    bool Invert = true;
    const char* FontFamily = nullptr;
};

/**
 * @brief Set the color of the text.
 * @param color color of the text
 * @note The default color is white.
 * @note This will affect all following text rendering until next call.
 */
void SetTextColor(const glm::vec4& color);
void GetFontColor(glm::vec4* color);

/**
 * @brief Set the font style.
 * @param fontSize font size
 * @param fontFace font name.
 * @note The font name should be registered in the FontRegistry in advance.
 */
void SetTextStyle(float fontSize, const char* fontFace = nullptr);

/**
 * @brief Set letter spacing and line spacing.
 * @param letterSpacing extra space between letters
 * @param lineSpacing extra space between lines
 * @note It adds extra space to the default spacing.
 */
void SetTextStyle(float letterSpacing, float lineSpacing);

/**
 * @brief Set invert.
 * @param invert invert
 */
void InvertFont(bool invert);

/**
 * @brief Set all text style.
 * @param style the style to set
 */
void SetTextStyle(const TextStyle& style);
void GetTextStyle(TextStyle* style);

// clang-format off
using StringAlign = uint8_t;
enum StringAlignEnum : StringAlign
{
    SA_Left     = BIT(0),
    SA_Right    = BIT(1),
    SA_Center   = BIT(2),
    SA_Top      = BIT(3),
    SA_Bottom   = BIT(4),
    SA_Middle   = BIT(5),
    SA_MultiLine     = BIT(6),   // enable align per line
    SA_Default  = SA_Left | SA_Top
};

// clang-format on

/**
 * @brief Only calculate the width and height of the string.
 * @param string the string to draw
 * @param width return the width of rendered string
 * @param height return the height of rendered string
 */
void DrawString(const std::string& string, float* width, float* height);

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
