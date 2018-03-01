#!/bin/bash -e

EXECUTABLE="$1"

mkdir -p output

for f in `ls input/`; do
    "$1" < "input/${f}" > "output/${f}.out"
done

git archive -o src.zip HEAD
