# c0chk

check files for c0 control chars

tested on macOS

## usage

    usage: c0chk [-achmTt] [file ...]

    checks files for c0 control chars except line feed '\n'.

    returns 0 if no offending chars were found, 1 otherwise, 2 on failures.

      -a   ascii, do not allow 8 bit characters (iso, utf-8)
      -c   continue inspecting, print all occurences
      -h   print this help
      -m   allow exactly one leading tab in Makefiles (GNUmakefile, makefile, Makefile)
      -T   allow all horizontal tabs
      -t   allow leading horizontal tabs for indentation

## todo

- add '-n' option: files are allowed not to end with an eol-symbol, must end in eol would be default.
- rewrite to use memory mapped io
- add '-u' option: check for utf-8 encoding (without BOM).
