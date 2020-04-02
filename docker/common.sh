#!/bin/sh

RUBY_MAJOR_VER=2.7

function update_ruby_build()
{
    echo "Updating ruby-build definitions"
    pushd ~/.rbenv/plugins/ruby-build/ >> /dev/null
    git pull
    popd >> /dev/null
}

function set_ruby_ver()
{
    local args=("$@")

    if [ ${#args[@]} -eq 0 ]; then
        update_ruby_build
        RUBY_VERSION=`ruby-build --definitions | grep  "^$RUBY_MAJOR_VER" | tail -1`
    else
        RUBY_VERSION=$1
    fi

    echo "Using ruby $RUBY_VERSION"
}
