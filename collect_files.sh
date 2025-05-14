#!/bin/bash

# Define the output file name
OUTPUT_FILE="project_contents.txt"

# Empty the output file
> "$OUTPUT_FILE"

# Find all .c, .h, and Makefile files and process each
find . -type f \( -name "*.c" -o -name "*.h" -o -name "Makefile" \) | while read file; do
    # Write the relative path
    echo "=== $file ===" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"
    # Append the file's contents
    cat "$file" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"
done