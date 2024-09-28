/*
 * This file comes from Hazel Engine, with some modifications.
 *
 * Since the rendering methods are anonymous (no explicit class defined in user
 * program), we store the rendering data in pre-allocated buffers with default
 * shaders and textures.
 *
 * Here we support rectangle (quad), circle and line rendering, which is quite
 * sufficient for 2D rendering. We also support text and sprite (image) rendering
 * for better visual effects.
 *
 * How do we render shapes of different size? Using transform.
 * We define the basic shapes in local, e.g. square, then use transform to scale
 * and rotate them to the desired size and rotation.
 */

#include "DgeX/Platform/OpenGL/OpenGLShaderTemplate.h"
#include "DgeX/Renderer/Buffer.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/RenderCommand.h"
#include "DgeX/Renderer/Shader.h"
#include "DgeX/Renderer/Texture.h"
#include "DgeX/Renderer/UniformBuffer.h"
#include "DgeX/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DgeX/Utils/Math.h"

#ifdef DGEX_OPENGL

DGEX_BEGIN

namespace RenderApi
{

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

struct CircleVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

struct LineVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

struct TextVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;

    // TODO: bg color for outline/bg
};

struct RendererData
{
    static const uint32_t MaxTextureSlots = 32;

    static const uint32_t MaxQuad = 10000;
    static const uint32_t MaxQuadVertices = MaxQuad * 4;
    static const uint32_t MaxQuadIndices = MaxQuad * 6;

    static const uint32_t MaxCircle = 100;
    static const uint32_t CircleFragments = 30;
    static const uint32_t CircleVertices = CircleFragments + 2;
    static const uint32_t MaxCircleVertices = MaxCircle * CircleVertices;

    static const uint32_t MaxLine = 10000;
    static const uint32_t MaxLineVertices = MaxLine * 2;

    static const uint32_t MaxText = 1000;
    static const uint32_t MaxTextVertices = MaxText * 4;
    static const uint32_t MaxTextIndices = MaxText * 6;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;
    Ref<Texture> WhiteTexture;

    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;
    Ref<Shader> CircleShader;

    Ref<VertexArray> LineVertexArray;
    Ref<VertexBuffer> LineVertexBuffer;
    Ref<Shader> LineShader;

    Ref<VertexArray> TextVertexArray;
    Ref<VertexBuffer> TextVertexBuffer;
    Ref<Shader> TextShader;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    uint32_t CircleVertexCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    uint32_t LineVertexCount = 0;
    LineVertex* LineVertexBufferBase = nullptr;
    LineVertex* LineVertexBufferPtr = nullptr;

    uint32_t TextIndexCount = 0;
    TextVertex* TextVertexBufferBase = nullptr;
    TextVertex* TextVertexBufferPtr = nullptr;

    float LineWidth = 2.0f;

    std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

    glm::vec4 QuadVertexPositions[4];

    Ref<Texture> FontAtlasTexture;
    const MsdfData* FontData;
    FontStyle CurrentFontStyle;
    bool InvertFont; // Invert Y-axis of font.

    Statistics Stats;

    struct CameraData
    {
        glm::mat4 ViewProjection;
    };

    CameraData CameraBuffer;
    Ref<UniformBuffer> CameraUniformBuffer;
};

static RendererData sData;

static void StartBatch();
static void NextBatch();

void Init()
{
    DGEX_TIME_BEGIN("Initialize OpenGL render API");

    sData.QuadVertexBuffer = VertexBuffer::Create(RendererData::MaxQuadVertices * sizeof(QuadVertex));
    sData.QuadVertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" },
                                        { ShaderDataType::Float4, "a_Color" },
                                        { ShaderDataType::Float2, "a_TexCoord" },
                                        { ShaderDataType::Float, "a_TexIndex" },
                                        { ShaderDataType::Float, "a_TilingFactor" } });
    sData.QuadVertexArray = VertexArray::Create();
    sData.QuadVertexArray->AddVertexBuffer(sData.QuadVertexBuffer);
    sData.QuadVertexBufferBase = new QuadVertex[RendererData::MaxQuadVertices];

    uint32_t* quadIndices = new uint32_t[RendererData::MaxQuadIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < RendererData::MaxQuadIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, RendererData::MaxQuadIndices);
    sData.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    // Circles
    sData.CircleVertexBuffer = VertexBuffer::Create(RendererData::MaxCircleVertices * sizeof(CircleVertex));
    sData.CircleVertexBuffer->SetLayout(
        { { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float4, "a_Color" } });

    sData.CircleVertexArray = VertexArray::Create();
    sData.CircleVertexArray->AddVertexBuffer(sData.CircleVertexBuffer);
    sData.CircleVertexBufferBase = new CircleVertex[RendererData::MaxCircleVertices];

    // Lines
    sData.LineVertexBuffer = VertexBuffer::Create(RendererData::MaxLineVertices * sizeof(LineVertex));
    sData.LineVertexBuffer->SetLayout(
        { { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float4, "a_Color" } });
    sData.LineVertexArray = VertexArray::Create();
    sData.LineVertexArray->AddVertexBuffer(sData.LineVertexBuffer);
    sData.LineVertexBufferBase = new LineVertex[RendererData::MaxLineVertices];

    // Text
    sData.TextVertexArray = VertexArray::Create();

    sData.TextVertexBuffer = VertexBuffer::Create(RendererData::MaxQuadVertices * sizeof(TextVertex));
    sData.TextVertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" },
                                        { ShaderDataType::Float4, "a_Color" },
                                        { ShaderDataType::Float2, "a_TexCoord" } });
    sData.TextVertexArray->AddVertexBuffer(sData.TextVertexBuffer);
    sData.TextVertexArray->SetIndexBuffer(quadIB);
    sData.TextVertexBufferBase = new TextVertex[RendererData::MaxQuadVertices];

    sData.WhiteTexture = Texture::Create(TextureSpecification());
    uint32_t whiteTextureData = 0xFFFFFFFF;
    sData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    {
        using namespace ShaderTemplate::OpenGL;
        sData.QuadShader = Shader::Create("__QuadShader", QUAD_VS, QUAD_FS);
        sData.CircleShader = Shader::Create("__CircleShader", CIRCLE_VS, CIRCLE_FS);
        sData.LineShader = Shader::Create("__LineShader", LINE_VS, LINE_FS);
        sData.TextShader = Shader::Create("__TextShader", TEXT_VS, TEXT_FS);
    }

    // Set first texture slot to 0
    sData.TextureSlots[0] = sData.WhiteTexture;

    sData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    sData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    sData.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    sData.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

    DGEX_TIME_BEGIN("Initialize font");
    FontRegistry::Init();
    sData.FontAtlasTexture = FontRegistry::GetDefault()->GetAtlasTexture();
    sData.FontData = FontRegistry::GetDefault()->GetMsdfData();
    sData.CurrentFontStyle = { { 1.0f, 1.0f, 1.0f, 1.0f }, 48.0f, 0.0f, 0.0f };
    sData.InvertFont = true;
    DGEX_TIME_END();

    sData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);

    DGEX_TIME_END();
}

void Shutdown()
{
    DGEX_TIME_BEGIN("Shutdown render API");

    delete[] sData.QuadVertexBufferBase;
    sData.QuadVertexBufferBase = nullptr;
    delete[] sData.CircleVertexBufferBase;
    sData.CircleVertexBufferBase = nullptr;
    delete[] sData.LineVertexBufferBase;
    sData.LineVertexBufferBase = nullptr;
    delete[] sData.TextVertexBufferBase;
    sData.TextVertexBufferBase = nullptr;

    // Warning:
    // must recycle these global static variables to ensure they are deleted
    // before logger is destroyed.
    sData.QuadShader.reset();
    sData.CircleShader.reset();
    sData.LineShader.reset();
    sData.TextShader.reset();

    sData.TextureSlots.fill(nullptr);
    sData.WhiteTexture.reset();

    DGEX_TIME_BEGIN("Shutdown font registry");
    FontRegistry::Shutdown();
    sData.FontAtlasTexture.reset();
    DGEX_TIME_END();

    DGEX_TIME_END();
}

static glm::vec4 sClearColor = { 0.f, 0.f, 0.f, 1.f };

void SetClearColor(const glm::vec4& color, glm::vec4* old)
{
    RenderCommand::SetClearColor({ color.r, color.g, color.b, color.a });
    if (old)
    {
        *old = sClearColor;
    }
    sClearColor = color;
}

void BeginScene(const Camera& camera, const glm::mat4& transform)
{
    sData.CameraBuffer.ViewProjection = camera.GetProjection() * inverse(transform);
    sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    StartBatch();
}

void BeginScene(const Camera& camera)
{
    sData.CameraBuffer.ViewProjection = camera.GetViewProjection();
    sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    StartBatch();
}

void EndScene()
{
    Flush();
}

void Flush()
{
    if (sData.QuadIndexCount)
    {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(sData.QuadVertexBufferPtr) -
                                                  reinterpret_cast<uint8_t*>(sData.QuadVertexBufferBase));
        sData.QuadVertexBuffer->SetData(sData.QuadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
        {
            sData.TextureSlots[i]->Bind(i);
        }

        sData.QuadShader->Bind();
        RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.QuadIndexCount);
        sData.Stats.DrawCalls++;
    }

    if (sData.CircleVertexCount)
    {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(sData.CircleVertexBufferPtr) -
                                                  reinterpret_cast<uint8_t*>(sData.CircleVertexBufferBase));
        sData.CircleVertexBuffer->SetData(sData.CircleVertexBufferBase, dataSize);

        sData.CircleShader->Bind();
        RenderCommand::DrawCircle(sData.CircleVertexArray, sData.CircleVertexCount, RendererData::CircleVertices);
        sData.Stats.DrawCalls++;
    }

    if (sData.LineVertexCount)
    {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(sData.LineVertexBufferPtr) -
                                                  reinterpret_cast<uint8_t*>(sData.LineVertexBufferBase));
        sData.LineVertexBuffer->SetData(sData.LineVertexBufferBase, dataSize);

        sData.LineShader->Bind();
        RenderCommand::SetLineWidth(sData.LineWidth);
        RenderCommand::DrawLines(sData.LineVertexArray, sData.LineVertexCount);
        sData.Stats.DrawCalls++;
    }

    if (sData.TextIndexCount)
    {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(sData.TextVertexBufferPtr) -
                                                  reinterpret_cast<uint8_t*>(sData.TextVertexBufferBase));
        sData.TextVertexBuffer->SetData(sData.TextVertexBufferBase, dataSize);

        sData.FontAtlasTexture->Bind(0);

        sData.TextShader->Bind();
        RenderCommand::DrawIndexed(sData.TextVertexArray, sData.TextIndexCount);
        sData.Stats.DrawCalls++;
    }
}

/*
 * ===================================================================
 * -------------------------- Draw Rectangle -------------------------
 * ===================================================================
 */

void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawFilledRect({ position.x, position.y, 0.0f }, size, color);
}

void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawFilledRect(transform, color);
}

void DrawFilledRect(const glm::mat4& transform, const glm::vec4& color)
{
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (sData.QuadIndexCount >= RendererData::MaxQuadIndices)
    {
        NextBatch();
    }

    for (size_t i = 0; i < 4; i++)
    {
        sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
        sData.QuadVertexBufferPtr->Color = color;
        sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        sData.QuadVertexBufferPtr->TexIndex = 0.0f;
        sData.QuadVertexBufferPtr->TilingFactor = 1.0f;
        sData.QuadVertexBufferPtr++;
    }
    sData.QuadIndexCount += 6;

    sData.Stats.QuadCount++;
}

void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color)
{
    DrawRotatedFilledRect({ position.x, position.y, 0.0f }, size, radian, color);
}

void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), radian, { 0.0f, 0.0f, 1.0f }) *
                          scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawFilledRect(transform, color);
}

void DrawRotatedRect(const glm::vec2& position, const glm::vec2& size, float radian, const glm::vec4& color)
{
    DrawRotatedRect({ position.x, position.y, 0.0f }, size, radian, color);
}

void DrawRotatedRect(const glm::vec3& position, const glm::vec2& size, float radian, const glm::vec4& color)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), radian, { 0.0f, 0.0f, 1.0f }) *
                          scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawRect(transform, color);
}

void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawRect({ position.x, position.y, 0.0f }, size, color);
}

void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
}

void DrawRect(const glm::mat4& transform, const glm::vec4& color)
{
    glm::vec3 lineVertices[4];
    for (size_t i = 0; i < 4; i++)
    {
        lineVertices[i] = transform * sData.QuadVertexPositions[i];
    }
    DrawLine(lineVertices[0], lineVertices[1], color);
    DrawLine(lineVertices[1], lineVertices[2], color);
    DrawLine(lineVertices[2], lineVertices[3], color);
    DrawLine(lineVertices[3], lineVertices[0], color);
}

/*
 * ===================================================================
 * -------------------------- Draw Texture ---------------------------
 * ===================================================================
 */

void DrawTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor,
                 const glm::vec4& tintColor)
{
    DrawTexture({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}

void DrawTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor,
                 const glm::vec4& tintColor)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawTexture(transform, texture, tilingFactor, tintColor);
}

void DrawRotatedTexture(const glm::vec2& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                        float tilingFactor, const glm::vec4& tintColor)
{
    DrawRotatedTexture({ position.x, position.y, 0.0f }, size, radian, texture, tilingFactor, tintColor);
}

void DrawRotatedTexture(const glm::vec3& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                        float tilingFactor, const glm::vec4& tintColor)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), radian, { 0.0f, 0.0f, 1.0f }) *
                          scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawTexture(transform, texture, tilingFactor, tintColor);
}

void DrawTexture(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                 const glm::vec4& tintColor)
{
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (sData.QuadIndexCount >= RendererData::MaxQuad)
    {
        NextBatch();
    }

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < sData.TextureSlotIndex; i++)
    {
        if (*sData.TextureSlots[i] == *texture)
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (sData.TextureSlotIndex >= RendererData::MaxTextureSlots)
        {
            NextBatch();
        }
        textureIndex = static_cast<float>(sData.TextureSlotIndex);
        sData.TextureSlots[sData.TextureSlotIndex] = texture;
        sData.TextureSlotIndex++;
    }

    for (size_t i = 0; i < 4; i++)
    {
        sData.QuadVertexBufferPtr->Position = transform * sData.QuadVertexPositions[i];
        sData.QuadVertexBufferPtr->Color = tintColor;
        sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        sData.QuadVertexBufferPtr->TexIndex = textureIndex;
        sData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QuadVertexBufferPtr++;
    }
    sData.QuadIndexCount += 6;

    sData.Stats.QuadCount++;
}

/*
 * ===================================================================
 * -------------------------- Draw Circle ----------------------------
 * ===================================================================
 */

void DrawFilledCircle(const glm::vec2& position, float radius, const glm::vec4& color)
{
    DrawFilledCircle({ position.x, position.y, 0.0f }, radius, color);
}

void DrawFilledCircle(const glm::vec3& position, float radius, const glm::vec4& color)
{
    if (sData.CircleVertexCount >= RendererData::MaxCircleVertices)
    {
        NextBatch();
    }

    sData.CircleVertexBufferPtr->Position = position;
    sData.CircleVertexBufferPtr->Color = color;
    sData.CircleVertexBufferPtr++;
    float angleStep = 2.0f * Math::PI<float> / static_cast<float>(RendererData::CircleFragments);
    for (uint32_t i = 0; i <= RendererData::CircleFragments; i++)
    {
        sData.CircleVertexBufferPtr->Position =
            position + glm::vec3(radius * Math::Cos(static_cast<float>(i) * angleStep),
                                 radius * Math::Sin(static_cast<float>(i) * angleStep), 0.0f);
        sData.CircleVertexBufferPtr->Color = color;
        sData.CircleVertexBufferPtr++;
    }
    sData.CircleVertexCount += RendererData::CircleVertices;

    sData.Stats.QuadCount++;
}

void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color)
{
    DrawCircle({ position.x, position.y, 0.0f }, radius, color);
}

void DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color)
{
    float angleStep = 2.0f * Math::PI<float> / static_cast<float>(RendererData::CircleFragments);
    glm::vec3 p0 = position + glm::vec3(radius, 0.0f, 0.0f);
    for (uint32_t i = 1; i <= RendererData::CircleFragments; i++)
    {
        glm::vec3 p1 = position + glm::vec3(radius * Math::Cos(static_cast<float>(i) * angleStep),
                                            radius * Math::Sin(static_cast<float>(i) * angleStep), 0.0f);
        DrawLine(p0, p1, color);
        p0 = p1;
    }
}

/*
 * ===================================================================
 * --------------------------- Draw Line -----------------------------
 * ===================================================================
 */

float GetLineWidth()
{
    return sData.LineWidth;
}

void SetLineWidth(float width)
{
    // Flush line buffer if line width changes.
    NextBatch();

    sData.LineWidth = width;
    RenderCommand::SetLineWidth(width);
}

void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
{
    if (sData.LineVertexCount >= RendererData::MaxLineVertices)
    {
        NextBatch();
    }

    sData.LineVertexBufferPtr->Position = p0;
    sData.LineVertexBufferPtr->Color = color;
    sData.LineVertexBufferPtr++;

    sData.LineVertexBufferPtr->Position = p1;
    sData.LineVertexBufferPtr->Color = color;
    sData.LineVertexBufferPtr++;

    sData.LineVertexCount += 2;
}

/*
 * ===================================================================
 * --------------------------- Draw Text -----------------------------
 * ===================================================================
 */

void SetFontFamily(float fontSize, const Ref<Font>& font)
{
    sData.CurrentFontStyle.FontSize = fontSize;

    if (font)
    {
        NextBatch();
        sData.FontAtlasTexture = font->GetAtlasTexture();
        sData.FontData = font->GetMsdfData();
    }
}

bool InvertFont()
{
    return sData.InvertFont;
}

void InvertFont(bool invert)
{
    sData.InvertFont = invert;
}

void GetFontColor(glm::vec4* color)
{
    if (color)
    {
        *color = sData.CurrentFontStyle.Color;
    }
}

void SetFontColor(const glm::vec4& color, glm::vec4* old)
{
    GetFontColor(old);
    sData.CurrentFontStyle.Color = color;
}

void GetFontStyle(FontStyle* style)
{
    if (style)
    {
        *style = sData.CurrentFontStyle;
    }
}

void SetFontStyle(const FontStyle* style, FontStyle* old)
{
    GetFontStyle(old);
    if (style)
    {
        sData.CurrentFontStyle = *style;
    }
}

void DrawString(const std::string& string, const glm::vec2& position, StringAlign align)
{
    DrawString(string, { position.x, position.y, 0.0f }, align);
}

void DrawString(const std::string& string, const glm::vec3& position, StringAlign align)
{
    DrawString(string, position, sData.CurrentFontStyle.Color, align);
}

void DrawString(const std::string& string, const glm::vec2& position, const glm::vec4& color, StringAlign align)
{
    DrawString(string, { position.x, position.y, 0.0f }, color, align);
}

void DrawString(const std::string& string, const glm::vec3& position, const glm::vec4& color, StringAlign align)
{
    glm::mat4 transform =
        translate(glm::mat4(1.0f), position) *
        scale(glm::mat4(1.0f),
              { sData.CurrentFontStyle.FontSize,
                sData.InvertFont ? -sData.CurrentFontStyle.FontSize : sData.CurrentFontStyle.FontSize, 1.0f });
    DrawString(transform, string, color, align);
}

void DrawRotatedString(const std::string& string, const glm::vec2& position, float radian, StringAlign align)
{
    DrawRotatedString(string, { position.x, position.y, 0.0f }, radian, align);
}

void DrawRotatedString(const std::string& string, const glm::vec3& position, float radian, StringAlign align)
{
    DrawRotatedString(string, position, radian, sData.CurrentFontStyle.Color, align);
}

void DrawRotatedString(const std::string& string, const glm::vec2& position, float radian, const glm::vec4& color,
                       StringAlign align)
{
    DrawRotatedString(string, { position.x, position.y, 0.0f }, radian, color, align);
}

void DrawRotatedString(const std::string& string, const glm::vec3& position, float radian, const glm::vec4& color,
                       StringAlign align)
{
    glm::mat4 transform =
        translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), radian, { 0.0f, 0.0f, 1.0f }) *
        scale(glm::mat4(1.0f),
              { sData.CurrentFontStyle.FontSize,
                sData.InvertFont ? -sData.CurrentFontStyle.FontSize : sData.CurrentFontStyle.FontSize, 1.0f });
    DrawString(transform, string, color, align);
}

void DrawString(const glm::mat4& transform, const std::string& string, StringAlign align)
{
    DrawString(transform, string, sData.CurrentFontStyle.Color, align);
}

static void AlignTextHorizontal(TextVertex** begin, TextVertex* end, float lineWidth, StringAlign align);
static void AlignTextVertical(TextVertex* begin, TextVertex* end, float lineHeight, float textHeight,
                              StringAlign align);

void DrawString(const glm::mat4& transform, const std::string& string, const glm::vec4& color, StringAlign align)
{
    if (sData.TextIndexCount >= RendererData::MaxTextIndices)
    {
        NextBatch();
    }

    const auto& fontGeometry = sData.FontData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    const float spaceGlyphAdvance = static_cast<float>(fontGeometry.getGlyph(' ')->getAdvance());

    double x = 0.0;
    double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
    double y = 0.0;

    TextVertex* lineBegin = sData.TextVertexBufferPtr;
    TextVertex* textBegin = sData.TextVertexBufferPtr;
    for (size_t i = 0; i < string.size(); i++)
    {
        char character = string[i];
        if (character == '\r')
        {
            continue;
        }

        if (character == '\n')
        {
            AlignTextHorizontal(&lineBegin, sData.TextVertexBufferPtr, static_cast<float>(x), align);
            x = 0;
            y -= fsScale * metrics.lineHeight + sData.CurrentFontStyle.LineSpacing;
            continue;
        }

        if (character == ' ')
        {
            float advance = spaceGlyphAdvance;
            if (i < string.size() - 1)
            {
                char nextCharacter = string[i + 1];
                double dAdvance;
                fontGeometry.getAdvance(dAdvance, character, nextCharacter);
                advance = static_cast<float>(dAdvance);
            }
            x += fsScale * advance + sData.CurrentFontStyle.LetterSpacing;
            continue;
        }

        if (character == '\t')
        {
            // We assume tab width is 4 spaces.
            x += 4.0f * (fsScale * spaceGlyphAdvance + sData.CurrentFontStyle.LetterSpacing);
            continue;
        }

        auto glyph = fontGeometry.getGlyph(character);
        if (!glyph)
        {
            glyph = fontGeometry.getGlyph('?');
        }
        if (!glyph)
        {
            return;
        }

        double al, ab, ar, at;
        glyph->getQuadAtlasBounds(al, ab, ar, at);
        glm::vec2 texCoordMin(static_cast<float>(al), static_cast<float>(ab));
        glm::vec2 texCoordMax(static_cast<float>(ar), static_cast<float>(at));

        double pl, pb, pr, pt;
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);
        glm::vec2 quadMin(static_cast<float>(pl), static_cast<float>(pb));
        glm::vec2 quadMax(static_cast<float>(pr), static_cast<float>(pt));

        quadMin *= fsScale;
        quadMax *= fsScale;
        quadMin += glm::vec2(x, y);
        quadMax += glm::vec2(x, y);

        float texWidth = 1.0f / static_cast<float>(sData.FontAtlasTexture->GetWidth());
        float texHeight = 1.0f / static_cast<float>(sData.FontAtlasTexture->GetHeight());
        texCoordMin *= glm::vec2(texWidth, texHeight);
        texCoordMax *= glm::vec2(texWidth, texHeight);

        // render here
        sData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
        sData.TextVertexBufferPtr->Color = color;
        sData.TextVertexBufferPtr->TexCoord = texCoordMin;
        sData.TextVertexBufferPtr++;

        sData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
        sData.TextVertexBufferPtr->Color = color;
        sData.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
        sData.TextVertexBufferPtr++;

        sData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
        sData.TextVertexBufferPtr->Color = color;
        sData.TextVertexBufferPtr->TexCoord = texCoordMax;
        sData.TextVertexBufferPtr++;

        sData.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
        sData.TextVertexBufferPtr->Color = color;
        sData.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
        sData.TextVertexBufferPtr++;

        sData.TextIndexCount += 6;
        sData.Stats.QuadCount++;

        if (i < string.size() - 1)
        {
            double advance = glyph->getAdvance();
            char nextCharacter = string[i + 1];
            fontGeometry.getAdvance(advance, character, nextCharacter);
            x += fsScale * advance + sData.CurrentFontStyle.LetterSpacing;
        }
    }

    AlignTextHorizontal(&lineBegin, sData.TextVertexBufferPtr, static_cast<float>(x), align);
    AlignTextVertical(textBegin, sData.TextVertexBufferPtr, static_cast<float>(fsScale * metrics.lineHeight),
                      static_cast<float>(y), align);
}

void AlignTextHorizontal(TextVertex** begin, TextVertex* end, float lineWidth, StringAlign align)
{
    if (*begin == end)
    {
        return;
    }

    float translateX;
    if (align & SA_Center)
    {
        translateX = -lineWidth * 0.5f;
    }
    else if (align & SA_Right)
    {
        translateX = -lineWidth;
    }
    else
    {
        *begin = end;
        return;
    }
    translateX *= sData.CurrentFontStyle.FontSize;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { translateX, 0.0f, 0.0f });

    for (auto it = *begin; it != end; it++)
    {
        it->Position = transform * glm::vec4(it->Position, 1.0f);
    }

    *begin = end;
}

void AlignTextVertical(TextVertex* begin, TextVertex* end, float lineHeight, float textHeight, StringAlign align)
{
    if (begin == end)
    {
        return;
    }

    float translateY;
    if (align & SA_Bottom)
    {
        translateY = textHeight - lineHeight * 0.2f;
    }
    else if (align & SA_VCenter)
    {
        translateY = textHeight * 0.5f + lineHeight * 0.2f;
    }
    else
    {
        translateY = lineHeight * 0.8f;
    }
    translateY *= sData.CurrentFontStyle.FontSize;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f, translateY, 0.0f });

    for (auto it = begin; it != end; it++)
    {
        it->Position = transform * glm::vec4(it->Position, 1.0f);
    }
}

/*
 * ===================================================================
 * ---------------------------- Stats --------------------------------
 * ===================================================================
 */

void ResetStats()
{
    memset(&sData.Stats, 0, sizeof(Statistics));
}

Statistics GetStats()
{
    return sData.Stats;
}

void StartBatch()
{
    sData.QuadIndexCount = 0;
    sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;

    sData.CircleVertexCount = 0;
    sData.CircleVertexBufferPtr = sData.CircleVertexBufferBase;

    sData.LineVertexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.TextIndexCount = 0;
    sData.TextVertexBufferPtr = sData.TextVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void NextBatch()
{
    Flush();
    StartBatch();
}

} // namespace RenderApi

DGEX_END

#endif
