#!/bin/bash

if [ -z "$LTD_HOME" ]; then
    echo "Please set the \$LTD_HOME variable"
    exit
fi 

echo "\$LTD_HOME is set to '$LTD_HOME'"

echo "Building minimum binary..."

EXE="/tmp/ltd"
CXX="g++"
CXXFLAGS="-O3 -Wall -Werror=return-type -std=c++20 -s"
TMP_DIR="/tmp"

# Initialize flags
REBUILD_EXE=false
OBJ_FILES=()

SRC_FILES=(
    "app/main.cpp" 
    "app/commands.cpp" 
    "app/command_handlers.cpp" 
    "app/build_tools.cpp"
    "app/env_vars.cpp" 
    "app/ignore_filter.cpp"
    "app/ltd_home.cpp" 
    "lib/cli.cpp" 
    "lib/fmt.cpp" 
    "lib/stddef.cpp" 
    "lib/stdfs.cpp" 
    "lib/stdltd.cpp" 
    "lib/allocators.cpp" 
    "lib/log.cpp" 
    "lib/log_writer.cpp" 
    "lib/log_media_console.cpp"
)

for SRC in "${SRC_FILES[@]}";  do
    # Ensure the source file actually exists
    if [ ! -f "$SRC" ]; then
        echo "Error: Source file '$SRC' not found."
        exit 1
    fi

    SAFE_NAME=$(echo "$SRC" | tr '/' '_')
    OBJ="$TMP_DIR/${SAFE_NAME%.cpp}.o"
    OBJ_FILES+=("$OBJ")

    # Check if .cpp is newer than .o, or if .o does not exist
    if [ "$SRC" -nt "$OBJ" ] || [ ! -f "$OBJ" ]; then
        echo "Compiling $SRC..."
        if $CXX $CXXFLAGS -c "$SRC" -o "$OBJ"; then
            REBUILD_EXE=true
        else
            echo "Error: Failed to compile $SRC"
            exit 1
        fi
    fi
done

if [ ! -f "$EXE" ]; then
    REBUILD_EXE=true
fi

if [ "$REBUILD_EXE" = true ]; then
    echo "Linking object files into $EXE..."
    if $CXX "${OBJ_FILES[@]}" -o "$EXE"; then
        echo "Build successful: $EXE"
    else
        echo "Error: Linking failed"
        exit 1
    fi
else
    echo "Project is already up-to-date."
fi

if [ $? -ne 0 ]; then
    echo "Error: Failed to build minimum binary."
    exit 1
fi

echo "Selecting 'ltd' as active project..."
/tmp/ltd cd ltd