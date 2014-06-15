#!/bin/sh

SUFFIX="\.c$|\.cpp$|\.h$|\.sh$|\.cc$|\.php$"

function count_sources_files_lines()
{
    if [ $# -lt 1 ]; then
        echo "Please input files"
        exit -1
    fi

    source_path=$1

    files=`find $source_path -name "*" | grep -E "$SUFFIX"`
    for file in $files;
    do
        lines=`cat $file | wc -l`
        echo "$file: $lines lines"
        sum=`expr $sum + $lines`
    done

    echo "sum: $sum"
}

count_sources_files_lines $1
