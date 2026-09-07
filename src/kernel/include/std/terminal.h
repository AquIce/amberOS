#ifndef __AMBER_KERNEL_TERM__
#define __AMBER_KERNEL_TERM__

#include <std/ktypes.h>

#include <base/foundation/macros.h>
#include <base/foundation/memory/memory.h>

#define VGA_MEMORY ((volatile u16*)0xB8000)

#define TERMINAL_WIDTH  80
#define TERMINAL_HEIGHT 25

/* VGA colors */
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} VGAColor;

typedef struct {
	u16 row;
	u16 column;
} Cursor;

void kterm_init(const MemorySource* memory_source);

void kterm_putchar(char c);
void kterm_write(const char* str);
void kterm_endline();

void kterm_write_dec(u32 value);
void kterm_write_hex(u32 value);
void kterm_write_hex64(u64 value);

void kterm_clear(void);

void kterm_set_color(VGAColor foreground, VGAColor background);

void kterm_set_cursor(Cursor cursor);
void kterm_get_cursor(Cursor* cursor);

#endif // __AMBER_KERNEL_TERM__
