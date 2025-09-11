#!/bin/bash

devenv q4sdk.sln -build "Release" -project game -projectconfig "Release" -out "log.txt"
if [ $? -ne 0 ]; then
    cat "log.txt"
    rm "log.txt"
    exit 1
fi

exit 0
