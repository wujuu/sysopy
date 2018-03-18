#!/bin/bash

TEST_RECORDS1=$1
TEST_RECORDS2=$2

touch wyniki.txt

echo -e "Running tests for file_mgmt library...\n"


for BYTES in 4 512 4096 8192
do
    for TEST_RECORDS in $TEST_RECORDS1 $TEST_RECORDS2 
    do
        echo -e "Generating $BYTES byte, $TEST_RECORDS record file...\n"
        ./cmd_file_mgmt generate $BYTES_bytes_data $TEST_RECORDS $BYTES 



        echo -e "Copying $BYTES byte, $TEST_RECORDS record file with system libraries...\n"
        echo -e "System libraries $BYTES bytes, $TEST_RECRODS1 COPYING time stats: \n" >> wyniki.txt
        { time ./cmd_file_mgmt copy $BYTES_bytes_data $BYTES_bytes_data_copy_sys $TEST_RECORDS $BYTES sys ; } 2>> wyniki.txt

        echo -e "Copying $BYTES byte, $TEST_RECORDS record file with C libraries...\n"
        echo -e "C libraries $BYTES bytes, $TEST_RECRODS1 COPYING time stats: \n" >> wyniki.txt
        { time ./cmd_file_mgmt copy $BYTES_bytes_data $BYTES_bytes_data_copy_lib $TEST_RECORDS $BYTES lib ; } 2>> wyniki.txt


        
        echo -e "Sorting $BYTES byte, $TEST_RECORDS record file with system libraries...\n"
        echo -e "System libraries $BYTES bytes, $TEST_RECORDS SORTING time stats: \n" >> wyniki.txt
        { time ./cmd_file_mgmt sort $BYTES_bytes_data_copy_sys $TEST_RECORDS $BYTES sys ; } 2>> wyniki.txt

        echo -e "Sorting $BYTES byte, $TEST_RECORDS record file with system libraries...\n"
        echo -e "C libraries $BYTES bytes, $TEST_RECORDS SORTING time stats: \n" >> wyniki.txt
        { time ./cmd_file_mgmt sort $BYTES_bytes_data_copy_sys $TEST_RECORDS $BYTES lib ; } 2>> wyniki.txt
    done
done

