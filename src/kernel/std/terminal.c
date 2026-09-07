#include <std/kio.h>
#include <std/terminal.h>

#include <base/foundation/macros.h>
#include <base/foundation/memory/memory.h>

// --= Local Header =--

internal_fn u8 vga_color(VGAColor foreground, VGAColor background) {
    return ((u8)background << 4) | (u8)foreground;
}

internal_fn u16 vga_entry(char character, u8 color) {
    return ((u16)color << 8) | (u16)character;
}

internal_fn volatile u16* vga_cell(u16 row, u16 column) {
	return &VGA_MEMORY[row * TERMINAL_WIDTH + column];
}

typedef struct KTermLine KTermLine;

struct KTermLine {
	KTermLine* prev;
	KTermLine* next;
	u16 buffer[TERMINAL_WIDTH];
};

typedef struct {
    VGAColor foreground;
    VGAColor background;
	Cursor cursor;
	KTermLine* current_line;
} KTerm;

persistent KTerm terminal;

internal void kterm_line_setup(KTermLine* first_line, u16 buffer_count);

internal void kterm_render(void);

internal_fn KTermLine* nth_offset_line(KTermLine* base_line, usize offset) {
	while(offset--) {
		if(base_line == nullptr) {
			panic("Invalid nth offset");
		}
		base_line = base_line->next;
	}
	return base_line;
}

internal_fn u16* buffer_cell(u16 row, u16 column) {
	return &nth_offset_line(
		terminal.current_line, row
	)->buffer[column];
}

// --= Implementation =--


internal void kterm_line_setup(KTermLine* first_line, u16 buffer_count) {
	KTermLine* prev = nullptr;
	for(u16 i = 0; i < buffer_count; i++) {
		first_line->prev = prev;
		first_line->next = nullptr;
		if(prev) {
			prev->next = first_line;
		}
		prev = first_line;
		first_line++;
	}
}

internal void kterm_render(void) {
	for(u16 row = 0; row < TERMINAL_HEIGHT; row++) {
		for(u16 column = 0; column < TERMINAL_WIDTH; column++) {
			*vga_cell(row, column) = *buffer_cell(row, column);
		}
	}
}

void kterm_init(const MemorySource* memory_source) {

	u16 buffer_count = TERMINAL_HEIGHT * 2;
	KTermLine* first_line = MEMORY_SOURCE_RESERVE_NT(KTermLine, memory_source, buffer_count);
	if(!first_line) {
		panic("Fatal memory error while trying to allocate terminal lines buffer.");
	}
	kterm_line_setup(first_line, buffer_count);

	terminal = (KTerm){
		.foreground = VGA_COLOR_WHITE,
		.background = VGA_COLOR_BLACK,
		.cursor = (Cursor){
			.row = 0,
			.column = 0,
		},
		.current_line = first_line,
	};

	kterm_clear();
}

void kterm_putchar(char c) {

	if(c == '\n') {
        terminal.cursor.column = 0;
        terminal.cursor.row++;

        if(terminal.cursor.row >= TERMINAL_HEIGHT) {
			goto scroll_down_behavior;
        }

        goto render;
    }

	*buffer_cell(
		terminal.cursor.row, 
		terminal.cursor.column
	) = vga_entry(
        c,
        vga_color(terminal.foreground, terminal.background)
    );

    terminal.cursor.column++;

    if(terminal.cursor.column >= TERMINAL_WIDTH) {
        terminal.cursor.column = 0;
        terminal.cursor.row++;
    }

    if(terminal.cursor.row >= TERMINAL_HEIGHT) {
		goto scroll_down_behavior;
    }
	goto render;

	// WARN: DO NOT REMOVE THE LONE SEMICOLON PLEASE ._.
scroll_down_behavior:
	;
	KTermLine* last_line = nth_offset_line(terminal.current_line, TERMINAL_HEIGHT);
	if(last_line->next) {
		// TODO: Redraw screen
		terminal.current_line = terminal.current_line->next;
		terminal.cursor.row--;
		goto render;
	}
	panic("Welp");

render:
	kterm_render();
}
void kterm_write(const char* str) {
	while(*str) {
		kterm_putchar(*str++);
	}
}
void kterm_endline() {
	kterm_putchar('\n');
}

void kterm_write_dec(u32 value) {
	char buf[11];
    u32 i = 10;

    buf[i] = '\0';

    if(value == 0) {
        kterm_write("0");
        return;
    }

    while(value > 0) {
        buf[--i] = '0' + (value % 10);
        value /= 10;
    }

    kterm_write(&buf[i]);
}
void kterm_write_hex(u32 value) {
    static const char digits[] = "0123456789ABCDEF";
    char buf[11];

    buf[0] = '0';
    buf[1] = 'x';

    for(u32 i = 0; i < 8; i++) {
        buf[2 + i] = digits[(value >> (28 - i * 4)) & 0xF];
    }

    buf[10] = '\0';

    kterm_write(buf);
}
void kterm_write_hex64(u64 value) {
    static const char digits[] = "0123456789ABCDEF";
    char buf[19];

    buf[0] = '0';
    buf[1] = 'x';

    for(u32 i = 0; i < 16; i++) {
        buf[2 + i] = digits[(value >> (60 - i * 4)) & 0xF];
    }

    buf[18] = '\0';

    kterm_write(buf);
}

void kterm_clear(void) {
    u8 color = vga_color(
        terminal.foreground,
        terminal.background
    );

    for(u16 row = 0; row < TERMINAL_HEIGHT; row++) {
        for(u16 column = 0; column < TERMINAL_WIDTH; column++) {
            *vga_cell(row, column) = vga_entry(' ', color);
        }
    }

    terminal.cursor = (Cursor){
        .row = 0,
        .column = 0,
    };
}

void kterm_set_color(VGAColor foreground, VGAColor background) {
	terminal.foreground = foreground;
	terminal.background = background;
}

void kterm_set_cursor(Cursor cursor) {
	PRECONDITION(
        cursor.row < TERMINAL_HEIGHT && cursor.column < TERMINAL_WIDTH,
        "Invalid terminal cursor position: %u, %u",
        cursor.row,
		cursor.column
    );
	terminal.cursor = cursor;
}
void kterm_get_cursor(Cursor* cursor) {
	*cursor = terminal.cursor;
}
