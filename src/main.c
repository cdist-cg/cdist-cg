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
	int *argc, char ***argv, const char *options[]) {
	/**
	 * Consume global cdist-cg options.
	 * Will update the `options` variable.
	 *
	 * Note that this function will begin parsing options from the first element
	 * of argv!
	 *
	 * This function updates argc and argv.
	 *
	 * @returns 0 on success
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

	while ((c = getopt(*argc, *argv, ":g:hV")) != -1) {
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

	*argc -= optind;
	*argv += optind;

	return 0;
}

static int cdist_get_cmd(int *argc, char ***argv, enum cdist_cmd *cmd) {
	/**
	 * Get cdist-cg command.
	 *
	 * This function updates argc and argv.
	 *
	 * @returns 0 on success.
	 *          1 no command.
	 *          2 invalid command.
	 */
	const char *_cmd = NULL;

	if (*argc <= 0) return 1;

	_cmd = *argv[0];

	++*argv;
	--*argc;

	if (!strcasecmp(_cmd, "banner")) {
		*cmd = CMD_BANNER;
	} else if (!strcasecmp(_cmd, "config")) {
		*cmd = CMD_CONFIG;
	} else if (!strcasecmp(_cmd, "install")) {
		*cmd = CMD_INSTALL;
	} else if (!strcasecmp(_cmd, "inventory")) {
		*cmd = CMD_INVENTORY;
	} else if (!strcasecmp(_cmd, "preos")) {
		*cmd = CMD_PREOS;
	} else if (!strcasecmp(_cmd, "shell")) {
		*cmd = CMD_SHELL;
	} else if (!strcasecmp(_cmd, "info")) {
		*cmd = CMD_INFO;
	} else if (!strcasecmp(_cmd, "scan")) {
		*cmd = CMD_SCAN;
	} else {
		/* Invalid command */
		return 2;
	}

	return 0;
}


int main(int argc, char *argv[]) {
	enum cdist_cmd command = CMD_UNKNOWN;

	EXECNAME = argv[0];

	if (argc < 1) return 1;

#if DEBUG
	printf("Debugging enabled.\n");
#endif

	char **argrestv = &argv[1];
	int argrestc = (argc - 1);

	if (cdist_get_global_opts(&argrestc, &argrestv, global_options)) {
		/* error in global options */
		return EXIT_FAILURE;
	}

	switch (cdist_get_cmd(&argrestc, &argrestv, &command)) {
	case 0:
		/* ok */
		break;
	case 1:
		fprintf(stderr, "%s: no command given\n", EXECNAME);
		return EXIT_FAILURE;
	case 2:
		fprintf(stderr, "%s: invalid command: %s\n", EXECNAME, argrestv[-1]);
		return EXIT_FAILURE;
	default:
		return EXIT_FAILURE;
	}

	/* NOTE: Handling of command line options is now passed to the commands.
	 *       Unlike before, commands do not advance the *rest* pointers. */
	switch (command) {
	case CMD_BANNER:
		return cdist_banner_main(argrestc, argrestv);
	case CMD_CONFIG:
		return cdist_config_main(argrestc, argrestv);
	case CMD_INSTALL:
		fprintf(stderr, "install command is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_INVENTORY:
		return cdist_inventory_main(argrestc, argrestv);
	case CMD_PREOS:
		fprintf(stderr, "preos is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_SHELL:
		fprintf(stderr, "cdist shell is not available in cdist-cg.\n");
		return EXIT_FAILURE;
	case CMD_INFO:
		return cdist_info_main(argrestc, argrestv);
	default:
		fprintf(stderr, "command not implemented: %s\n", argrestv[-1]);
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
