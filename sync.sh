#!/bin/bash

git fetch upstream && \
git checkout main && \
git merge upstream/main && \
git checkout gme && \
git rebase main

