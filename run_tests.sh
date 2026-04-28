#!/bin/sh
set -eu

make

for file in tests/small.txt tests/medium.txt tests/large.txt; do
    printf '\n== %s ==\n' "$file"
    while IFS= read -r command; do
        [ -n "$command" ] || continue
        printf '\n$ %s\n' "$command"
        sh -c "$command"
    done < "$file"
done
