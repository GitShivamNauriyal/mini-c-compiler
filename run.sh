#!/bin/bash

# Colors for terminal
GREEN='\033[1;32m'
RED='\033[1;31m'
NC='\033[0m' 

COMPILER="./minic"

build() {
    echo "Building minic compiler..."
    make
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[SUCCESS]${NC} Build successful!"
    else
        echo -e "${RED}[ERROR]${NC} Build failed!"
        exit 1
    fi
}

clean() {
    echo "Cleaning project..."
    make clean
    echo -e "${GREEN}[SUCCESS]${NC} Cleanup complete."
}

run_test_dir() {
    local dir=$1
    local expected=$2
    echo "Running tests in $dir (Expected: $expected)"
    echo "--------------------------------------------------"
    
    for file in "$dir"/*.c; do
        if [ -f "$file" ]; then
            $COMPILER "$file" > /tmp/minic_out 2>&1
            if grep -q "\-\-\- Compilation Finished Successfully \-\-\-" /tmp/minic_out; then
                actual="SUCCESS"
            else
                actual="FAILED"
            fi
            
            if [ "$actual" == "$expected" ]; then
                echo -e "${GREEN}[PASS]${NC} $file"
            else
                echo -e "${RED}[FAIL]${NC} $file (Expected $expected, got $actual)"
            fi
        fi
    done
    echo ""
}

run_all_tests() {
    if [ ! -f "$COMPILER" ]; then
        echo -e "${RED}[ERROR]${NC} Compiler not found! Build it first."
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
            echo "Usage: ./run.sh [FLAG | FILE]"
            echo "Flags:"
            echo "  -t, --test    : Run all test cases (Valid & Error)"
            echo "  -b, --build   : Compile and run all tests"
            echo "  -c, --clean   : Clean and recompile only"
            echo "  <file>        : Run compiler on a specific file"
        fi
        ;;
esac
