#!/bin/bash

# Colors for terminal
GREEN='\e[1;32m'
RED='\e[1;31m'
NC='\e[0m' 

COMPILER="./minic"

build() {
    printf "Building minic compiler...\n"
    make
    if [ $? -eq 0 ]; then
        printf "${GREEN}[SUCCESS]${NC} Build successful!\n"
    else
        printf "${RED}[ERROR]${NC} Build failed!\n"
        exit 1
    fi
}

clean() {
    printf "Cleaning project...\n"
    make clean
    printf "${GREEN}[SUCCESS]${NC} Cleanup complete.\n"
}

run_test_dir() {
    local dir=$1
    local expected=$2
    printf "Running tests in $dir (Expected: $expected)\n"
    printf "--------------------------------------------------\n"
    
    for file in "$dir"/*.c; do
        if [ -f "$file" ]; then
            $COMPILER "$file" > /tmp/minic_out 2>&1
            if grep -q "\-\-\- Compilation Finished Successfully \-\-\-" /tmp/minic_out; then
                actual="SUCCESS"
            else
                actual="FAILED"
            fi
            
            if [ "$actual" == "$expected" ]; then
                printf "${GREEN}[PASS]${NC} %s\n" "$file"
            else
                printf "${RED}[FAIL]${NC} %s (Expected %s, got %s)\n" "$file" "$expected" "$actual"
            fi
        fi
    done
    printf "\n"
}

run_all_tests() {
    if [ ! -f "$COMPILER" ]; then
        printf "${RED}[ERROR]${NC} Compiler not found! Build it first.\n"
        exit 1
    fi
    run_test_dir "tests/valid" "SUCCESS"
    run_test_dir "tests/errors" "FAILED"
}

case "$1" in
    --clean|-c)
        clean
        build
        ;;
    --build|-b)
        build
        run_all_tests
        ;;
    --test|-t)
        run_all_tests
        ;;
    *)
        if [ -n "$1" ] && [ -f "$1" ]; then
            $COMPILER "$1"
        else
            printf "Usage: ./run.sh [FLAG | FILE]\n"
            printf "Flags:\n"
            printf "  -t, --test    : Run all test cases (Valid & Error)\n"
            printf "  -b, --build   : Compile and run all tests\n"
            printf "  -c, --clean   : Clean and recompile only\n"
            printf "  <file>        : Run compiler on a specific file\n"
        fi
        ;;
esac
