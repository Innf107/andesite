#!/usr/bin/bash
set -e

make

failed=0
for f in $(find test -name "*.mcfunction" -type f) 
do
    expected="$(grep -Po '(?<=#EXPECT:).*' $f)"
    args=$(grep -Po '(?<=#ARGS:).*' $f; true)
    set +e
    result=$(out/andesite $args $f)
    runError=$?
    set -e
    if [ -z runError ]
    then 
        echo -e "\e[31mRUNTIME ERROR!!!: $result\e[0m"
        failed=$(echo "$failed + 1" | bc)
    elif  [ "$(echo "$result" | tail -n 1 | grep -Po "$expected")" ] 
    then
        echo -e "\e[32mPassed: $f\e[0m"
    else
        echo -e "\e[31mFAILED!!!: $f"
        echo -e "   Expected: $expected"
        echo -e "   Received: $(echo "$result" | tail -n 1)"
        echo -e "   Context: \n$result\e[0m"
        failed=$(echo "$failed + 1" | bc)
    fi
done;

if [ $failed -eq 0 ]
then
    echo -e "\n\e[38;2;0;255;0mAll tests successfully passed\e[0m"
else
    echo -e "\n\e[38;2;255;0;0m$failed TEST(S) FAILED!!!\e[0m"
fi
