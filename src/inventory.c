#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum cdist_inventory_opt {
	INVENTORY_DIR
};

static const char *options[] = {
	[INVENTORY_DIR] = NULL
};


char *find_inventory_dir(void) {
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

static void print_help(FILE *outstream) {

}

static int get_inventory_opts(int argc, char *argv[], const char *options[]) {
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
			print_help(stdout);
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

int cmd_inventory(int argc, char *argv[]) {
	(void)get_inventory_opts(argc, argv, options);
	return 1;
}
