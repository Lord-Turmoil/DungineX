#pragma once

#include "DgeX/Core/Macros.h"

DGEX_BEGIN

/*
 * This object pool uses a ring buffer to store all objects, the layout
 * is as follows.
 * ___________________________________________________
 * | | | | |X| |X|X|X| | |X|X| |X| | |X|X| | |X|X|X| |
 * -----^---------------------------------------------
 *      Next
 * When an object is acquired, the head pointer will move to the next available
 * slot and return the address of the object, and the object will be marked as
 * in use. When the object is released, the object will be marked as not int use.
 */

/**
 * @brief
 * Represents an object in the object pool. Such object has a flag to indicate
 * whether it is in use or not.
 *
 * @note
 * The caller who get an object from the pool should initialize them afterward,
 * as the pool has no idea about the object's initialization.
 *
 * @note
 * After using the object, the caller should explicitly call Dispose() to
 * release the object back to the pool.
 *
 * @warning
 * All objects inherit from this class should have a default constructor.
 */
template <typename _Ty> class PooledObject
{
    template <class T> friend class ObjectPool;

public:
    using object_type = _Ty;

    /**
     * @brief Dispose the object and release it back to the pool.
     * @note
     * This is directly called by the client, and is the only way to return
     * a pooled object.
     */
    void Dispose();

    /**
     * @brief Return the state of the pooled object.
     * @return whether the object is in use
     */
    bool InUse() const
    {
        return _inUse;
    }

private:
    /**
     * @brief Mark this object in use before returned to client.
     * @note This can only be invoked by the pool.
     */
    void _Activate();

    /**
     * @brief Mark this object not in use after returned to pool.
     * @note This can only be invoked by the pool.
     */
    void _Deactivate();

    /**
     * @brief Bind this object to a pool.
     * @param pool the pool to bind
     * @note This is invoked once when the pool initializes the object buffer.
     */
    void _Bind(ObjectPool<object_type>* pool);

private:
    bool _inUse = false;
    ObjectPool<object_type>* _pool = nullptr;
};

template <typename _Ty> class ObjectPool
{
    friend class PooledObject<_Ty>;

public:
    using object_type = _Ty;

    /**
     * @note The size is fixed for the basic object pool.
     */
    ObjectPool(size_t size);
    virtual ~ObjectPool();

    /**
     * @brief Acquire an object from the pool.
     * @return an object in the pool
     * @warning: This method will assert if the pool is full, then return nullptr.
     * @note To dynamically grow the pool, use DynamicObjectPool instead.
     */
    virtual object_type* Acquire();

    /**
     * @brief Try to acquire an object from the pool.
     * @return an object in the pool, nullptr if is full
     */
    object_type* TryAcquire();

    /**
     * @brief Check the availability of the object pool.
     * @return whether the object pool is full or not
     */
    bool IsFull() const
    {
        return _allocated == _size;
    }

protected:
    /**
     * @brief Release an object back to the pool.
     * @param object the object to release
     * @note
     * Here I used friend class to limit the access to this method, so that
     * only the PooledObject's Dispose() method can call this method.
     */
    void _Release(object_type* object);

    size_t _size;
    size_t _allocated;

    object_type* _base;
    object_type* _head; // points to the next available object
    object_type* _tail; // points to the end of the pool (base + size)
};

/*
 * =============================================================================
 * ----------------------------- PooledObject ----------------------------------
 * =============================================================================
 */

template <typename _Ty> void PooledObject<_Ty>::Dispose()
{
    DGEX_ASSERT(_pool, "Not allocated from any pool.");
    if (_inUse)
    {
        static_cast<ObjectPool<object_type>*>(_pool)->_Release(static_cast<object_type*>(this));
    }
}

template <typename _Ty> void PooledObject<_Ty>::_Activate()
{
    DGEX_ASSERT(!_inUse, "Already in use.");
    _inUse = true;
}

template <typename _Ty> void PooledObject<_Ty>::_Deactivate()
{
    DGEX_ASSERT(_inUse, "Already in idle.");
    _inUse = false;
}

template <typename _Ty> void PooledObject<_Ty>::_Bind(ObjectPool<object_type>* pool)
{
    DGEX_ASSERT(!_pool, "Already bound to a pool.");
    _pool = pool;
}

/*
 * =============================================================================
 * ------------------------------ ObjectPool -----------------------------------
 * =============================================================================
 */

template <typename _Ty> ObjectPool<_Ty>::ObjectPool(size_t size) : _size(size), _allocated(0)
{
    DGEX_ASSERT(size > 0, "Size of the pool must be greater than 0.");

    _base = new object_type[size];
    _head = _base;
    _tail = _base + size;
    for (auto it = _head; it != _tail; ++it)
    {
        static_cast<PooledObject<object_type>*>(it)->_Bind(this);
    }
}

template <typename _Ty> ObjectPool<_Ty>::~ObjectPool()
{
    delete[] _base;
}

template <typename _Ty> typename ObjectPool<_Ty>::object_type* ObjectPool<_Ty>::Acquire()
{
    if (IsFull())
    {
        DGEX_ASSERT(false, "No available object in the pool!");
        return nullptr;
    }

    while (_head->InUse())
    {
        ++_head;
        if (_head == _tail)
        {
            _head = _base;
        }
    }
    object_type* object = _head;
    static_cast<PooledObject<object_type>*>(object)->_Activate();
    _allocated++;

    return object;
}

template <typename _Ty> typename ObjectPool<_Ty>::object_type* ObjectPool<_Ty>::TryAcquire()
{
    return IsFull() ? nullptr : Acquire();
}

template <typename _Ty> void ObjectPool<_Ty>::_Release(object_type* object)
{
    DGEX_ASSERT(object >= _base && object < _base + _size, "Invalid object to release.");

    static_cast<PooledObject<object_type>*>(object)->_Deactivate();

    _allocated--;
}

DGEX_END
