#!/bin/bash

# Check if correct arguments are given
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <n> <file1> <file2> ..."
    exit 1
fi

# Extract the first argument as the number of top words to display
N=$1
shift  # Remove first argument, leaving only file names

# Loop through all files provided as arguments
for file in "$@"; do
    if [[ ! -f "$file" ]]; then
        echo "File $file not found!"
        continue
    fi

    echo "==== Processing: $file ===="

    # Count the number of lines, words, and characters
    wc_output=$(wc "$file")
    echo "=== $wc_output ==="

    # Extract and count words, sort by frequency, and display top N
    tr -c '[:alnum:]' '[\n*]' < "$file" | grep -v "^\s*$" | sort | uniq -c | sort -nr | head -n "$N"

    echo "========================="
done