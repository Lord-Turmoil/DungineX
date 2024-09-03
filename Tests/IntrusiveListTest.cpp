#include <DgeX/DgeX.h>

#include "doctest.h"

struct Node : DgeX::IntrusiveListNode
{
    int Value;
};

TEST_CASE("IntrusiveList Test")
{
    DgeX::IntrusiveList<Node> list;
    Node nodes[10];
    int i;

    for (i = 0; i < 10; i++)
    {
        nodes[i].Value = i;
        list.PushBack(&nodes[i]);
    }

    i = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        CHECK(it->Value == i);
        i++;
    }
}
