#!/bin/bash

cd ./root

find ./ -type d > ../InputFiles/dir_list.txt
find ./ -type f -ls > ../InputFiles/file_list.txt