#!/bin/sh

### 功能：读取文件并判断是否有包含要检查的字段，如果有则打印指定连续行数的文件，用于显示有相关性的内容

if [[ $# != 3 ]]; then
    echo "Please input check_file check_content num_of_consistent_line"
    exit -1
fi

check_file=$1
check_content=$2
num_of_consistent_line=$3


awk -F' ' '

BEGIN {

printable = 0;
num_of_print = 0;

}

{

if (printable == 0) {
    if (index($0, "'"$check_content"'") != 0) {
        printable = 1
    }
}

if (printable == 1) {
    if (num_of_print < "'"$num_of_consistent_line"'") {
        print $0;
        num_of_print++;
    } else {
        printable = 0
        num_of_print = 0
    }
}

}

END {
}

' $check_file
