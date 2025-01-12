#!/bin/bash

if [ -z "$LTD_HOME" ]; then
    echo "Please set the \$LTD_HOME variable"
    exit
fi 

echo "\$LTD_HOME is set to '$LTD_HOME'"

echo "Building binary..."

g++  $1 -Ofast -std=c++17 app/ltd.cpp app/sdk.cpp app/compiler.cpp lib/cli_args.cpp lib/cli.cpp lib/fmt.cpp lib/stddef.cpp -o $LTD_HOME/ltd

echo "Adding binary to \$PATH"

case ":$PATH:" in
    *"$LTD_HOME"*) :;;
    *) PATH=$PATH:$LTD_HOME; export PATH
esac

echo "Don't forget to add $LTD_HOME to your PATH"