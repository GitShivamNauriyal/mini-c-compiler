#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Build the compiler
echo "Building minic compiler..."
make clean > /dev/null
make > /dev/null

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Build successful!${NC}\n"

# Function to run tests
run_tests() {
    local dir=$1
    local expected_status=$2
    echo "Running tests in $dir (Expected: $expected_status)"
    echo "--------------------------------------------------"
    
    for file in $dir/*.c; do
        if [ -f "$file" ]; then
            ./minic "$file" > /tmp/minic_output 2>&1
            local exit_code=$?
            
            if grep -q "\-\-\- Compilation Finished Successfully \-\-\-" /tmp/minic_output; then
                actual_status="SUCCESS"
            else
                actual_status="FAILED"
            fi
            
            if [ "$actual_status" == "$expected_status" ]; then
                echo -e "[PASS] $file"
            else
                echo -e "[FAIL] $file (Expected $expected_status but got $actual_status)"
                # cat /tmp/minic_output | grep "ERROR"
            fi
        fi
    done
    echo ""
}

# Run valid tests
run_tests "tests/valid" "SUCCESS"

# Run error tests
run_tests "tests/errors" "FAILED"
