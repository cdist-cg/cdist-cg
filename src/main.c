#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#error no config.h
#endif

#include "config.h"

enum cdist_cmd {
	CMD_UNKNOWN,
	CMD_BANNER,
	CMD_CONFIG,
	CMD_INSTALL,
	CMD_INVENTORY,
	CMD_PREOS,
	CMD_SHELL,
	CMD_INFO,
	CMD_SCAN
};

/* command declarations */

extern void cmd_banner(FILE *outstream);


/* argument parsing */

static unsigned int get_global_opts(int argc, char *argv[]) {
	/**
	 * Consume global cdist-cg options.
	 *
	 * @returns the number of consumed arguments
	 */

	return 0;
}

static int get_cdist_cmd(int argc, char *argv[], enum cdist_cmd *cmd) {
	/**
	 * Get cdist command
	 *
	 * @returns int positive on success (the number of consumed arguments),
	 * negative on error.
	 *
	 *  0 no more arguments/invalid command
	 *  1 command consumed
	 */

	if (argc <= 0) return 0;

	if (!strcasecmp(argv[0], "banner")) {
		*cmd = CMD_BANNER;
	} else if (!strcasecmp(argv[0], "config")) {
		*cmd = CMD_CONFIG;
	} else if (!strcasecmp(argv[0], "install")) {
		*cmd = CMD_INSTALL;
	} else if (!strcasecmp(argv[0], "inventory")) {
		*cmd = CMD_INVENTORY;
	} else if (!strcasecmp(argv[0], "preos")) {
		*cmd = CMD_PREOS;
	} else if (!strcasecmp(argv[0], "shell")) {
		*cmd = CMD_SHELL;
	} else if (!strcasecmp(argv[0], "info")) {
		*cmd = CMD_INFO;
	} else if (!strcasecmp(argv[0], "scan")) {
		*cmd = CMD_SCAN;
	} else {
		/* Invalid command */
		return 0;
	}

	return 1;
}


int main(int argc, char *argv[]) {
	int apos = 1;
	enum cdist_cmd command = CMD_UNKNOWN;

	if (argc < 1) return 1;

#if DEBUG
	printf("Debugging enabled.\n");
#endif

	apos += get_global_opts((argc - apos), &argv[apos]);

	if ((argc - apos) <= 0) {
		fprintf(stderr, "no command given.\n");
		return 1;
	}

	apos += get_cdist_cmd((argc - apos), &argv[apos], &command);

	switch (command) {
	case CMD_BANNER:
		cmd_banner(stdout);
		break;
	case CMD_CONFIG:
		fprintf(stderr, "config command is not yet implemented.\n");
		return 1;
		break;
	case CMD_INSTALL:
		fprintf(stderr, "install command is not available in cdist-cg.\n");
		return 1;
		break;
	case CMD_INVENTORY:
		fprintf(stderr, "inventory command is not yet implemented..\n");
		return 1;
		break;
	case CMD_PREOS:
		fprintf(stderr, "preos is not available in cdist-cg.\n");
		return 1;
		break;
	case CMD_SHELL:
		fprintf(stderr, "cdist shell is not available in cdist-cg.\n");
		return 1;
		break;
	case CMD_INFO:
		fprintf(stderr, "info command is not yet implemented..\n");
		return 1;
		break;
	default:
		fprintf(stderr, "unknown command: %s\n", argv[apos]);
		return 1;
	}

	return 0;
}
