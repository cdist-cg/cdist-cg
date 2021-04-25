#include "config.h"
#include "util/string.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * cdist_config_alloc:
 * Allocates a new cdist_config struct filled with defaults.
 *
 * @return pointer to a new cdist_config struct
 *         (or NULL if it ran out of memory.)
 */
struct cdist_config *cdist_config_alloc() {
	struct cdist_config *new = malloc(sizeof(struct cdist_config));

	if (new != NULL) {
		*new = (struct cdist_config) {
			.global = {
				.local_shell = NULL
			}
		};
	}

	return new;
}


/**
 * cdist_config_print:
 * Prints a cdist_config struct.
 *
 * @param config: the cdist_config struct to print
 * @param outstream: the stream to print to
 */
void cdist_config_print(struct cdist_config *config, FILE *outstream) {
	fprintf(
		outstream,
		"cdist_config {\n"
		"  global {\n"
		"    local_shell = %s\n"
		"    remote_shell = %s\n"
		"    cache_path_pattern = %s\n"
		"    conf_dir = %s\n"
		"    coloured_output = %u\n"
		"    init_manifest = %s\n"
		"    out_path = %s\n"
		"    remote_out_path = %s\n"
		"    remote_copy = %s\n"
		"    remote_exec = %s\n"
		"    inventory_dir = %s\n"
		"    jobs = %u\n"
		"    parallel = %u\n"
		"    verbosity_level = %u\n"
		"    archiving = %u\n"
		"    save_output_streams = %s\n"
		"    log_timestamp = %s\n"
		"  }\n"
		"}\n",

		config->global.local_shell,
		config->global.remote_shell,
		config->global.cache_path_pattern,
		config->global.conf_dir,
		config->global.coloured_output,
		config->global.init_manifest,
		config->global.out_path,
		config->global.remote_out_path,
		config->global.remote_copy,
		config->global.remote_exec,
		config->global.inventory_dir,
		config->global.jobs,
		config->global.parallel,
		config->global.verbosity_level,
		config->global.archiving,
		config->global.save_output_streams ? "true" : "false",
		config->global.log_timestamp ? "true" : "false"
		);
}


/**
 * cdist_config_find:
 * Finds the cdist.cfg file to use.
 *
 * The caller must free() the returned pointer.
 *
 * @return path to the cdist.cfg file to use,
 *         or NULL if none was found or an error occurred.
 */
const char *cdist_config_find() {

	static const char *conffilename = "cdist.cfg";
	char *confdir, *conffile;

	/* first, try user config file */

	if ((confdir = getenv("XDG_CONFIG_HOME"))) {
		conffile = pathjoin(confdir, conffilename);
	} else if ((confdir = getenv("HOME"))) {
		conffile = pathjoin(confdir, ".config", conffilename);
	} else {
		goto global;
	}

	printf("checking %s\n", conffile);
	if (!access(conffile, R_OK)) {
		return conffile;
	}

	free(conffile);


  global:
	/* then, try global config file */

#ifdef SYSCONFDIR
	confdir = SYSCONFDIR;
#else
	/* fallback */
	confdir = str(DIRSEP) "etc";
#endif

	conffile = pathjoin(confdir, conffilename);

	if (!access(conffile, R_OK)) {
		return conffile;
	}

	free(conffile);

	/* no config found */
	return NULL;
}

/******************************************************************************/
/* Reading config */

#define _str_conf(s) s
/* #define _path_conf(s) */

inline enum cdist_colour_output _colour_conf(const char *in) {
	if (!strcmp(in, "always")) {
		return CDIST_COLOUR_ALWAYS;
	} else if (!strcmp(in, "never")) {
		return CDIST_NOCOLOR;
	} else if (!strcmp(in, "auto")) {
		return CDIST_COLOUR_AUTO;
	} else {
		/* ??? */
	}
}

int cdist_config_parse_file(struct cdist_config *config, FILE *fh) {
	char buf[BUFSIZ];

	(void)fread(buf, sizeof(char), BUFSIZ, fh);

	return 1;
}


#define _cdist_config_parse_env_map(config, envvar, tf_func, confopt) \
	{ \
		char *envval = getenv(envvar); \
		if (envval) { \
			config->confopt = tf_func(envval); \
		} \
	}

/**
 * cdist_config_parse_env:
 * ...
 *
 * @param config
 * @return
 */
int cdist_config_parse_env(struct cdist_config *config) {

	/* _cdist_config_parse_env_map(config, "CDIST_PATH", _path_conf, global.conf_dir); */
	_cdist_config_parse_env_map(config, "CDIST_LOCAL_SHELL", _str_conf, global.local_shell);
	_cdist_config_parse_env_map(config, "CDIST_REMOTE_SHELL", _str_conf, global.remote_shell);
	_cdist_config_parse_env_map(config, "CDIST_REMOTE_EXEC", _str_conf, global.remote_exec);
	_cdist_config_parse_env_map(config, "CDIST_REMOTE_COPY", _str_conf, global.remote_copy);
	/* _cdist_config_parse_env_map(config, "CDIST_INVENTORY_DIR", _dir_conf, global.inventory_dir); */
	_cdist_config_parse_env_map(config, "CDIST_CACHE_PATH_PATTERN", _str_conf, global.cache_path_pattern);
	_cdist_config_parse_env_map(config, "CDIST_COLORED_OUTPUT", _colour_conf, global.coloured_output);
	/* _cdist_config_parse_env_map(config, "__cdist_log_level", _loglvl_conf, "verbosity"); */

	return 0;
}
