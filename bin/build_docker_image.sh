#!/bin/bash

set -e

if [[ $# -eq 0 ]]; then
    RUBY_VERSION=2.6.3
else
    RUBY_VERSION=$1
fi

docker build -t edn_turbo_image:$RUBY_VERSION .
