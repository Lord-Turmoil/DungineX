/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Files.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : August 24, 2025                           *
 *                                                                            *
 *                    Last Update : August 24, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * File operation utilities.                                                  *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "Types.h"

#include <fstream>
#include <sstream>
#include <string>

DGEX_BEGIN

namespace Files
{

/**
 * @brief Auto-closable output handle.
 */
class OutputHandle
{
public:
    OutputHandle() = default;
    OutputHandle(const OutputHandle& other) = delete;
    OutputHandle(OutputHandle&& other) noexcept = delete;
    OutputHandle& operator=(const OutputHandle& other) = delete;
    OutputHandle& operator=(OutputHandle&& other) noexcept = delete;

    virtual ~OutputHandle() = default;

    /**
     * @brief Check if the handle is open.
     *
     * @return Whether the handle is open.
     */
    virtual bool IsOpen() const = 0;

    /**
     * @brief Close the handle.
     *
     * After closing, the handle cannot be used again.
     */
    virtual void Close() = 0;

    /**
     * @brief Write a string to the handle.
     *
     * @param str The string to write.
     */
    virtual void Write(const char* str) = 0;

    /**
     * @brief Write a string to the handle, followed by a newline.
     *
     * @param str The string to write.
     */
    virtual void WriteLine(const char* str);

    /**
     * @brief Write a string to the handle using stream operator.
     *
     * @param str The string to write.
     * @return Reference to this handle.
     */
    OutputHandle& operator<<(const char* str);
};

class FileHandle : public OutputHandle
{
public:
    explicit FileHandle(std::ofstream file);
    ~FileHandle() override;

    bool IsOpen() const override;
    void Close() override;

    void Write(const char* str) override;

private:
    std::ofstream _file;
};

class BufferHandle : public OutputHandle
{
public:
    explicit BufferHandle() = default;
    ~BufferHandle() override = default;

    bool IsOpen() const override;
    void Close() override;

    void Write(const char* str) override;

    std::string GetBuffer() const;

private:
    std::stringstream _buffer;
};

Ref<FileHandle> OpenFile(const std::string& filename);
Ref<BufferHandle> OpenBuffer();

} // namespace Files

DGEX_END
