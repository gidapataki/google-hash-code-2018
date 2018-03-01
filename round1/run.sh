#!/bin/bash -e

EXECUTABLE="$1"

mkdir -p output

scores=($(cat scores.best))
best_scores=()

i=0
for f in `ls input/`; do
    # Run stuff
    current=$("$1" < "input/${f}" 2>&1 >"output/${f}.out")

    # Print * if there is a better score, and = if its the same
    best="${scores[$i]}"
    new_best="${best}"
    msg="${current}"

    if test "${current}" -gt "${best}"
    then
        msg="${current} *"
        new_best="${current}"
    elif test "${current}" -eq "${best}"
    then
        msg="${current} ="
    fi
    echo "${msg}"
    i=$((i + 1))
    best_scores+=("${new_best}")
done


# Update best scores
for score in "${best_scores[@]}"
do
    echo "${score}"
done > scores.best

git archive -o src.zip HEAD
