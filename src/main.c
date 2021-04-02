#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#error no config.h
#endif

#include "config.h"

int main(int argc, char *argv[]) {
#if DEBUG
	printf("Debugging enabled.\n");
#endif
	return 0;
}
