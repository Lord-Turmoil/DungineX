#pragma once

#include "DgeX/Core/Base.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

DGEX_BEGIN

/**
 * @brief Shader represents a shader program that can be used to render objects.
 */
class Shader
{
public:
    Shader() = default;
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept = delete;

    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetIntArray(const std::string& name, int* values, int count) = 0;
    virtual void SetFloat(const std::string& name, float value) = 0;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

    virtual const std::string& GetName() const = 0;

    /**
     * @brief Create a shader
     * @param filepath path of the shader program
     * @return the created shader
     */
    static Ref<Shader> Create(const std::string& filepath);

    /**
     * @brief Create a shader
     * @param name name of the shader
     * @param vertexShaderSource source of the vertex shader, nullptr to use default (no projection)
     * @param fragmentShaderSource source of the framgment shader, mustn't be nullptr
     * @return the created shader
     */
    static Ref<Shader> Create(const std::string& name, const char* vertexShaderSource,
                              const char* fragmentShaderSource);
};

/**
 * @brief A utility class to manage a set of shaders.
 */
class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);

    /**
     * @brief Add a shader and give it a new name.
     * @param name overriding the original name of the shader
     * @param shader the shader to add
     * @warning The original name of the shader won't be changed.
     */
    void Add(const std::string& name, const Ref<Shader>& shader);

    // shortcut for Create and Add
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> _shaders;
};

namespace ShaderTemplate
{

/**
 * @brief No modification to the input.
 */
extern const char* DEFAULT_VERTEX_SHADER;

/**
 * @brief Output magenta color.
 */
extern const char* DEFAULT_FRAGMENT_SHADER;

/**
 * @brief Accept vec4 color input and output it.
 */
extern const char* STANDARD_FRAGMENT_SHADER;

} // namespace ShaderTemplate

DGEX_END
