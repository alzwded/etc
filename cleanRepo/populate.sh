#!/bin/bash

# Ensure the script is run from its own directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CURRENT_DIR="$(pwd)"

if [ "$SCRIPT_DIR" != "$CURRENT_DIR" ]; then
    echo "Error: This script must be run from its own directory ($SCRIPT_DIR)."
    exit 1
fi

# Purge current directory except for the script itself and .geminiignore
for i in * .* ; do
    case "$i" in
        . | .. | populate.sh | .geminiignore)
            ;;
        *)
            echo Purging "$i"
            rm -rf "$i"
            ;;
    esac
done

EXCLUDES=()

# Read excludes from .geminiignore (Single Source of Truth)
if [ -f .geminiignore ]; then
    while IFS= read -r line || [ -n "$line" ]; do
        # Trim leading/trailing whitespace
        trimmed=$(echo "$line" | xargs)
        # Skip empty lines and lines starting with #
        if [[ -n "$trimmed" && ! "$trimmed" =~ ^# ]]; then
            EXCLUDES+=("--exclude=$trimmed")
        fi
    done < .geminiignore
fi

# Use the first argument as the source directory
SOURCE_DIR="$1"

if [ ! -d "$SOURCE_DIR" ]; then
    echo "Usage: $0 <source_directory>"
    exit 1
fi

if [ -d "$SOURCE_DIR" ]; then
    rsync -av "${SOURCE_DIR%/}/" "${EXCLUDES[@]}" .
else
    echo "Error: Source directory '$SOURCE_DIR' not found or is not a directory."
    exit 1
fi



