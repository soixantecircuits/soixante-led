#!/bin/bash

# Dependencies for ofxTimeline

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}YCAMInterlab/ofxTimecode.git

${PREFIX}obviousjim/ofxTween.git

${PREFIX}obviousjim/ofxMSATimer.git

${PREFIX}elliotwoods/ofxTextInputField.git

${PREFIX}Flightphase/ofxRange.git

${PREFIX}emmanuelgeoffray/ofxTimeline.git -b bugfix-in-out-startup 

${PREFIX}emmanuelgeoffray/ofxOPC.git
