#include "DgeX/Application/Style/Selector.h"

#include "DgeX/Application/Widget/DomElement.h"
#include "DgeX/Core/Assert.h"
#include "DgeX/Utils/String.h"

#include <sstream>

DGEX_BEGIN

TagSelectorPattern::TagSelectorPattern(std::string tag, DomElementState state) : _tag(std::move(tag)), _state(state)
{
}

bool TagSelectorPattern::Match(const DomElement& element) const
{
    return (element.TagName() == _tag) && (element.GetState() == _state);
}

std::string TagSelectorPattern::ToString() const
{
    if (_state == DomElementState::Normal)
    {
        return _tag;
    }
    return _tag + ":" + DomElementStateToString(_state);
}

IdSelectorPattern::IdSelectorPattern(std::string id, DomElementState state) : _id(std::move(id)), _state(state)
{
}

bool IdSelectorPattern::Match(const DomElement& element) const
{
    return (element.Id() == _id) && (element.GetState() == _state);
}

std::string IdSelectorPattern::ToString() const
{
    if (_state == DomElementState::Normal)
    {
        return "#" + _id;
    }
    return "#" + _id + ":" + DomElementStateToString(_state);
}

ClassSelectorPattern::ClassSelectorPattern(std::string className, DomElementState state)
    : _className(std::move(className)), _state(state)
{
}

bool ClassSelectorPattern::Match(const DomElement& element) const
{
    return (element.GetState() == _state) && element.HasClass(_className);
}

std::string ClassSelectorPattern::ToString() const
{
    if (_state == DomElementState::Normal)
    {
        return "." + _className;
    }
    return "." + _className + ":" + DomElementStateToString(_state);
}

bool SelectorPattern::Match(const DomElement& element) const
{
    for (auto& pattern : _patterns)
    {
        if (!pattern->Match(element))
        {
            return false;
        }
    }
    return true;
}

std::string SelectorPattern::ToString() const
{
    std::stringstream ss;

    for (auto& pattern : _patterns)
    {
        ss << pattern->ToString();
    }

    return ss.str();
}

void SelectorPattern::AddPattern(const Ref<BaseSelectorPattern>& pattern)
{
    _patterns.push_back(pattern);
}

Selector::Selector(const char* value)
{
    Assign(value);
}

void Selector::Assign(const char* value)
{
    _patterns.clear();

    const char* left = value;
    const char* right = value;
    bool found = false;
    while (*left)
    {
        if (found)
        {
            if ((*right == ' ') || (*right == '\0'))
            {
                _patterns.push_back(_ParseSelectorPattern(left, right));
                found = false;
                left = right;
            }
            else
            {
                right++;
            }
        }
        else
        {
            if ((*left == ' ') || (*left == ':'))
            {
                left++;
            }
            else
            {
                found = true;
                right = left + 1;
            }
        }
    }
}

bool Selector::Match(const DomElement& element) const
{
    if (_patterns.empty())
    {
        return false;
    }

    auto it = _patterns.rbegin();

    auto target = &element;
    while (target)
    {
        if ((*it)->Match(*target))
        {
            ++it;
            if (it == _patterns.rend())
            {
                return true;
            }
        }
        target = target->GetParentElement();
    }

    return false;
}

std::string Selector::ToString() const
{
    std::stringstream ss;
    bool first = true;

    for (auto& pattern : _patterns)
    {
        if (first)
        {
            ss << pattern->ToString();
            first = false;
        }
        else
        {
            ss << " " << pattern->ToString();
        }
    }

    return ss.str();
}

Ref<BaseSelectorPattern> Selector::_ParseSelectorPattern(const char* begin, const char* end)
{
    DGEX_ASSERT(begin < end, "Selector pattern can't be empty");

    Ref<SelectorPattern> pattern = CreateRef<SelectorPattern>();

    const char* left = begin;
    const char* right = begin + 1;
    while (left != end)
    {
        if ((right == end) || (*right == '.'))
        {
            pattern->AddPattern(_ParseSingleSelectorPattern(left, right));
            left = right;
        }
        right++;
    }

    return pattern;
}

Ref<BaseSelectorPattern> Selector::_ParseSingleSelectorPattern(const char* begin, const char* end)
{
    DomElementState state = DomElementState::Normal;

    if (const char* middle = Utils::String::Find(begin, end, ':'))
    {
        state = DomElementStateFromString(std::string(middle + 1, end));
        end = middle;
    }

    if (*begin == '.')
    {
        return CreateRef<ClassSelectorPattern>(std::string(begin + 1, end), state);
    }
    if (*begin == '#')
    {
        return CreateRef<IdSelectorPattern>(std::string(begin + 1, end), state);
    }
    return CreateRef<TagSelectorPattern>(std::string(begin, end), state);
}

DGEX_END
