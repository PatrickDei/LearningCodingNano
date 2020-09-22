#!/bin/bash

stpryGameDir=$(pwd | rev | cut -d'/' -f3- | rev)'/StoryGameIOS'
find $stpryGameDir -name '*.png' -exec ./pngquant 256 --ext=.png --f --skip-if-larger --v {} \;