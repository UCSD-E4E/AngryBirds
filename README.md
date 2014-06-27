Angry Birds
=========

Core Contributors
=================
  - Angela To
  - Dustin Mendoza
  - Traci Takasugi
  - Luke DeLuccia

Objectives
==========

Angry Birds is a project which aims to do the following:
asdfasdf
  - Detect if a bird has flown into a window
  - Record video footage of the crash
  - Record other data such as speed and/or force
  - Upload data to a database for further examination

Version
-------

1.0 - Starting version of project

Detailed Specifications
-----------------------

Tech
-----------

Angry Birds uses a number of open source projects and tech to work properly:

* [Dillinger] - awesome online markdown editor
* [SublimeText2] - awesome editor with code folding and other good stuff
* [Beagle Bone Black] - Awesome microcontroller
* [Ubuntu] - 14.04 
* [OpenCV] - Computer Vision used to store the images

Installation
--------------

On the Beagle Bone Board update the repos (it WILL take a while and look like it stalls but don't worry, it's updating)

```sh
sudo apt-get update
git clone [git-repo-url] angrybirds
cd angrybirds
npm i -d
mkdir -p public/files/{md,html,pdf}
```

##### Configure Plugins. Instructions in following README.md files

* plugins/dropbox/README.md
* plugins/github/README.md
* plugins/googledrive/README.md

```sh
node app
```

TODO
====
- [ ] Update installation instructions
- [ ] Add in images for project
- [ ] Purchase piezo electric sensors
- [ ] 

License
----

MIT


**Free Software, Hell Yeah!**

[Beagle Bone Black]:http://beagleboard.org/Products/BeagleBone+Black
[Dillinger]:http://dillinger.io/
[SublimeText2]:http://www.sublimetext.com/
[OpenCV]:http://opencv.org/