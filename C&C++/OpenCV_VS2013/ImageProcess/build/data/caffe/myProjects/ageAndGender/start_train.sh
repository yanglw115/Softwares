#!/usr/bin/env sh
ProjectDir=myProjects/ageAndGender
solver=${ProjectDir}/data/gender_solver.prototxt

echo "Start train..."

caffe train -solver ${solver}
