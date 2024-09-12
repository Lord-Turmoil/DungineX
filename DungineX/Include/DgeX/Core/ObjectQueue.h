#pragma once

#include "DgeX/dgexpch.h"

DGEX_BEGIN

/**
 * @brief
 * Object queue is a queue that stores current active objects. It helps to
 * manage the adding and removing of objects, and provides a way to iterate
 * through all objects.
 *
 * @note
 * It can be used with ObjectPool to manage the acquired objects in the pool.
 */
template <typename _Ty> class ObjectQueue
{
    using object_type = _Ty;

public:
    ObjectQueue() = default;

    /**
     * @brief Lock the queue to prevent modification during iteration.
     * @note This should be called before iterating through the objects.
     * @warning Don't recursively lock the queue.
     */
    void Lock();

    /**
     * @brief Unlock the queue and process the pending objects.
     * @note The unlock action will process the pending objects.
     * @warning Don't recursively unlock the queue.
     */
    void Unlock();

    void Add(object_type* object);

    // Return whether the object is successfully removed.
    bool Remove(object_type* object);

    void Clear();

    // clang-format off
    typename std::vector<object_type*>::iterator begin() { return _objects.begin(); }
    typename std::vector<object_type*>::iterator end() { return _objects.end(); }
    typename std::vector<object_type*>::const_iterator begin() const { return _objects.begin(); }
    typename std::vector<object_type*>::const_iterator end() const { return _objects.end(); }

    // clang-format on

private:
    void _ProcessPending();

    bool _Remove(object_type* object);
    void _Remove(const std::vector<object_type*>& removing);
    bool _Remove(std::vector<object_type*>& objects, object_type* object);

private:
    std::vector<object_type*> _objects;
    std::vector<object_type*> _pending;
    std::vector<object_type*> _removing;

    /**
     * @brief
     * If the queue is locked, no object can be added or removed to prevent
     * modification during iteration. New objects will be added to the pending
     * and removed objects will be added to the removing list.
     */
    bool _isLocked = false;
};

template <typename _Ty> void ObjectQueue<_Ty>::Lock()
{
    _isLocked = true;
}

template <typename _Ty> void ObjectQueue<_Ty>::Unlock()
{
    _isLocked = false;
    _ProcessPending();
}

template <typename _Ty> void ObjectQueue<_Ty>::Add(object_type* object)
{
    if (_isLocked)
    {
        _pending.push_back(object);
    }
    else
    {
        _objects.push_back(object);
    }
}

template <typename _Ty> bool ObjectQueue<_Ty>::Remove(object_type* object)
{
    if (_isLocked)
    {
        _removing.push_back(object);
        return false;
    }
    return _Remove(_objects, object);
}

template <typename _Ty> void ObjectQueue<_Ty>::Clear()
{
    _objects.clear();
    _pending.clear();
    _removing.clear();
}

template <typename _Ty> void ObjectQueue<_Ty>::_ProcessPending()
{
    for (object_type* object : _pending)
    {
        _objects.push_back(object);
    }
    _pending.clear();

    _Remove(_removing);
    _removing.clear();
}

template <typename _Ty> bool ObjectQueue<_Ty>::_Remove(object_type* object)
{
    if (!_Remove(_objects, object))
    {
        return _Remove(_pending, object);
    }
    return true;
}

template <typename _Ty> void ObjectQueue<_Ty>::_Remove(const std::vector<object_type*>& removing)
{
    for (object_type* object : removing)
    {
        _Remove(object);
    }
}

template <typename _Ty> bool ObjectQueue<_Ty>::_Remove(std::vector<object_type*>& objects, object_type* object)
{
    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end())
    {
        std::swap(*it, objects.back());
        objects.pop_back();
        return true;
    }
    return false;
}

DGEX_END
