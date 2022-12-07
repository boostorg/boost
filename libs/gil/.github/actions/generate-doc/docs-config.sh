#!/bin/bash
set -e # Exit with Non Zero exit Code

mkdir temp-doc
cd temp-doc

git init


git remote add upstream "https://github.com/boostorg/gil.git"

git fetch upstream
git switch gh-pages


if [ "${GITHUB_REF##*/}" = develop ]; then
    # Only updates develop directory and keeps others intact
    rm -r develop
    mkdir -p develop/doc
    cp ../index.html develop/
    cp ../doc/index.html develop/doc
    cp -a ../doc/html develop/doc/
else
    # main branch
    rm index.html
    rm -r html
    cp ../doc/index.html .
    cp -r ../doc/html .
fi

# Remove version control
rm -rf .git