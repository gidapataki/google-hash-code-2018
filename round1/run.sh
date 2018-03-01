#!/bin/bash -e


mkdir -p output
mkdir -p best_output


run_solution() {
    EXECUTABLE="$1"
    shift
    ARGS=("$@")

    echo "-- ${ARGS[@]} --"
    scores=($(cat scores.best))
    best_scores=()

    i=0
    for f in `\ls -1 input/`; do
        # Run stuff
        output_file="output/${f}.out"
        current=$("${EXECUTABLE}" "${ARGS[@]}" < "input/${f}" 2>&1 >"${output_file}")

        # Print * if there is a better score, and = if its the same
        best="${scores[$i]}"
        new_best="${best}"

        if test "${current}" -gt "${best}"
        then
            new_best="${current}"
            cp "${output_file}" "best_${output_file}"
            echo "${current} *"
        elif test "${current}" -eq "${best}"
        then
            echo "${current} ="
        else
            echo "${current}"
        fi

        i=$((i + 1))
        best_scores+=("${new_best}")
    done


    # Update best scores
    for score in "${best_scores[@]}"
    do
        echo "${score}"
    done > scores.best
}

for i in $(seq 0 10)
do
    run_solution "$1" "$i"
done

git archive -o src.zip HEAD
