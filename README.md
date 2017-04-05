# c0chk
check files for c0 control chars

tested on macOS

## usage

    usage: c0chk [-chm] [file ...]

    checks files for c0 control chars except line feed '\n'.

    returns 0 if no offending chars were found, 1 otherwise, 2 on failures.

      -c   continue inspecting, print all occurences
      -h   print this help
      -m   allow tabs in Makefiles
