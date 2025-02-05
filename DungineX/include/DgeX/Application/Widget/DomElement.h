#pragma once

#include "DgeX/Application/Widget/DomNode.h"

#include "DgeX/Application/Event/MouseEvent.h"
#include "DgeX/Application/Input/MouseCode.h"
#include "DgeX/Core/Base.h"
#include "DgeX/Core/DeltaTime.h"

#include <tinyxml2.h>

#include <string>
#include <vector>

DGEX_BEGIN

enum class DomElementState : uint8_t
{
    Normal,
    Hover,
    Active,
    Focus,
    Disabled,
};

DomElementState DomElementStateFromString(const std::string& state);
std::string DomElementStateToString(DomElementState state);

/**
 * @brief Base class for all DOM elements.
 */
class DomElement : public DomNode, public std::enable_shared_from_this<DomElement>
{
    friend class DomContext;

public:
    DomElement(Ptr<DomContext> context, std::string tagName, std::string id = "");

    DomElement(const DomElement&) = delete;
    DomElement(DomElement&& other) noexcept = default;
    DomElement& operator=(const DomElement&) = delete;
    DomElement& operator=(DomElement&& other) = default;

    ~DomElement() override = default;

    Ptr<DomElement> AsElement() override;

public:
    /**
     * @brief Load properties from XML element.
     * @param element XML element.
     * @note
     * This is non-recursive, i.e. only load properties of the current element.
     */
    virtual void Load(const tinyxml2::XMLElement& element);

    void OnUpdate(DeltaTime delta) override;
    void OnEvent(const Ref<Event>& event) override;
    void OnRender() override;

    void ComputeProperties() override;
    void ComputeLayout() override;

    /*
     * ---------------------------------------------------------------
     *                      Structural properties
     * ---------------------------------------------------------------
     */
public:
    const std::string& TagName() const;
    const std::string& Id() const;

    /**
     * @brief A shortcut method to create a child element.
     */
    Ref<DomElement> CreateElement(const std::string& name, const std::string& id = "") const;

    /**
     * @brief Remove the child element.
     * @param element Child element to remove.
     * @note If the child element doesn't belong to this element, nothing happens.
     */
    void RemoveChild(const Ref<DomElement>& element);

    /**
     * @brief Insert to the end of the child elements.
     * @param element Child element to insert.
     * @note
     * If the child element already belongs to this element, it will be moved to back.
     * If the child element belongs to another element, it will be moved to this element.
     */
    void InsertEndChild(const Ref<DomElement>& element);

    using iterator = std::vector<Ref<DomElement>>::iterator;
    using const_iterator = std::vector<Ref<DomElement>>::const_iterator;

    const std::vector<Ref<DomElement>>& Children() const;
    size_t Size() const;
    Ref<DomElement> FirstChild() const;
    Ref<DomElement> LastChild() const;
    Ref<DomElement> PrevSibling() const;
    Ref<DomElement> NextSibling() const;

    // clang-format off
    iterator begin() { return _children.begin(); }
    iterator end() { return _children.end(); }
    const_iterator cbegin() const { return _children.cbegin(); }
    const_iterator cend() const { return _children.cend(); }

    // clang-format on

    Ref<DomElement> GetElementById(const std::string& id);
    std::vector<Ref<DomElement>> GetElementsByTagName(const std::string& tagName);
    std::vector<Ref<DomElement>> GetElementsByClassName(const std::string& className);

private:
    std::string _tagName;
    std::string _id;

    /*
     * ---------------------------------------------------------------
     *                        Style properties
     * ---------------------------------------------------------------
     */
public:
    void SetState(DomElementState state);
    DomElementState GetState() const;

    /**
     * @brief Add a class to the element.
     * @param className Name of the class.
     * @note If the class presents, nothing happens.
     */
    void AddClass(const std::string& className);

    /**
     * @brief Check if the element has the class.
     * @param className Name of the class.
     * @return True if the element has the class, false otherwise.
     */
    bool HasClass(const std::string& className) const;

    /**
     * @brief Remove a class from the element.
     * @param className Name of the class.
     * @note If the class doesn't present, nothing happens.
     */
    void RemoveClass(const std::string& className);

private:
    DomElementState _state;
    std::vector<std::string> _classList;

    /*
     * ---------------------------------------------------------------
     *                        Event properties
     * ---------------------------------------------------------------
     */
public:
    template <typename TEvent>
    void AddEventListener(const std::string& name, const EventListenerCallback<TEvent>& callback);
    void RemoveEventListener(const std::string& name);

private:
    std::unordered_map<std::string, Ref<EventListener>> _eventListeners;

private:
    /*
     * ---------------------------------------------------------------
     *                       Internal methods
     * ---------------------------------------------------------------
     */

    /**
     * @brief Check if the mouse position is inside the element.
     * @param position Mouse position.
     * @return Whether the position is inside the element or not.
     */
    bool _InRange(const MousePosition& position);

    bool _OnMouseMoved(Ptr<DomElement> element, const MouseMovedEvent& event);
    bool _OnMousePressed(Ptr<DomElement> element, const MouseButtonPressedEvent& event);
    bool _OnMouseReleased(Ptr<DomElement> element, const MouseButtonReleasedEvent& event);

    void _OnStateChange(DomElementState newState);

private:
    MouseCode _lastMouseCode = Mouse::ButtonLeft;
    MousePosition _lastMousePosition = { 0.0, 0.0 };
};

template <typename TEvent>
void DomElement::AddEventListener(const std::string& name, const EventListenerCallback<TEvent>& callback)
{
    _eventListeners[name] = CreateRef<EventListener>(this, callback);
}

DGEX_END
