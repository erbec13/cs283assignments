#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Check if cd with one argument changes the directory properly" {
    run ./dsh <<EOF
cd ..
pwd
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/ece46/cs283/cs283_gitdsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Check if cd with no arguments does nothing" {
    run ./dsh <<EOF
cd
pwd
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="/home/ece46/cs283/cs283_git/4-ShellPt2dsh2> dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Spaces before commands do not affect execution" {
    run ./dsh <<EOF                
       ls
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Spaces after commands do not affect execution" {
    run ./dsh <<EOF                
ls         
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Duplicate spaces in commands do not affect execution" {
    run ./dsh <<EOF                
ls         -a
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="...batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mddsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "The exit command will exit the shell with no errors" {
    run ./dsh <<EOF                
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Non-existant commands print the return value of -6 and cause no error" {
    run ./dsh <<EOF
asdk
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> cmd loop returned -6dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "Empty command prints the return value of -6 and causes no error" {
    run ./dsh <<EOF

EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh2> warning: no commands providedcmd loop returned -6dsh2> warning: no commands provideddsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}