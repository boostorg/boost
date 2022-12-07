#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

if [ "$JOB_UUID" = "92cfceb39d" ] || [ "$JOB_UUID" = "0286dd552c" ] ; then
    brew install ccache || true;

fi

