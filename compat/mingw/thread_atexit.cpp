/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace
{

using Destructor = void (*)(void*);

struct DestructorNode
{
    Destructor      destructor;
    void*           object;
    DestructorNode* next;
};

INIT_ONCE fls_init_once = INIT_ONCE_STATIC_INIT;
DWORD     fls_index     = FLS_OUT_OF_INDEXES;

void CALLBACK run_thread_destructors(void* value)
{
    auto* node = static_cast<DestructorNode*>(value);

    while (node != nullptr)
    {
        auto* next = node->next;
        node->destructor(node->object);
        HeapFree(GetProcessHeap(), 0, node);
        node = next;
    }
}

BOOL CALLBACK initialize_fls(PINIT_ONCE, PVOID, PVOID*)
{
    fls_index = FlsAlloc(run_thread_destructors);
    return fls_index != FLS_OUT_OF_INDEXES;
}

} // namespace

extern "C" int __cxa_thread_atexit(Destructor destructor, void* object, void*)
{
    if (!InitOnceExecuteOnce(&fls_init_once, initialize_fls, nullptr, nullptr))
    {
        return -1;
    }

    auto* node =
        static_cast<DestructorNode*>(HeapAlloc(GetProcessHeap(), 0, sizeof(DestructorNode)));
    if (node == nullptr)
    {
        return -1;
    }

    node->destructor = destructor;
    node->object     = object;
    node->next       = static_cast<DestructorNode*>(FlsGetValue(fls_index));

    if (!FlsSetValue(fls_index, node))
    {
        HeapFree(GetProcessHeap(), 0, node);
        return -1;
    }

    return 0;
}
