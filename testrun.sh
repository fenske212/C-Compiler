#!/bin/bash

# Build the project with make
make

# Define an array of test case file names
test_cases=(
  "testcases/test_assignment_basic1.txt"
  "testcases/test_assignment_basic2.txt"
  "testcases/test_assignment_variables_no_init.txt"
  "testcases/test_assignment_variables1.txt"
  "testcases/test_assignment_variables2_operators.txt"
  "testcases/test_control_i_if_f_i_if_f1.txt"
  "testcases/test_control_i_if_f_i_if_f2.txt"
  "testcases/test_control_i_if_f1.txt"
  "testcases/test_control_i_if_f2.txt"
  "testcases/test_control_i_if_f3.txt"
  "testcases/test_control_i_if_f4.txt"
  "testcases/test_control_i_if_f5.txt"
  "testcases/test_control_i_if_fif1.txt"
  "testcases/test_control_i_if_fif2.txt"
  "testcases/test_control_if1.txt"
  "testcases/test_control_if2.txt"
  "testcases/test_control_if3_list.txt"
  "testcases/test_control_if3_list2.txt"
  "testcases/test_control_if4_list.txt"
  "testcases/test_control_ifif1.txt"
  "testcases/test_control_ifif2.txt"
  "testcases/test_control_ifif3.txt"
  "testcases/test_control_while1.txt"
  "testcases/test_control_while2.txt"
  "testcases/test_control_while3_fibonacci.txt"
  "testcases/test_control_while4.txt"
  "testcases/test_control_while5.txt"
  "testcases/test_control_whilewhile_if1.txt"
  "testcases/test_control_whilewhile6.txt"
  "testcases/test_control_whilewhile7.txt"
  "testcases/test_control_whilewhile8.txt"
  "testcases/test_control_whilewhile9.txt"
  "testcases/test_control_switch1.txt"
  "testcases/test_control_for1.txt"
)

# Loop through the test cases and run them
for test_case in "${test_cases[@]}"
do
  echo "Running test case: $test_case"
  actual_output=$(./a.out < $test_case)
  expected_output=$(cat "$test_case.expected")
  if [ "$actual_output" == "$expected_output" ]; then
    echo "Passed"
    echo
  else
    echo "Failed"
    echo
  fi
done