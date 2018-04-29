#!/bin/bash
#
# Docker cleanup script
# Run occasionally to remove intermediate docker cruft
#
# Copyright (c) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Delete all stopped containers
docker rm $(docker ps -a -q)

# Delete all untagged images
docker rmi $(docker images -q -f dangling=true)
