#!/bin/bash

valgrind --tool=memcheck \
         --gen-suppressions=all \
         --leak-check=full \
         --show-leak-kinds=all \
         --leak-resolution=high \
         --track-origins=yes \
         --vgdb=no \
         --error-exitcode=1 "$@"
