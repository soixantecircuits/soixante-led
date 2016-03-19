
#pragma once
#include "ofMain.h"
#include "Led.h"
#include "ofxTimeline.h"

class Chase {
  public:
    Chase (){
      startIndex = 0;
      endIndex = 1;
      length = 1;
      speed = 10;
      color = ofColor::white;
      pos = 0;
      linked = 1; 
      clockwise = 1;
    };
    void setLeds(vector<Led> & leds){
      this->leds = & leds;
    };
    void setStartIndex(int startIndex){
      this->startIndex = startIndex;
    };
    void setEndIndex(int endIndex){
      this->endIndex = endIndex;
    };
    void setChaseLength(int length){
      this->length = length;
    };
    void setSpeed(float speed){
      this->speed = speed;
    };
    void setLinked(int linked){
      this->linked = linked;
    };
    void setClockwise(int clockwise){
      this->clockwise = clockwise;
    };
    void setColor(ofColor color){
      this->color = color;
    };
    void setTimeline(ofxTimeline & timeline){
      this->timeline = &timeline;
    };
    // deprecated
    void updateSimple(){
      pos += speed/100.;
      pos = fmod(pos, getLength());
      for (int i = 0; i < length; i++){
        int index = startIndex + fmod( i + floor(pos),getLength());
        (*leds)[index].c = color;
      }
    };
    void update(){
      if (timeline != NULL && timeline->getIsPlaying()){
        pos += clockwise*speed/100.;
      }
      
      //pos = ofMap(timeline->getCurrentTimeMillis(), 0, 1000000./speed, 0, endIndex - startIndex);
      pos = fmod(getLength() + pos, getLength());
      //pos = timeline->getValue("lfo") * (endIndex - startIndex);
      float gaussian_intensity = 100;
      float sd = length;
      for (int i = 0; i < getLength(); i++){
        // for the stripes, control them all together
        int index = startIndex + fmod( i + floor(pos), getLength());
        float value = gaussian(index, pos, sd)*gaussian_intensity/100.;
        value += gaussian(index, pos + getLength(), sd)*gaussian_intensity/100.;
        value += gaussian(index, pos - getLength(), sd)*gaussian_intensity/100.;
        for (int j = 0; j < linked; j++){
          int index_ = index*linked + j;
          if (index_ < 0 || index_ >= leds->size()){
            ofLogNotice("Something went wrong with this index: " + ofToString(index_));
            break;
          }
          (*leds)[index_].c = color;
          (*leds)[index_].c.setBrightness(value*color.getBrightness());
        }
      }
    };
    float gaussian(float x, float mean, float variance) {  
      float dx = x - mean;  
      //return (1.f / sqrtf(TWO_PI * variance)) * expf(-(dx * dx) / (2 * variance));  
      return expf(-(dx * dx) / (2 * variance));  
    };
    int getLength(){
      return (endIndex - startIndex)/linked;
    };
    void setBrightness(float b){
      for (int i = startIndex; i < endIndex; i++){
          (*leds)[i].c.setBrightness(b*color.getBrightness());
      }
    };

  private:
    vector<Led> *leds;
    int startIndex;
    int endIndex;
    int length;
    float speed;
    ofColor color;
    float pos;
    ofxTimeline* timeline;
    int linked;
    int clockwise;
};
