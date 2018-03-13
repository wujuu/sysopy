#!/bin/bash

for i in dblocks sblocks ;
do 
    make shared LIB=$i 
    make static LIB=$i
done