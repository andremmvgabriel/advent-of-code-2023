#!/bin/bash
set -e

# Constants
DAYS=(); for ((i=1; i<=25; i++)); do DAYS+=("$(printf "%02d" $i)"); done

# Variables
CLEAN="FALSE"
EXAMPLE="FALSE"
EXAMPLE_NUM=""
DAY=00
PROJECT_DIR="$(dirname "$(realpath -s "$0")")"

# Input flags
while test $# -gt 0; do
  case "$1" in
    -h|--help)
        echo "Usage:"
        echo "$ ./execute.sh <options>"
        echo ""
        echo "Options:"
        echo "-h,  --help               show brief help"
        echo "-c,  --clean              clear build directory of target day"
        echo "-d,  --day=DAY            specify the day to run"
        echo "-e,  --example=EXAMPLE    run day with the example inputs"
        exit 0
        ;;
    -c|--clean)
        CLEAN="TRUE"
        shift
        ;;
    -d*|--day*)
        shift
        if test $# -gt 0; then
            DAY=$(printf "%02d" "${1}")
            shift
        else
            echo "You forgot to specify a day..."
            echo ""
            ./execute.sh --help
            exit 1
        fi
        ;;
    -e|--example)
        EXAMPLE="TRUE"
        shift
        if test $# -gt 0; then
            EXAMPLE_NUM="${1}"
            shift
        else
            EXAMPLE_NUM="1"
        fi
        ;;
    *)
        break
        ;;
  esac
done

# Core functions
verification() {
    # Checks if the input day is a valid day [1-25]
    if [[ ! "${DAYS[*]}" =~ ${DAY} ]]; then
        # whatever you want to do when array doesn't contain value
        echo "Day ${DAY} is not a valid day. Please insert a value between [1-25]."
        exit 1
    fi

    # Checks if the input day exists in the repository
    if [[ ! $(find . -maxdepth 1 -type d -name "day_${DAY}") ]]; then
        echo "Day ${DAY} is yet to be developed."
        exit 1
    fi
}

build_repository() {
    # Makes sure the build directory exists
    mkdir -p build

    # Removes the build of the target day, in case it exists
    if [ "${CLEAN}" == "TRUE" ]; then
        rm -rf "build/day_${DAY}"
    fi

    # Builds
    pushd "build" > /dev/null || exit 1
        cmake .. && cmake --build .
    popd > /dev/null || exit 1
}

run_day() {
    # Checks which input file to insert
    if [ "${EXAMPLE}" == "TRUE" ]; then
        input_file="example${EXAMPLE_NUM}.txt"
    else
        input_file="input.txt"
    fi

    # Validates that the file exist
    if [[ ! -f "day_${DAY}/${input_file}" ]]; then
        echo "Specified input file does not exist for day ${DAY}. File: ${input_file}".
        exit 1
    fi

    # Execute the binary
    pushd bin > /dev/null || exit 1
        ./"Day_${DAY}" "../day_${DAY}/${input_file}"
    popd > /dev/null
}

# Calling of the core functions
pushd "${PROJECT_DIR}" > /dev/null || exit 1
    verification
    build_repository
    run_day
popd > /dev/null || exit 1
