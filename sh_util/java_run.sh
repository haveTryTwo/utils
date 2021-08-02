#!/bin/sh

work_space_dir=/Users/test/eclipse-workspace

javac $work_space_dir/Test1/src/org/com/my/TestArgs.java

mkdir -p target/org/com/my
mv  $work_space_dir/Test1/src/org/com/my/TestArgs.class   target/org/com/my

cur_dir=$(pwd)
echo "java -cp $cur_dir/target -Dencoding=utf8 org.com.my.TestArgs aa bb"
java -cp $cur_dir/target -Dencoding=utf8 org.com.my.TestArgs aa bb

cd $cur_dir/target
jar cvf $cur_dir/target/TestArgs.jar org/com/my/*.class
cd -
rm -fr $cur_dir/target/org
echo "java -cp $cur_dir/target/* -Dencoding=utf8 org.com.my.TestArgs aa bb"
java -cp $cur_dir/target/* -Dencoding=utf8 org.com.my.TestArgs aa bb
