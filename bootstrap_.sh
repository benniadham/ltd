#!/bin/bash

if [ -z "$LTD_HOME" ]; then
    echo "Please set the \$LTD_HOME variable"
    exit
fi 

echo "\$LTD_HOME is set to '$LTD_HOME'"

echo "Building minimum binary..."

g++ $1 -Ofast -std=c++17 app/ltd.cpp app/sdk.cpp app/compiler.cpp lib/cli.cpp lib/fmt.cpp lib/stddef.cpp lib/log.cpp lib/log_writer.cpp lib/log_media_console.cpp -o /tmp/ltd

if [ $? -ne 0 ]; then
    echo "Error: Failed to build minimum binary."
    exit 1
fi

echo "Selecting 'ltd' as active project..."
/tmp/ltd cd ltd

echo "Building 'ltd' project..."
/tmp/ltd build -vv

echo "Deploying 'ltd' binaries..."
/tmp/ltd deploy

PATH=$PATH:$LTD_HOME/modules/ltd; export PATH