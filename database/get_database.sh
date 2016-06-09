#!/bin/bash

GETCMD=0

if hash wget 2>/dev/null; then
  GETCMD="wget"
elif hash curl 2>/dev/null; then
  GETCMD="curl -O"
else
  echo >&2 "I require wget or curl but they're not installed. Aborting."
  exit 1
fi

# Get decos database from dropbox
$GETCMD https://dl.dropboxusercontent.com/u/61644344/database.tar.bz2
hash tar 2>/dev/null || { echo >&2 "I require bunzip2 but it's not installed. Aborting."; exit 1; }
tar -xjf database.tar.bz2;
rm database.tar.bz2

echo "Done."
