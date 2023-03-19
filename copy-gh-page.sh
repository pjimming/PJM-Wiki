#! /bin/bash

git commit -m $1

git push

git pull

mkdocs build --clean

mkdocs gh-deploy