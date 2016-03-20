
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
      pingpong = false;
      lfo = false;
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
    void setLfo(int index){
      lfo = true;
      this->lfoIndex = index;
    };
    void setPingpong(bool pingpong){
      this->pingpong = pingpong;
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
        if (pingpong){
          if (pos >= getLength()){
            clockwise = -1;
          }
          if (pos <= 0){
            clockwise = 1;
          }
        }
        else {
          pos = fmod(getLength() + pos, getLength());
        }
      }
      if (lfo){
        pos = timeline->getValue("lfo_" + ofToString(lfoIndex)) * (endIndex - startIndex);
      }
      
      //pos = ofMap(timeline->getCurrentTimeMillis(), 0, 1000000./speed, 0, endIndex - startIndex);
      float gaussian_intensity = 100;
      float sd = length;
      for (int i = 0; i < getLength(); i++){
        //int index = startIndex + fmod( i + floor(pos), getLength());
        int index = i;
        float value = gaussian(index, pos, sd)*gaussian_intensity/100.;
        value += gaussian(index, pos + getLength(), sd)*gaussian_intensity/100.;
        value += gaussian(index, pos - getLength(), sd)*gaussian_intensity/100.;
        // for the stripes, control them all together
        for (int j = 0; j < linked; j++){
          int index_ = startIndex + index*linked + j;
          if (index_ < 0 || index_ >= leds->size()){
            ofLogNotice("Something went wrong with this index: " + ofToString(index_));
            break;
          }
          ofColor c = color;
          c.setBrightness(value*c.getBrightness());
          (*leds)[index_].c.setBrightness((*leds)[index_].c.getBrightness() + c.getBrightness() );
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
        color.setBrightness(b*255);
    };
    void fullLine(){
      //color.setBrightness(b*255*color.getBrightness());
      for (int i = startIndex; i < endIndex; i++){
          (*leds)[i].c = color;
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
    bool pingpong;
    bool lfo;
    int lfoIndex;
};
