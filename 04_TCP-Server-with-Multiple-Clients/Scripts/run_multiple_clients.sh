#!/bin/bash

CURRENT_DIR=$(pwd)
OUTPUT_DIR="${CURRENT_DIR}/Output/"
CLIENT_NUM_MAX=10

if [ ! -d "$OUTPUT_DIR" ]; then
    echo "$OUTPUT_DIR does not exists. Build the binary first."
    exit 1
fi

for i in $(seq 1 $CLIENT_NUM_MAX); do
    echo "[CLIENT $i] Starting the client..."
done
