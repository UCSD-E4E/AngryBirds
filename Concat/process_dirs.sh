#! /bin/bash

path=/home/ubuntu/AngryBirds/SDCard/images/
script=/home/ubuntu/AngryBirds/Concat/concat_test.sh

echo "Attempting to concat images into avi"
find ${path:?} -type d | while read dir; do
     echo "Processing dir: $dir"
     echo " "
     cd $dir
     now=$(date +"%m-%d-%Y_%H.%M.%S")
     echo "$now"
     echo " "
     id="/home/ubuntu/AngryBirds/SDCard/videos/$now.avi"
     echo "$id"
     echo " "
     sudo avconv -r 20 -i "$dir"/img_000.jpg -b:v 1000k "$id"
     sleep 1 # Sleep to prevent files from being overwritten (same timestamp name)
done
echo "Done"
