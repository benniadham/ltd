#!/bin/bash

if [ -z "$LTD_HOME" ]; then
    echo "Please set the \$LTD_HOME variable"
    exit
fi 

echo "\$LTD_HOME is set to '$LTD_HOME'"

echo "Building minimum binary..."

g++ $1 -Ofast -std=c++17 app/ltd.cpp app/sdk.cpp app/compiler.cpp lib/cli.cpp lib/fmt.cpp lib/stddef.cpp -o /tmp/ltd

echo "Selecting 'ltd' as active project..."
/tmp/ltd cd ltd
/tmp/ltd build -v
/tmp/ltd deploy

echo "Building 'ltd' project..."
/tmp/ltd build -vv

echo "Deploying 'ltd' binaries..."
/tmp/ltd deploy

echo "Trying to add '$LTD_HOME/modules/ltd/' to your PATH"

PATH=$PATH:$LTD_HOME/modules/ltd; export PATH