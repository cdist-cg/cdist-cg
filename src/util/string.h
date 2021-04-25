#ifndef _CDIST_UTIL_STRING_H
#define _CDIST_UTIL_STRING_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define DIRSEP '/'

static char *_strjoin(char sep, /* const char * */...) {
	const char *s;
	char *dst, *p;
	va_list ap;
	size_t len, l;

	va_start(ap, sep);

	s = va_arg(ap, const char *);

	if (s) {
		/* Determine length of joined string */
		len = strlen(s) + 1;

		while ((s = va_arg(ap, const char *))) {
			len += strlen(s) + 1;
		}
		va_end(ap);

		/* Then, join strings */
		dst = calloc(len, sizeof(char));
		if (!dst) {
			return NULL;
		}
		p = dst;

		va_start(ap, sep);

		/* copy first element */
		s = va_arg(ap, const char *);
		l = strlen(s);
		strncpy(dst, s, l);
		p += l;

		/* then the others */
		while ((s = va_arg(ap, const char *))) {
			l = strlen(s);

			*p++ = sep;
			strncpy(p, s, l);
			p += l;
		}

		*p = '\0';
	} else {
		dst = strdup("");
	}

	va_end(ap);

	return dst;
}

/**
 * strjoin:
 * Joins a number of strings together to form one long string, with the
 * @separator inserted between each of them. The returned string
 * should be free()d.
 *
 * @param sep: a char to insert between each of the strings
 * @param ...: a list of strings to join
 * @return a newly-allocated string containing all of the strings joined
 */
#define strjoin(sep, ...) _strjoin(sep, __VA_ARGS__, NULL)

/**
 * pathjoin:
 * Joins a number of path components together to form one single path.
 * The returned string should be free()d.
 *
 * @param ...: a list of path components to join
 * @return a newly-allocated string containing all of the path components joined
 */
#define pathjoin(...) strjoin(DIRSEP, __VA_ARGS__)

#endif
