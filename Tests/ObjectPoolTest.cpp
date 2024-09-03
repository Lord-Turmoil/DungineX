#include <DgeX/DgeX.h>

#include "doctest.h"

struct Object : DgeX::PooledObject<Object>
{
    int Value;
};

TEST_CASE("ObjectPool Test")
{
    constexpr int POOL_SIZE = 10;

    DgeX::ObjectPool<Object> pool(POOL_SIZE);
    DgeX::ObjectQueue<Object> queue;

    CHECK_EQ(pool.IsFull(), false);

    for (int i = 0; i < POOL_SIZE; i++)
    {
        queue.Add(pool.Acquire());
    }
    CHECK_EQ(queue.Size(), POOL_SIZE);

    CHECK_EQ(pool.TryAcquire(), nullptr);

    queue.Lock();
    for (auto& it : queue)
    {
        CHECK_EQ(it->InUse(), true);
        it->Dispose();
        queue.Remove(it);
    }
    queue.Unlock();

    CHECK_EQ(queue.Size(), 0);

    CHECK_NE(pool.TryAcquire(), nullptr);
}
