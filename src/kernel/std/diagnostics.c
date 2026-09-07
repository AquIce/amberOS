#include <std/kio.h>
#include <std/terminal.h>
#include <base/foundation/macros.h>

void foundation_log(
    const char* file,
    int line,
    const char* function,
    const char* fmt,
    ...
) {
    __builtin_va_list args;

	kterm_write("In file \"");
	kterm_write(file);
	kterm_write("\", in function \"");
	kterm_write(function);
	kterm_write("\": ");

	const char* arg = __builtin_va_arg(args, const char*);
	kterm_write(arg);
	kterm_endline();
}

_Noreturn void foundation_panic(
    const char* kind,
    const char* file,
    int line,
    const char* function,
    const char* expression,
    const char* fmt,
    ...
) {
	panic(fmt);
}
