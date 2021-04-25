#ifndef _CDIST_CONFIG_H
#define _CDIST_CONFIG_H

#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#error no config.h
#endif

#include <stdio.h>

#if HAVE_STDBOOL_H
#include <stdbool.h>
#else
#error missing stdbool.h
#endif

typedef char * command_t;
typedef char * path_t;

enum cdist_log_verb {
	LOG_OFF = 0,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_VERBOSE,
	LOG_DEBUG,
	LOG_TRACE
};

enum cdist_colour_output {
	CDIST_NOCOLOR = 0,
	CDIST_COLOUR_ALWAYS,
	CDIST_COLOUR_AUTO
};

enum cdist_archiving_mode {
	CDIST_ARCHIVING_NONE = 0,
	CDIST_ARCHIVING_TAR,
	CDIST_ARCHIVING_TGZ,
	CDIST_ARCHIVING_TBZ2,
	CDIST_ARCHIVING_TXZ
};

struct cdist_config_global {
	char *local_shell;
	char *remote_shell;
	char *cache_path_pattern;
	path_t conf_dir;
	enum cdist_colour_output coloured_output;
	char *init_manifest;
	char *out_path;
	command_t remote_out_path;
	command_t remote_copy;
	command_t remote_exec;
	path_t inventory_dir;
	unsigned short jobs;
	unsigned short parallel;
	enum cdist_log_verb verbosity_level;
	enum cdist_archiving_mode archiving;
	bool save_output_streams;
	bool log_timestamp;
};

struct cdist_config {
	struct cdist_config_global global;
};


/* Functions */

struct cdist_config *cdist_config_alloc();
void cdist_config_print(struct cdist_config *config, FILE *outstream);

const char *cdist_config_find();

int cdist_config_parse_file(struct cdist_config *config, FILE *fh);
int cdist_config_parse_env(struct cdist_config *config);
int cdist_config_set_defaults(struct cdist_config *config, struct cdist_config *defaults);
const char *cdist_config_get(struct cdist_config *config, const char *key);

#endif
