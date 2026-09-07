#include <std/kio.h>
#include <std/kmem.h>
#include <std/terminal.h>

#include <base/foundation/macros.h>
#include <base/foundation/memory/memory.h>
#include <base/foundation/memory/sources/buffer.h>

typedef struct {
    u64 base;
    u64 length;
    u32 type;
    u32 attributes;
} E820_Entry;

typedef struct {
	u64 base;
	u64 length;
} MemoryRegion;

typedef struct {
    MemoryRegion *regions;
    usize count;
} PhysicalMemory;

void memory_init(MemorySource* memory_source) {
	E820_Entry* mmap = (E820_Entry*)E820_BUFFER;

    for(u16 i = 0; i < *(const u16*)E820_COUNT_ADDR; i++) {
        E820_Entry* entry = &mmap[i];

        if(entry->type != MEMORY_USABLE) {
            continue;
		}
		// NOTE: We ignore the 0th entry to start memory at 1MB
		if(entry->base == 0) {
			continue;
		}

		kterm_write("Entry #");
		kterm_write_dec(i);
		kterm_endline();
		kterm_write(" base:   ");
		kterm_write_hex64(entry->base);
		kterm_endline();

		kterm_write(" length: ");
		kterm_write_hex64(entry->length);
		kterm_write(" (");
		kterm_write_dec(entry->length / MB(1));
		kterm_write(" MB)");
		kterm_endline();

		kterm_write(" end:    ");
		kterm_write_hex64(entry->base + entry->length);
		kterm_endline();

		*memory_source = buffer_memory_source_create(
			(void*)(uptr)entry->base,
			entry->length - 2 * sizeof(BufferAllocCtx),
			(BufferAllocCtx*)align_down_ptr(
				(uptr)entry->base + entry->length,
				alignof(BufferAllocCtx)
			)
		);

		// panic("Kernel Panic Test");
	}
}
