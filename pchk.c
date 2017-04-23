/*
 * Copyright (c) 2017 Jan Niemann <jan.niemann@beet5.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h> /* fprintf */
#include <stdlib.h> /* NULL */
#include <string.h> /* strlen */

#include <dirent.h> /* opendir, readdir_r, closedir */
#include <unistd.h> /* getopt */

extern int optind; /* getopt() */
extern int errno;

static int is_filename_portable(const char *, const char *);
static void recur(const char *pathname, int *retval);
static __dead void usage();

static const char * dirsep = "/";


static void
recur(const char *pathname, int *retval)
{
  int rv;
  DIR *dirhandle;
  struct dirent entry_storage;
  struct dirent *entry;
  char *subdir;

  if (NULL == (dirhandle = opendir(pathname))) {
    perror(pathname);
    exit(2);
  }

  for (;;) {
    if (0 != (rv = readdir_r(dirhandle, &entry_storage, &entry))) {
      /* perror uses errno, use rv instead? */
      perror(NULL);
      exit(2);
    }
    if (NULL == entry) {
      break;
    }
    if (!strncmp(".", entry->d_name, 1) || !strncmp(".." , entry->d_name, 2)) {
      continue;
    }
    switch (entry->d_type) {
    case DT_DIR:
      // printf("directory: %s%s%s\n", pathname, dirsep, entry->d_name);
      if (-1 == asprintf(&subdir, "%s%s%s", pathname, dirsep, entry->d_name)) {
	perror("malloc");
	exit(2);
      }
      recur(subdir, retval);
      (void) free(subdir);
      subdir = NULL;
      break;
    case DT_REG:
      // printf("regular file: %s%s%s\n", pathname, dirsep, entry->d_name);
      break;
    default:
      fprintf(stderr, "%s: file is neither regular nor directory\n", entry->d_name);
      *retval = 1;
    }

    if (is_filename_portable(pathname, entry->d_name)) {
      *retval = 1;
    }

  }

  (void) closedir(dirhandle);
}


static int
is_filename_portable(const char *pathname, const char *name)
{
  const char *c;
  int rv = 0;
  size_t name_len = strlen(name);

  /* POSIX portable filename: [A-Z][a-z].-_ */
  for (c = name; *c != '\0'; c++) {
    if (!((('A' <= *c) && (*c <= 'Z')) || (('a' <= *c) && (*c <= 'z')) || (('0' <= *c) && (*c <= '9')) ||
	  ('.' == *c) || ('_' == *c) || ('-' == *c))) {
      fprintf(stderr, "%s%s%s: not a posix portable pathname ([A-Z][a-z][0-9]-_.)\n", pathname, dirsep, name);
      rv = 1;
    }
  }

  /* names should not start with a hyphen (UNIX Tooling) */
  if (name[0] == '-') {
    fprintf(stderr, "%s%s%s: should not start with a hyphen (UNIX Tooling)\n", pathname, dirsep, name);
    rv = 1;
  }

  /* names should not end with a period (Windows) */
  if (name[name_len-1] == '.') {
    fprintf(stderr, "%s%s%s: should not end with a period (Windows)\n", pathname, dirsep, name);
    rv = 1;
  }

  /* CON, PRN, AUX, CLOCK$, NUL, COM[1-9], LPT[1-9] alone or followed by period (and extension...) */
  if (!strncasecmp("CON", name, 3) ||
      !strncasecmp("PRN", name, 3) ||
      !strncasecmp("AUX", name, 3) ||
      !strncasecmp("NUL", name, 3)) {
    if ((name[3] == '.') || (name[3] == '\0')) {
      fprintf(stderr, "%s%s%s: windows special filename (con, prn, aux, clock$, nul, com1-9, lpt1-9)\n", pathname, dirsep, name);
      rv = 1;
    }
  }
  if (!strncasecmp("CLOCK$", name, 6)) {
    if ((name[6] == '.') || (name[6] == '\0')) {
      fprintf(stderr, "%s%s%s: windows special filename (con, prn, aux, clock$, nul, com1-9, lpt1-9)\n", pathname, dirsep, name);
      rv = 1;
    }
  }
  if (!strncasecmp("COM", name, 3) ||
      !strncasecmp("LPT", name, 3)) {
    if ((('0' <= name[3]) && (name[3] <= '9')) && ((name[4] == '.') || (name[4] == '\0'))) {
      fprintf(stderr, "%s%s%s: windows special filename (con, prn, aux, clock$, nul, com1-9, lpt1-9)\n", pathname, dirsep, name);
      rv = 1;
    }
  }

  return rv;
}


static __dead void
usage()
{
  fputs("usage: pchk [-h] [dir ...]\n"
	"\n"
	"recursively checks pathnames for portability issues.\n"
	"\n"
	"returns 0 if pathnames are portable, 1 otherwise, 2 on failures.\n"
	"\n"
	"  -h   print this help\n", stderr);

  exit(1);
}


int
main(int argc, char **argv)
{
  int i;
  int ch;
  int retval = 0;

  /* parse command line */
  while (-1 != (ch = getopt(argc, argv, "h"))) {
    switch (ch) {
    case 'h':
      usage();
      break;
    }
  }
  argc -= optind;
  argv += optind;

  /* loop directories */
  for (i=0; i<argc; i++) {
    recur(argv[i], &retval);
  }

  return retval;
}
