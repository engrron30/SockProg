#!/bin/bash

CURRENT_DIR=$(pwd)
OUTPUT_DIR="${CURRENT_DIR}/Output/"

if [ ! -d "$OUTPUT_DIR" ]; then
    echo "$OUTPUT_DIR does not exists. Build the binary first."
    exit 1
fi
