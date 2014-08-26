#! /bin/bash

now=$(date +"%m-%d-%Y_%H.%M.%S")
echo "$now"
id="/home/ubuntu/AngryBirds/SDCard/videos/$now.avi"
echo "$id"

sudo avconv -r 20 -i /home/ubuntu/AngryBirds/SDCard/images/0/img_000.jpg -b:v 1000k "$id"
#sudo avconv -r 20 -i ./img_000.jpg -b:v 1000k "$id"
