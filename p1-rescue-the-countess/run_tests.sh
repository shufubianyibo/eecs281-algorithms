// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#!/bin/bash
make

echo "---------------------------------------"
echo "Running Custom Test Suite"
echo "---------------------------------------"

# Function to run a specific test case
run_test() {
    TEST_NAME=$1
    ARGS=$2
    INPUT_FILE=$3
    
    echo "Testing $TEST_NAME..."
    
    ./superMarco $ARGS < $INPUT_FILE > actual_output.txt
    EXIT_CODE=$?

    if [ $EXIT_CODE -ne 0 ]; then
        echo "  [FAIL] Program crashed or returned non-zero exit code: $EXIT_CODE"
        return
    fi
    
    echo "  [PASS] Ran successfully. Check actual_output.txt for correctness."
}
run_fail_test() {
    TEST_NAME=$1
    INPUT_FILE=$2
    
    echo "Testing Error Case: $TEST_NAME..."
    
    # Run program. We don't care about output file, just the exit code.
    ./superMarco --stack -o M < $INPUT_FILE > /dev/null 2>&1
    EXIT_CODE=$?

    if [ $EXIT_CODE -eq 1 ]; then
        echo "  [PASS] Program correctly exited with status 1."
    elif [ $EXIT_CODE -eq 0 ]; then
        echo "  [FAIL] Program returned 0 (Success) but should have failed!"
    else
        echo "  [FAIL] Program crashed with unknown code: $EXIT_CODE"
    fi
}

# --- TEST CASES ---

# 1. Simple Stack
run_test "Simple Map (Stack)" "--stack -o M" "test-simple.txt"

# 2. Simple Queue
run_test "Simple List (Queue)" "--queue -o L" "test-simple.txt"

# 3. Pipe Test
run_test "Pipe Logic (Stack)" "--stack -o L" "test-pipe.txt"

# 4. No Solution Test
run_test "No Solution Map (Stack)" "--stack -o M" "test-nosolution.txt"

echo "---------------------------------------"
echo "Running Edge Case Tests"
echo "---------------------------------------"

# 1. Test More than 10 Rooms
run_fail_test "11 Rooms (Too Many)" "test-invalid-11rooms.txt"

# 2. Test 0 Rooms
run_fail_test "0 Rooms (Too Few)" "test-invalid-0rooms.txt"

# 3. Test 0 Size
run_fail_test "Room Size 0" "test-invalid-0size.txt"

# 4. Test Empty File
run_fail_test "Empty Input File" "test-empty-file.txt"

# 5. Test Invalid Characters
run_fail_test "Invalid Characters in Map" "test-invalid-characters.txt"

# 6. Test Missing Start
run_fail_test "Missing Start Position" "test-missing-start.txt"

echo "---------------------------------------"
echo "All tests completed."