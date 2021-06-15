#!/usr/bin/bash
set -e

make

for f in $(find . -name "*.mcfunction" -type f) 
do
    expected=$(grep -Po '(?<=#EXPECT:).*' $f)
    result=$(out/andesite $f | tail -n 1)
    if [ -n "$(echo "$result" | grep -Po "$expected")" ] 
    then
        echo -e "\e[32mPassed: $f\e[0m"
    else
        echo -e "\e[31mFAILED!!!: $f"
        echo -e "    Expected: $expected"
        echo -e "    Received: $result\e[0m"
    fi
done;

