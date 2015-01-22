#!bin/bash
########################################################################
### This script converts all files located in this repo's Code dir   ###
### from whatever their encoding was to Utf-8.                       ###
###                                                                  ###
### Required programs:                                               ###
###   iconv  For converting file encodings                           ###
###   git    For determining the actual root directory where the     ###
###          Code folder is located at                               ###
########################################################################

# abort script on first error
set -e

while [ $# -ge 1 ]; do
    case $1 in
        -q|--quiet )
            # Mute stdout
            exec 1> /dev/null;;
        -s|--silent )
            # Mute stdout and stderr
            exec 1> /dev/null 2>&1;;
    esac
    shift 1
done

# Required programs
which iconv 0> /dev/null
which git   0> /dev/null

root="$(git rev-parse --show-toplevel)"

if [ "$TERM" = "cygwin" ]; then
    which cygpath
    root="$(cygpath -m "$root")"
fi

echo "Working directory: $root"
cd "$root"

for f in $(find ./Code -type f); do
    echo "  $f"
    iconv -t UTF-8 < "$f" > "$f.tmp"
    cp "$f.tmp" "$f"
    rm "$f.tmp"
done

cd -
