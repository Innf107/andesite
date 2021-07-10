#!/usr/bin/bash
set -e

make

failed=0
for f in $(find test -name "*.mcfunction" -type f) 
do
    expected="$(grep -Po '(?<=#EXPECT:).*' $f | sed 's/\\n/\#/g')"
    args=$(grep -Po '(?<=#ARGS:).*' $f; true)
    lines=$(grep -Po '(?<=#LINES:).*' $f || echo '1')
    set +e

    if [ "$(echo "$expected" | grep -P '^\s*\d+\s*$')" ]    
    then
        query="\D$expected\D"
    else
        query="$expected"
    fi
    result=$(out/andesite $args $f)
    set -e
    if  [ "$(echo "$result" | tail -n $lines | tr "\n" \# | grep -Po "$query")" ] 
    then
        echo -e "\e[32mPassed: $f\e[0m"
    else
        echo -e "\e[31mFAILED!!!: $f"
        echo -e "   Expected: $(echo "$expected" | sed s/\#/"\n"/g)"
        echo -e "   Received: $(echo "$result" | tail -n $lines)"
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
