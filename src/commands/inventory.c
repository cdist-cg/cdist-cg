#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h> /* HACK: getopt() declaration */

enum cdist_inventory_subcmd {
	INV_UNKNOWN_SUBCMD,
	INV_CMD_ADD_HOST,
	INV_CMD_ADD_TAG,
	INV_CMD_DEL_HOST,
	INV_CMD_DEL_TAG,
	INV_CMD_LIST
};

enum cdist_inventory_opt {
	INVENTORY_DIR
};

static const char *inv_options[] = {
	[INVENTORY_DIR] = NULL
};


char *cdist_find_inventory_dir(void) {
	/**
	 * Finds the cdist inventory dir.
	 *
	 * The order of inventory dir setting by decreasing priority
	 * 1. inventory_dir from configuration
	 * 2. ~/.cdist/inventory if HOME env var is set
	 * 3. distribution inventory directory
	 *
	 * @returns the path if found, NULL otherwise.
	 */

    /* inventory_dir_set = False
     * if 'inventory_dir' in configuration:
     *     val = configuration['inventory_dir']
     *     return val
     * home = cdist.home_dir()
     * if home:
     *     return os.path.join(home, DIST_INVENTORY_DB_NAME)
     * else:
     *     return dist_inventory_db */
	return NULL;
}

static void cdist_inv_print_help(FILE *outstream) {

}

static int cdist_get_inventory_opts(
	int argc, char *argv[], const char *options[]) {
	/**
	 * Consume options to the inventory command.
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

	while ((c = getopt(argc, argv, ":I:h")) != -1) {
		switch (c) {
		case 'I':
			options[INVENTORY_DIR] = optarg;
			break;
		case 'h':
			cdist_inv_print_help(stdout);
			exit(EXIT_SUCCESS);
		case ':':
			fprintf(stderr, "- inventory: option requires an argument -- %c\n", optopt);
			error = 1;
			break;
		case '?':
		default:
			fprintf(stderr, "- inventory: illegal option -- %c\n", optopt);
			error = 1;
			break;
		}
	}

	if (error) {
		/* XXX: Should probably not exit() here */
		exit(EXIT_FAILURE);
	}

	return optind;
}

static int cdist_get_inventory_subcmd(
	int argc, char *argv[], enum cdist_inventory_subcmd *cmd) {
	/**
	 * Get inventory sub command
	 *
	 * @returns int positive on success (the number of consumed arguments),
	 * negative on error.
	 *
	 *  0 no more arguments/invalid command
	 *  1 command consumed
	 */

	if (argc <= 0) return 0;

	if (!strcasecmp(argv[0], "add-host")) {
		*cmd = INV_CMD_ADD_HOST;
	} else if (!strcasecmp(argv[0], "add-tag")) {
		*cmd = INV_CMD_ADD_TAG;
	} else if (!strcasecmp(argv[0], "del-host")) {
		*cmd = INV_CMD_DEL_HOST;
	} else if (!strcasecmp(argv[0], "del-tag")) {
		*cmd = INV_CMD_DEL_TAG;
	} else if (!strcasecmp(argv[0], "list")) {
		*cmd = INV_CMD_LIST;
	} else {
		/* Invalid command */
		return 0;
	}

	return 1;
}

int cdist_inventory_main(int argc, char *argv[]) {
	int apos = 0;
	enum cdist_inventory_subcmd subcmd = INV_UNKNOWN_SUBCMD;

	apos += cdist_get_inventory_opts(argc, argv, inv_options);
	apos += cdist_get_inventory_subcmd((argc-apos), &argv[apos], &subcmd);

	printf("%u\n", subcmd);
	return 1;
}
