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

# Get the neural network
$GETCMD http://openface-models.storage.cmusatyalab.org/nn4.v2.t7

# Get face shape for dlib
$GETCMD http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
hash bunzip2 2>/dev/null || { echo >&2 "I require bunzip2 but it's not installed. Aborting."; exit 1; }
bunzip2 shape_predictor_68_face_landmarks.dat.bz2;

# Get front face haarcascade classifier
$GETCMD https://raw.githubusercontent.com/Itseez/opencv/master/data/haarcascades/haarcascade_frontalface_alt.xml

echo "Done."
