#include "DgeX/Renderer/Shader.h"

DGEX_BEGIN

class OpenGLShader final : Shader
{
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const char* vertexShaderSource, const char* fragmentShaderSource);

    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void SetInt(const std::string& name, int value) override;
    void SetIntArray(const std::string& name, int* values, int count) override;
    void SetFloat(const std::string& name, float value) override;
    void SetFloat2(const std::string& name, const glm::vec2& value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    const std::string& GetName() const override
    {
        return _name;
    }

private:
    void _Init(const std::string& name, const char* vertexShaderSource, const char* fragmentShaderSource);
    static std::string _ParseShaderFile(const std::string& filepath, std::string& vertexSource,
                                        std::string& fragmentSource);
    static bool _CheckCompileErrors(uint32_t shader, const std::string& type);

    uint32_t _programId = 0;
    std::string _name;
};

DGEX_END
