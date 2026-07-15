#!/bin/sh
printf '\033c\033]0;%s\a' godot cpp template
base_path="$(dirname "$(realpath "$0")")"
"$base_path/godot cpp template.x86_64" "$@"
