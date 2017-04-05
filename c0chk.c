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

#include <unistd.h>  /* getopt() */

#include <libgen.h>  /* basename() */
#include <stdio.h>   /* fopen(), getc(), ferror(), feof(), fclose() */
#include <stdlib.h>  /* exit() */
#include <string.h>  /* strcmp() */

/* local prototypes */
static void usage();
static char *xlat(int);

/* for getopt() */
extern int optind;

/* see the GNU make manual, OpenBSD make only uses "makefile" and "Makefile" */
const char *makefile_names[] = {
  "GNUmakefile", "makefile", "Makefile"
};


static void
usage()
{
  fputs("usage: c0chk [-chm] [file ...]\n"
"\n"
"checks files for c0 control chars except line feed '\\n'.\n"
"\n"
"returns 0 if no offending chars were found, 1 otherwise, 2 on failures.\n"
"\n"
"  -c   continue inspecting, print all occurences\n"
"  -h   print this help\n"
"  -m   allow tabs in Makefiles\n", stderr);
  exit(1);
}

static char *
xlat(int ch)
{
  switch (ch) {
  case 0: return "Null (NUL)";
  case 1: return "Start of Heading (SOH)";
  case 2: return "Start of Text (STX)";
  case 3: return "End of Text (ETX)";
  case 4: return "End of Transmission (EOT)";
  case 5: return "Enquiry (ENQ)";
  case 6: return "Acknowledge (ACK)";
  case 7: return "Bell (BEL)";
  case 8: return "Backspace (BS)";
  case 9: return "Horizontal Tabulation (HT)";
  case 10: return "Line Feed (LF)";
  case 11: return "Vertical Tabulation (VT)";
  case 12: return "Form Feed (FF)";
  case 13: return "Carriage Return (CR)";
  case 14: return "Shift Out (SO)";
  case 15: return "Shift In (SI)";
  case 16: return "Data Link Escape (DLE)";
  case 17: return "Device Control 1 (DC1)";
  case 18: return "Device Control 2 (DC2)";
  case 19: return "Device Control 3 (DC3)";
  case 20: return "Device Control 4 (DC4)";
  case 21: return "Negative Acknowledge (NAK)";
  case 22: return "Synchronous Idle(SYN)";
  case 23: return "End of Transmission Block (ETB)";
  case 24: return "Cancel (CAN)";
  case 25: return "End of Medium (EM)";
  case 26: return "Substitute (SUB)";
  case 27: return "Escape (ESC)";
  case 28: return "File Separator (FS)";
  case 29: return "Group Separator (GS)";
  case 30: return "Record Separator (RS)";
  case 31: return "Unit Separator (US)";
  case 127: return "Delete (DEL)";
  default:
    return "not a c0 control";
  }
}

int
main(int argc, char **argv)
{
  int i; /* iterating argv */
  size_t j; /* iterating by sizeof() */
  FILE *fh;
  int ch;
  int cflag = 0; /* if set, keep running and output all c0 occurences of '\r' */
  int rv = 0;    /* only used if cflag, for recording return value */
  int line = 1;  /* only used if cflag, for recording line number */
  int pos = 0;   /* only used if cflag, for recording position */
  int mflag = 0; /* do not fail on Makefiles (GNUMakefile, Makefile) for tabs on first position */
  int is_makefile = 0;
  char *file_basename = NULL;

  /* parse command line */
  while (-1 != (ch = getopt(argc, argv, "chm"))) {
    switch (ch) {
    case 'c':
      cflag = 1;
      break;
    case 'h':
      usage();
      /* NOTREACHED */
    case 'm':
      mflag = 1;
      break;
    default:
      usage();
    }
  }
  argc -= optind;
  argv += optind;

  /* loop over files */
  for (i=0; i<argc; i++) {

    if (NULL == (fh = fopen(argv[i], "r"))) {
      perror(argv[i]);
      return 2;
    }

    /* check if file is a makefile (if mflag is set) */
    is_makefile = 0;
    if (mflag) {
      if (NULL == (file_basename = basename(argv[i]))) {
	perror(argv[i]);
	return 2;
      }
      for (j=0; j<sizeof(makefile_names)/sizeof(makefile_names[0]); j++) {
	if (!strcmp(file_basename, makefile_names[j])) {
	  is_makefile = 1;
	  break;
	}
      }
    }

    while (EOF != (ch = getc(fh))) {
      if ((ch == '\t') && (is_makefile) && (pos == 0)) {
	pos=7;
	continue;
      }
      if (ch == '\n') {
	line++;
	pos=0;
        continue;
      }
      pos++;
      if ((ch <= 31) || (ch == 127)) {
        if (cflag == 0) {
          return 1;
        } else {
          rv = 1;
          printf("%s:%i:%i: 0x%0.2x %s\n", argv[i], line, pos, ch, xlat(ch));
        }
      }
    }

    if (ferror(fh)) {
      perror("getc");
      return 2;
    }

    if (!feof(fh)) {
      fputs("something strange happened here...\n", stderr);
    }

    (void) fclose(fh);
  }

  return 0;
}
