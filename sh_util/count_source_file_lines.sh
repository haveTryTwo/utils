#!/bin/sh

C_SUFFIX="\.c$|\.h$"
CPP_SUFFIX="\.c$|\.cpp$|\.h$|\.cc$"
SHELL_SUFFIX="\.sh$"
PHP_SUFFIX="\.php$"
JAVA_SUFFIX="\.java$"

function count_sources_files_lines()
{
    if [ $# -lt 1 ]; then
        echo "Please input files"
        exit -1
    fi

    source_path=$1

    files=`find $source_path -name "*" | grep -E "$JAVA_SUFFIX"`
    for file in $files;
    do
        lines=$(cat $file | wc -l)
        echo "$file: $lines lines"
        (( sum = sum + $lines ))
    done

    echo "sum: $sum"
}

count_sources_files_lines $1
