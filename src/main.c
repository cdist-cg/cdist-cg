#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h> /* HACK: getopt() declaration */

#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#error no config.h
#endif

#include "config.h"

/* command declarations */

#define DECLARE_CDIST_CMD(name) \
	extern int cdist_##name##_main(int argc, char *argv[])

DECLARE_CDIST_CMD(banner);
DECLARE_CDIST_CMD(config);
DECLARE_CDIST_CMD(info);
DECLARE_CDIST_CMD(inventory);


/* types and static variables */

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

enum cdist_global_opt {
	PARAM_CONFIG_FILE
};

static const char *EXECNAME = PACKAGE;  /* default */

static const char *global_options[] = {
	[PARAM_CONFIG_FILE] = NULL
};

/* help */

void cdist_print_usage(FILE *outstream) {
	fprintf(outstream, "usage: %s ...\n", EXECNAME);
}

void cdist_print_version(FILE *outstream) {
	fprintf(outstream, "%s %s\n", PACKAGE, VERSION);
}

void cdist_print_help(FILE *outstream) {
	cdist_print_usage(outstream);
	fprintf(outstream, "\n");
	cdist_print_version(outstream);
	fprintf(outstream, "\n");
	fprintf(
		outstream,
		"foo\n");
}


/* argument parsing */

static int cdist_get_global_opts(
	int argc, char *argv[], const char *options[]) {
	/**
	 * Consume global cdist-cg options.
	 * Will update the `options` variable.
	 *
	 * Note that this function will begin parsing options from the first element
	 * of argv!
	 *
	 * @returns the number of consumed arguments
	 */

	extern char *optarg;
	extern int optind;
	extern int optopt;
	extern int opterr;
	extern int optreset;

	int c, error = 0;

	opterr = 0;
	optreset = 1;
	optind = 0;

	while ((c = getopt(argc, argv, ":g:hV")) != -1) {
		switch (c) {
		case 'g':
			options[PARAM_CONFIG_FILE] = optarg;
			break;
		case 'h':
			cdist_print_help(stdout);
			exit(EXIT_SUCCESS);
		case 'V':
			cdist_print_version(stdout);
			exit(EXIT_SUCCESS);
		case ':':
			fprintf(stderr, "%s: option requires an argument -- %c\n", EXECNAME, optopt);
			error = 1;
			break;
		case '?':
		default:
			fprintf(stderr, "%s: illegal option -- %c\n", EXECNAME, optopt);
			error = 1;
			break;
		}
	}

	if (error) {
		cdist_print_usage(stderr);
		exit(EXIT_FAILURE);
	}

	return optind;
}

static int cdist_get_cmd(int argc, char *argv[], enum cdist_cmd *cmd) {
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

	EXECNAME = argv[0];

	if (argc < 1) return 1;

#if DEBUG
	printf("Debugging enabled.\n");
#endif

	apos += cdist_get_global_opts((argc - apos), &argv[apos], global_options);

	if ((argc - apos) <= 0) {
		fprintf(stderr, "no command given.\n");
		return 1;
	}

	apos += cdist_get_cmd((argc - apos), &argv[apos], &command);

	switch (command) {
	case CMD_BANNER:
		return cdist_banner_main((argc - apos), &argv[apos]);
	case CMD_CONFIG:
		return cdist_config_main((argc - apos), &argv[apos]);
	case CMD_INSTALL:
		fprintf(stderr, "install command is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_INVENTORY:
		return cdist_inventory_main((argc - apos), &argv[apos]);
	case CMD_PREOS:
		fprintf(stderr, "preos is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_SHELL:
		fprintf(stderr, "cdist shell is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_INFO:
		return cdist_info_main((argc - apos), &argv[apos]);
	default:
		fprintf(stderr, "unknown command: %s\n", argv[apos]);
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
