#include <DgeX/DgeX.h>

#include "doctestplus.h"

struct Object : DgeX::PooledObject<Object>
{
    int Value;
};

TEST_CASE("ObjectPool Test")
{
    constexpr int POOL_SIZE = 10;

    DgeX::ObjectPool<Object> pool(POOL_SIZE);
    DgeX::ObjectQueue<Object> queue;

    CHECK_FALSE(pool.IsFull());

    for (int i = 0; i < POOL_SIZE; i++)
    {
        queue.Add(pool.Acquire());
    }
    CHECK_EQ(queue.Size(), POOL_SIZE);

    CHECK_NULL(pool.TryAcquire());

    queue.Lock();
    for (auto& it : queue)
    {
        CHECK(it->InUse());
        it->Dispose();
        queue.Remove(it);
    }
    queue.Unlock();

    CHECK_EQ(queue.Size(), 0);
    CHECK_NON_NULL(pool.TryAcquire());
}
