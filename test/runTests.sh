#!/usr/bin/bash
set -e

make

for f in $(find test -name "*.mcfunction" -type f) 
do
    expected=$(grep -Po '(?<=#EXPECT:).*' $f)
    args=$(grep -Po '(?<=#ARGS:).*' $f; true)
    result=$(out/andesite $args $f)
    if [ $(echo "$result" | tail -n 1 | grep -Po "$expected") ] 
    then
        echo -e "\e[32mPassed: $f\e[0m"
    else
        echo -e "\e[31mFAILED!!!: $f"
        echo -e "    Expected: $expected"
        echo -e "    Received: \n$result\e[0m"
    fi
done;

