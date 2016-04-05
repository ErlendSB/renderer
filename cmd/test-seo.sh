#!/bin/bash

bin/seo-generator -flagfile flags/seo-generator https://dl.dropboxusercontent.com/u/6706259/black.html | bin/seo -flagfile flags/seo
# | bin/seo-generator -flagfile flags/seo-generator http://www.google.com
# | bin/seo-reader -flagfile flags/seo-reader
