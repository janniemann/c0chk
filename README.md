# c0chk / pchk

c0chk checks files for c0 control chars and more.

pchk checks filenames for portability.

both tested on and compile on macOS

## c0chk 

c0chk checks files for c0 control chars (except \n and \t) and DEL
asserts that the last line ends with a newline symbol
has multiple modes for tabs

### usage

    usage: c0chk [-achmnTt] [file ...]

    checks files for c0 control chars except line feed '\n'.

    returns 0 if no offending chars were found, 1 otherwise, 2 on failures.

      -a   ascii, do not allow 8 bit characters (iso, utf-8)
      -c   continue inspecting, print all occurences
      -h   print this help
      -m   allow exactly one leading tab in Makefiles (GNUmakefile, makefile, Makefile)
      -n   allow files not to end with newlines
      -T   allow all horizontal tabs
      -t   allow leading horizontal tabs for indentation

### todo

- by default, every file should be tested for correct utf-8 encoding (without BOM).
- make '-m' option default
- make '-c' option default
- option to detect trailing whitespace?
- add more platforms
- add '-r' option (recursive)


## pchk

currently implemented filename checks:
- POSIX Portable Filename Character Set: [A-Z][a-z][0-9].-_
- must not start with a hyphen (UNIX tools may interpret filenames as tool arguments).
- must not end with a period (Windows)
- dos/windows devices: CON, PRN, AUX, CLOCK$, NUL, COM[1-9], LPT[1-9], and these names followed by an extension (for example, NUL.tx7).

see http://www.boost.org/doc/libs/1_60_0/libs/filesystem/doc/portability_guide.htm#portable_name


### usage

    usage: pchk [-h] [dir ...]

    recursively checks pathnames for portability issues.

    returns 0 if pathnames are portable, 1 otherwise, 2 on failures.

      -h   print this help

### example

    $ mkdir puh
    $ > puh/con.
    $ pchk puh
    puh/con.: should not end with a period (Windows)
    puh/con.: windows special filename (con, prn, aux, clock$, nul, com1-9, lpt1-9)
    $ echo $?
    1
    $ 

### todo

- define portability:
    - list operating systems/filesystems (macOS, Windows, Linux, OpenBSD)
    - list tools and formats (tar formats, pkzip, udf)
- add more platforms
