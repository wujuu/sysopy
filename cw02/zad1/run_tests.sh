#!/bin/bash

TEST_RECORDS1=$1
TEST_RECORDS2=$2

touch wyniki.txt

echo "Running tests for file_mgmt library...\n"


for BYTES in 4 512, 4096, 8192
    for TEST_RECORDS in $TEST_RECORDS1 $TEST_RECORDS2 
    do
        echo "Generating 4 byte, $TEST_RECORDS record file...\n"

        ./cmd_file_mgmt generate 4_bytes_data $TEST_RECORDS 4 


        echo "System libraries 4 bytes, $TEST_RECRODS1 COPYING time stats: \n" >> wyniki.txt

        time ./cmd_file_mgmt copy 4_bytes_data 4_bytes_data_copy_sys $TEST_RECORDS 4 sys >> wyniki.txt


        echo "C libraries 4 bytes, $TEST_RECRODS1 COPYING time stats: \n" >> wyniki.txt

        time ./cmd_file_mgmt copy 4_bytes_data 4_bytes_data_copy_lib $TEST_RECORDS 4 lib >> wyniki.txt


        echo "System libraries 4 bytes, $TEST_RECORDS SORTING time stats: \n" >> wyniki.txt

        time ./cmd_file_mgmt sort 4_bytes_data_copy_sys $TEST_RECORDS 4 sys

        echo "C libraries 4 bytes, $TEST_RECORDS SORTING time stats: \n" >> wyniki.txt

        time ./cmd_file_mgmt sort 4_bytes_data_copy_sys $TEST_RECORDS 4 lib
    done

