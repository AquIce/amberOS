#ifndef __AMBER_STD_KMEM__
#define __AMBER_STD_KMEM__

#include <base/foundation/memory/memory.h>

#define E820_BUFFER     0x4000
#define E820_COUNT_ADDR 0x3FFE

#define MEMORY_USABLE 1

void memory_init(MemorySource* memory_source);

#endif // __AMBER_STD_KMEM__
