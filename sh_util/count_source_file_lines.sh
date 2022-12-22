#!/bin/sh

if [[ $# != 2 ]]; then
    echo "Please input language type and directory!"
    echo " cmd: count_source_file_lines.sh [java/cpp/shell/php/go] directory "
    exit -1
fi

CPP_SUFFIX="\.c$|\.cpp$|\.h$|\.cc$"
JAVA_SUFFIX="\.java$"
SHELL_SUFFIX="\.sh$"
PHP_SUFFIX="\.php$"
GO_SUFFIX="\.go$"


function count_sources_files_lines()
{
    if [[ $# -lt 1 ]]; then
        echo "Please input files"
        exit -1
    fi

    language=$1
    source_path=$2
    case $language in 
        "java")
            SUFFIX=$JAVA_SUFFIX
            ;;
        "cpp")
            SUFFIX=$CPP_SUFFIX
            ;;
        "shell")
            SUFFIX=$SHELL_SUFFIX
            ;;
        "php")
            SUFFIX=$PHP_SUFFIX
            ;;
        "go")
            SUFFIX=$GO_SUFFIX
            ;;
        *)
            echo "please input right language"
            exit -1
            ;;
    esac

    files=$(find $source_path -name "*" | grep -E "$SUFFIX")
    for file in $files;
    do
        lines=$(cat $file | wc -l)
        echo "$file: $lines lines"
        (( sum = sum + $lines ))
    done

    echo "sum: $sum"
}

count_sources_files_lines $1 $2
