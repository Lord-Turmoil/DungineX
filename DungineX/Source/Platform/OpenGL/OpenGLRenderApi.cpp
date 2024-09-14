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

    uint32_t TextCount = 0;
    TextVertex* TextVertexBufferBase = nullptr;
    TextVertex* TextVertexBufferPtr = nullptr;

    float LineWidth = 2.0f;

    std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

    Ref<Texture> FontAtlasTexture;

    glm::vec4 QuadVertexPositions[4];

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

    sData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);

    DGEX_CORE_INFO("Render API initialized");
}

void Shutdown()
{
    delete[] sData.QuadVertexBufferBase;
    sData.QuadVertexBufferBase = nullptr;
    delete[] sData.CircleVertexBufferBase;
    sData.CircleVertexBufferBase = nullptr;
    delete[] sData.LineVertexBufferBase;
    sData.LineVertexBufferBase = nullptr;
    delete[] sData.TextVertexBufferBase;
    sData.TextVertexBufferBase = nullptr;

    DGEX_CORE_INFO("Render API shutdown");
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

    if (sData.TextCount)
    {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(sData.TextVertexBufferPtr) -
                                                  reinterpret_cast<uint8_t*>(sData.TextVertexBufferBase));
        sData.TextVertexBuffer->SetData(sData.TextVertexBufferBase, dataSize);

        sData.FontAtlasTexture->Bind(0);

        sData.TextShader->Bind();
        RenderCommand::DrawIndexed(sData.TextVertexArray, sData.TextCount);
        sData.Stats.DrawCalls++;
    }
}

// Primitives
void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawFilledRect({ position.x, position.y, 0.0f }, size, color);
}

void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawFilledRect(transform, color);
}

void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor,
                    const glm::vec4& tintColor)
{
    DrawFilledRect({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}

void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor,
                    const glm::vec4& tintColor)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawFilledRect(transform, texture, tilingFactor, tintColor);
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

void DrawFilledRect(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
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

void DrawRotatedFilledRect(const glm::vec2& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                           float tilingFactor, const glm::vec4& tintColor)
{
    DrawRotatedFilledRect({ position.x, position.y, 0.0f }, size, radian, texture, tilingFactor, tintColor);
}

void DrawRotatedFilledRect(const glm::vec3& position, const glm::vec2& size, float radian, const Ref<Texture>& texture,
                           float tilingFactor, const glm::vec4& tintColor)
{
    glm::mat4 transform = translate(glm::mat4(1.0f), position) * rotate(glm::mat4(1.0f), radian, { 0.0f, 0.0f, 1.0f }) *
                          scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawFilledRect(transform, texture, tilingFactor, tintColor);
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

// static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src);
// static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform,
//                        const TextParams& textParams);
// static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component);

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

float GetLineWidth()
{
    return sData.LineWidth;
}

void SetLineWidth(float width)
{
    sData.LineWidth = width;
}

void ResetStats()
{
    memset(&sData.Stats, 0, sizeof(Statistics));
}

Statistics GetStats()
{
    return sData.Stats;
}

static void StartBatch()
{
    sData.QuadIndexCount = 0;
    sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;

    sData.CircleVertexCount = 0;
    sData.CircleVertexBufferPtr = sData.CircleVertexBufferBase;

    sData.LineVertexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.TextCount = 0;
    sData.TextVertexBufferPtr = sData.TextVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

static void NextBatch()
{
    Flush();
    StartBatch();
}

} // namespace RenderApi

DGEX_END

#endif
