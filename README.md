# c0chk

check files for c0 control chars

tested on macOS

## usage

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

## todo

- rewrite to use memory mapped io?
- by default, every file should be tested for correct utf-8 encoding (without BOM).
- add option '-u' to allow other 8bit encodings than utf-8.
- what about c1 controls?
