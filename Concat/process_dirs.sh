#! /bin/bash

path=/home/ubuntu/AngryBirds/SDCard/images/
script=/home/ubuntu/AngryBirds/Concat/concat_test.sh

echo "Attempting to concat images into avi"
find ${path:?} -type d | while read dir; do
    (cd $dir && sh "$script")
done
echo "Done"
