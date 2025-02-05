#pragma once

#include "DgeX/Application/Widget/DomElement.h"

#include <string>
#include <vector>

DGEX_BEGIN

class BaseSelectorPattern
{
public:
    BaseSelectorPattern() = default;
    virtual ~BaseSelectorPattern() = default;

    virtual bool Match(const DomElement& element) const = 0;
    virtual std::string ToString() const = 0;
};

class TagSelectorPattern : public BaseSelectorPattern
{
public:
    TagSelectorPattern(std::string tag, DomElementState state);
    ~TagSelectorPattern() override = default;

    bool Match(const DomElement& element) const override;
    std::string ToString() const override;

private:
    std::string _tag;
    DomElementState _state;
};

class IdSelectorPattern : public BaseSelectorPattern
{
public:
    IdSelectorPattern(std::string id, DomElementState state);
    ~IdSelectorPattern() override = default;

    bool Match(const DomElement& element) const override;
    std::string ToString() const override;

private:
    std::string _id;
    DomElementState _state;
};

class ClassSelectorPattern : public BaseSelectorPattern
{
public:
    ClassSelectorPattern(std::string className, DomElementState state);
    ~ClassSelectorPattern() override = default;

    bool Match(const DomElement& element) const override;
    std::string ToString() const override;

private:
    std::string _className;
    DomElementState _state;
};

/**
 * @brief Selector pattern for one element.
 * @note Only match when all patterns match.
 */
class SelectorPattern : public BaseSelectorPattern
{
public:
    SelectorPattern() = default;
    ~SelectorPattern() override = default;

    bool Match(const DomElement& element) const override;
    std::string ToString() const override;

    void AddPattern(const Ref<BaseSelectorPattern>& pattern);

private:
    std::vector<Ref<BaseSelectorPattern>> _patterns;
};

/**
 * @brief Selector class to match elements.
 */
class Selector
{
public:
    Selector() = default;
    Selector(const char* value);

    /**
     * @brief Parse selector from raw string.
     * @param value Raw selector string.
     * @note This will clear previous patterns.
     */
    void Assign(const char* value);

    bool Match(const DomElement& element) const;
    std::string ToString() const;

private:
    Ref<BaseSelectorPattern> _ParseSelectorPattern(const char* begin, const char* end);
    Ref<BaseSelectorPattern> _ParseSingleSelectorPattern(const char* begin, const char* end);

private:
    std::vector<Ref<BaseSelectorPattern>> _patterns;
};

DGEX_END
