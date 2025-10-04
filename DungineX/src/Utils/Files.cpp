/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Files.cpp                                 *
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

#include "DgeX/Utils/Files.h"

#include "DgeX/Utils/Assert.h"

DGEX_BEGIN

namespace Files
{

void OutputHandle::WriteLine(const char* str)
{
    Write(str);
    Write("\n");
}

OutputHandle& DgeX::Files::OutputHandle::operator<<(const char* str)
{
    Write(str);
    return *this;
}

FileHandle::FileHandle(std::ofstream file) : _file(std::move(file))
{
    DGEX_ASSERT(_file.is_open(), "File stream must be open");
}

FileHandle::~FileHandle()
{
    FileHandle::Close();
}

bool FileHandle::IsOpen() const
{
    return _file.is_open();
}

void FileHandle::Close()
{
    if (_file.is_open())
    {
        _file.close();
    }
}

void FileHandle::Write(const char* str)
{
    _file << str;
}

bool BufferHandle::IsOpen() const
{
    return true;
}

void BufferHandle::Close()
{
    // Do nothing.
}

void BufferHandle::Write(const char* str)
{
    _buffer << str;
}

std::string BufferHandle::GetBuffer() const
{
    return _buffer.str();
}

Ref<FileHandle> OpenFile(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        DGEX_CORE_ERROR("Failed to open file: {}", filename);
    }
    return CreateRef<FileHandle>(std::move(file));
}

Ref<BufferHandle> OpenBuffer()
{
    return CreateRef<BufferHandle>();
}

} // namespace Files

DGEX_END
