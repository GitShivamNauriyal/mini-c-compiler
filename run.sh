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
    local opt_flag=$3
    echo "Running tests in $dir (Expected: $expected, Opt: $opt_flag)"
    echo "--------------------------------------------------"
    
    for file in "$dir"/*.c; do
        if [ -f "$file" ]; then
            $COMPILER $opt_flag "$file" > /tmp/minic_out 2>&1
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
    local opt_flag=$1
    run_test_dir "tests/valid" "SUCCESS" "$opt_flag"
    run_test_dir "tests/errors" "FAILED" "$opt_flag"
}

case "$1" in
    --clean|-c)
        clean
        build
        ;;
    --build|-b)
        build
        run_all_tests "-O1"
        ;;
    --test|-t)
        run_all_tests "-O1"
        ;;
    --test-no-opt)
        run_all_tests "-O0"
        ;;
    *)
        if [ -n "$1" ] && [ -f "$1" ]; then
            $COMPILER "$@"
        else
            echo "Usage: ./run.sh [FLAG | FILE]"
            echo "Flags:"
            echo "  -t, --test         : Run all tests with optimization (-O1)"
            echo "  --test-no-opt      : Run all tests without optimization (-O0)"
            echo "  -b, --build        : Compile and run all tests"
            echo "  -c, --clean        : Clean and recompile only"
            echo "  <file> [opt_flag]  : Run compiler on a specific file (e.g., ./run.sh test.c -O0)"
        fi
        ;;
esac
