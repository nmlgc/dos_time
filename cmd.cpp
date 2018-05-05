#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ticks.hpp"

#if defined(__TURBOC__) && defined(__MSDOS__)
// Remove C++ exception handler bloat on Borland compilers
// (see https://community.embarcadero.com/article/technical-articles/162/14700)
extern "C" void _ExceptInit(void)
{
}
#endif

void ticks_print_ms(const Ticks& t, const char *prefix)
{
	const long double freq = (65536.0L / (105.0L / 88.0L)) / 1000.0L;
	long double ms = (t.coarse * freq) + ((t.fine / 65536.0L) * freq);
	ticks_print_prefixed(t, prefix);
	printf(" (%Lf ms)", ms);
}

int should_show_opts(int argc, char **argv)
{
	const char *arg1 = argv[1];
	if(argc <= 1) {
		return 1;
	}
	return
		!strcmp(arg1, "-h")
		|| !strcmp(arg1, "--help")
		|| !strcmp(arg1, "/h")
		|| !strcmp(arg1, "/help");
}

int main(int argc, char **argv)
{
	char self_name[9];
	_splitpath(argv[0], NULL, NULL, self_name, NULL);

	if(should_show_opts(argc, argv)) {
		printf(
"Usage: %s [command ...]\n"
"Measures the amount of time taken for executing the given command, in both\n"
"hardware timer ticks and milliseconds.\n",
			self_name
		);
		return 1;
	}

	Ticks t1 = ticks_get();
	int ret = spawnvp(P_WAIT, argv[1], argv + 1);
	if(ret == -1) {
		printf("%s: Error starting ", self_name);
		perror(argv[1]);
		return errno;
	}

	Ticks t2 = ticks_get();
	Ticks diff = t2 - t1;
	ticks_print_ms(diff, "real");
	return ret;
}
