#pragma once

#include "DgeX/Core/Assert.h"

DGEX_BEGIN

/*
 * ===================================================================
 * ----------------------- IntrusiveListNode -------------------------
 * ===================================================================
 */

/**
 * @brief All elements in an intrusive list must inherit from this class.
 */
class IntrusiveListNode
{
    friend class IntrusiveListBase;

public:
    IntrusiveListNode() : _next(nullptr), _prev(nullptr)
    {
    }

    IntrusiveListNode(const IntrusiveListNode& other) = default;
    IntrusiveListNode& operator=(const IntrusiveListNode& other) = default;

    IntrusiveListNode(IntrusiveListNode&& other) noexcept
    {
        if (this != &other)
        {
            this->_next = other._next;
            this->_prev = other._prev;
            other._next = other._prev = nullptr;
        }
    }

    IntrusiveListNode& operator=(IntrusiveListNode&& other) noexcept
    {
        if (this != &other)
        {
            this->_next = other._next;
            this->_prev = other._prev;
            other._next = other._prev = nullptr;
        }
        return *this;
    }

    virtual ~IntrusiveListNode();

    /**
     * @return whether this node is in any list
     */
    bool IsLinked() const;

    // clang-format off
    IntrusiveListNode* Next() const { return _next; }
    IntrusiveListNode* Prev() const { return _prev; }

    /**
     * @warning This function doesn't check whether the node is in list or not.
     */
    bool IsHead() const { return _prev == nullptr; }

    /**
     * @warning This function doesn't check whether the node is in list or not.
     */
    bool IsTail() const { return _next == nullptr; }

    // clang-format on

private:
    /**
     * @brief Remove this node from the list. Do nothing if it's not in any list.
     */
    void _RemoveSelf();

private:
    IntrusiveListNode* _next;
    IntrusiveListNode* _prev;
};

inline IntrusiveListNode::~IntrusiveListNode()
{
    if (IsLinked())
    {
        _RemoveSelf();
    }
}

inline bool IntrusiveListNode::IsLinked() const
{
    // Because we add pseudo head and tail to the list, so the real nodes
    // must have both _prev and _next pointers.
    return (_prev != nullptr) && (_next != nullptr);
}

inline void IntrusiveListNode::_RemoveSelf()
{
    if (_next)
    {
        _next->_prev = _prev;
    }
    if (_prev)
    {
        _prev->_next = _next;
    }
    _next = _prev = nullptr;
}

/*
 * ===================================================================
 * ----------------------- IntrusiveListBase -------------------------
 * ===================================================================
 */

/**
 * @brief Base implementation of an intrusive list.
 */
class IntrusiveListBase
{
public:
    using node_type = IntrusiveListNode;
    using reference = node_type&;
    using pointer = node_type*;

    IntrusiveListBase()
    {
        _head._next = &_tail;
        _tail._prev = &_head;
    }

    virtual ~IntrusiveListBase() = 0;

    void Erase(reference node);
    void Erase(pointer node);

    void PopFront();
    void PopBack();

    void Clear();

    bool IsEmpty() const;

protected:
    void _InsertBefore(IntrusiveListNode& pos, IntrusiveListNode& node);
    void _InsertAfter(IntrusiveListNode& pos, IntrusiveListNode& node);

    IntrusiveListNode* _RealHead() const;
    IntrusiveListNode* _RealTail() const;

protected:
    IntrusiveListNode _head;
    IntrusiveListNode _tail;
};

inline IntrusiveListBase::~IntrusiveListBase() = default;

inline void IntrusiveListBase::_InsertBefore(IntrusiveListNode& pos, IntrusiveListNode& node)
{
    DGEX_ASSERT(!node.IsLinked(), "The node is already in a list.");

    node._next = &pos;
    node._prev = pos._prev;
    pos._prev->_next = &node;
    pos._prev = &node;
}

inline void IntrusiveListBase::_InsertAfter(IntrusiveListNode& pos, IntrusiveListNode& node)
{
    DGEX_ASSERT(!node.IsLinked(), "The node is already in a list.");

    node._prev = &pos;
    node._next = pos._next;
    pos._next->_prev = &node;
    pos._next = &node;
}

inline IntrusiveListNode* IntrusiveListBase::_RealHead() const
{
    return _head._next;
}

inline IntrusiveListNode* IntrusiveListBase::_RealTail() const
{
    return _tail._prev;
}

inline void IntrusiveListBase::PopFront()
{
    Erase(*_head._next);
}

inline void IntrusiveListBase::PopBack()
{
    Erase(*_tail._prev);
}

inline void IntrusiveListBase::Clear()
{
    while (!IsEmpty())
    {
        PopFront();
    }
}

inline bool IntrusiveListBase::IsEmpty() const
{
    return _head._next == &_tail;
}

inline void IntrusiveListBase::Erase(reference node)
{
    DGEX_ASSERT(node.IsLinked(), "The node is not valid or in any list.");
    node._RemoveSelf();
}

inline void IntrusiveListBase::Erase(pointer node)
{
    DGEX_ASSERT(node, "Cannot erase nullptr.");
    Erase(*node);
}

/*
 * ===================================================================
 * --------------------- IntrusiveList Iterator ----------------------
 * ===================================================================
 */

template <typename _Ty, bool _Const> class IntrusiveListIterator
{
public:
    // using conditional_t to avoid two classes
    using value_type = std::conditional_t<_Const, const _Ty, _Ty>;
    using reference = value_type&;
    using pointer = value_type*;

    using node_type = std::conditional_t<_Const, const IntrusiveListNode, IntrusiveListNode>;
    using node_pointer = node_type*;

    explicit IntrusiveListIterator(node_pointer node) : _node(node)
    {
    }

    reference operator*() const
    {
        return *static_cast<pointer>(_node);
    }

    pointer operator->() const
    {
        return static_cast<pointer>(_node);
    }

    IntrusiveListIterator& operator++()
    {
        DGEX_ASSERT(_node, "Out of range.");
        _node = _node->Next();
        return *this;
    }

    IntrusiveListIterator operator++(int)
    {
        IntrusiveListIterator temp = *this;
        ++(*this);
        return temp;
    }

    IntrusiveListIterator& operator--()
    {
        DGEX_ASSERT(_node, "Out of range.");
        _node = _node->Prev();
        return *this;
    }

    IntrusiveListIterator operator--(int)
    {
        IntrusiveListIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const IntrusiveListIterator& other) const
    {
        return _node == other._node;
    }

    bool operator!=(const IntrusiveListIterator& other) const
    {
        return !(this->operator==(other));
    }

private:
    node_pointer _node;
};

/*
 * ===================================================================
 * ------------------------- IntrusiveList ---------------------------
 * ===================================================================
 */

/**
 * @brief Intrusive list wrapper.
 * @tparam _Ty element type
 */
template <typename _Ty> class IntrusiveList final : public IntrusiveListBase
{
public:
    using value_type = _Ty;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = IntrusiveListIterator<_Ty, false>;
    using const_iterator = IntrusiveListIterator<_Ty, true>;

public:
    IntrusiveList() = default;
    ~IntrusiveList() override = default;

    void PushFront(reference node);
    void PushFront(pointer node);
    void PushBack(reference node);
    void PushBack(pointer node);

    reference Front();
    const_reference Front() const;
    reference Back();
    const_reference Back() const;

public:
    // clang-format off
    iterator begin() { return iterator(_RealHead()); }
    iterator end() { return iterator(&_tail); }
    const_iterator begin() const { return const_iterator(_RealHead()); }
    const_iterator end() const { return const_iterator(&_tail); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    // clang-format on
};

template <typename _Ty> void IntrusiveList<_Ty>::PushFront(reference node)
{
    _InsertAfter(_head, node);
}

template <typename _Ty> void IntrusiveList<_Ty>::PushFront(pointer node)
{
    DGEX_ASSERT(node, "Cannot insert nullptr");
    PushFront(*node);
}

template <typename _Ty> void IntrusiveList<_Ty>::PushBack(reference node)
{
    _InsertBefore(_tail, node);
}

template <typename _Ty> void IntrusiveList<_Ty>::PushBack(pointer node)
{
    DGEX_ASSERT(node, "Cannot insert nullptr");
    PushBack(*node);
}

template <typename _Ty> typename IntrusiveList<_Ty>::reference IntrusiveList<_Ty>::Front()
{
    DGEX_ASSERT(!IsEmpty(), "The list is empty.");
    return *static_cast<pointer>(_RealHead());
}

template <typename _Ty> typename IntrusiveList<_Ty>::const_reference IntrusiveList<_Ty>::Front() const
{
    DGEX_ASSERT(!IsEmpty(), "The list is empty.");
    return *static_cast<const_pointer>(_RealHead());
}

template <typename _Ty> typename IntrusiveList<_Ty>::reference IntrusiveList<_Ty>::Back()
{
    DGEX_ASSERT(!IsEmpty(), "The list is empty.");
    return *static_cast<pointer>(_RealTail());
}

template <typename _Ty> typename IntrusiveList<_Ty>::const_reference IntrusiveList<_Ty>::Back() const
{
    DGEX_ASSERT(!IsEmpty(), "The list is empty.");
    return *static_cast<const_pointer>(_RealTail());
}

DGEX_END
