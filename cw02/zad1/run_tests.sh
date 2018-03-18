#!/bin/bash

TEST_RECORDS1=$1
TEST_RECORDS2=$2
BYTES_DATA=_bytes_
COPY_LIB=_copy_lib
COPY_SYS=_copy_sys
RECORDS=_records_data

touch wyniki.txt

echo "Running tests for file_mgmt library..."
echo ""


for BYTES in 4 512 4096 8192
do
    for TEST_RECORDS in $TEST_RECORDS1 $TEST_RECORDS2 
    do
        echo "Generating $BYTES bytes, $TEST_RECORDS record file..."
        ./cmd_file_mgmt generate $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS $TEST_RECORDS $BYTES 

        echo "Time stats for $BYTES bytes, $TEST_RECORDS records file:" >> wyniki.txt
        echo "" >> wyniki.txt



        echo "Copying this file using system libraries..."
        echo "Copying using system libraries:" >> wyniki.txt
        { time ./cmd_file_mgmt copy $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS$COPY_SYS $TEST_RECORDS $BYTES sys ; } 2>> wyniki.txt

        echo "" >> wyniki.txt

        echo "Copying this file using C libraries..."
        echo "Copying using C libraries:" >> wyniki.txt
        { time ./cmd_file_mgmt copy $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS$COPY_LIB $TEST_RECORDS $BYTES lib ; } 2>> wyniki.txt



        echo "" >> wyniki.txt



        echo "Sorting this file using system libraries..."
        echo "Sorting using system libraries:" >> wyniki.txt
        { time ./cmd_file_mgmt sort $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS$COPY_SYS $TEST_RECORDS $BYTES sys ; } 2>> wyniki.txt

        echo "" >> wyniki.txt

        echo "Sorting this file using C libraries..."
        echo "Sorting using C libraries:" >> wyniki.txt
        { time ./cmd_file_mgmt sort $BYTES$BYTES_DATA$TEST_RECORDS$RECORDS$COPY_LIB $TEST_RECORDS $BYTES lib ; } 2>> wyniki.txt



        echo ""
        echo "" >> wyniki.txt
        echo "" >> wyniki.txt
        
    done
done

if [ "$3" == "clean" ]
then
    rm *_bytes_*
fi