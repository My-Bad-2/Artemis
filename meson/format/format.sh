#!/bin/bash

MESON_BUILD_ROOT=${MESON_BUILD_ROOT:-buildresults}
MESON_SOURCE_ROOT=${MESON_SOURCE_ROOT:-../../}
FILE_TYPES=
EXCLUDES_ARGS=
PATCH='0'

while getopts "e:p" opt; do
    case $opt in
    e)
        EXCLUDES_ARGS="$OPTARG"
        ;;
    p)
        PATCH='1'
        ;;
    \?)
        echo "Invalid option -$OPTARG" >&2
        ;;
    esac
done

shift $((OPTIND - 1))

IFS=',' read -ra DIRS <<<"$1"

IFS=',' read -ra ENTRIES <<<"$2"
for entry in "${ENTRIES[@]}"; do
    FILE_TYPES="$FILE_TYPES -o -iname \"$entry\""
done

IFS=',' read -ra ENTRIES <<<"$EXCLUDES_ARGS"
for entry in "${ENTRIES[@]}"; do
    EXCLUDES="$EXCLUDES -o -path \"$entry\""
done

if [[ ! -z $EXCLUDES ]]; then
    EXCLUDES=${EXCLUDES:3:${#EXCLUDES}}
    EXCLUDES="-type d \( $EXCLUDES \) -prune"
else
    FILE_TYPES=${FILE_TYPES:3:${#FILE_TYPES}}
fi

cd ${MESON_SOURCE_ROOT}
eval find ${DIRS[@]} ${EXCLUDES} -type f ${FILE_TYPES} |
    xargs clang-format -style=file -i -fallback-style=none

if [ "$PATCH" == '1' ]; then
    git diff >clang_format.patch

    if [ ! -s clang_format.patch ]; then
        rm clang_format.patch
    fi
fi
