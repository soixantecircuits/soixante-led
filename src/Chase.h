
#pragma once
#include "ofMain.h"
#include "Led.h"

class Chase {
  public:
    Chase (){
      startIndex = 0;
      endIndex = 1;
      length = 1;
      speed = 10;
      color = ofColor::white;
      pos = 0;
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
    void setColor(ofColor color){
      this->color = color;
    };
    void updateSimple(){
      pos += speed/100.;
      pos = fmod(pos, endIndex - startIndex);
      for (int i = 0; i < length; i++){
        int index = startIndex + fmod( i + floor(pos), endIndex - startIndex);
        (*leds)[index].c = color;
      }
    };
    void update(){
      pos += speed/100.;
      pos = fmod(pos, endIndex - startIndex);
      float gaussian_intensity = 100;
      float sd = length;
      for (int i = 0; i < endIndex - startIndex; i++){
        int index = startIndex + fmod( i + floor(pos), endIndex - startIndex);
        float value = gaussian(index, pos, sd)*gaussian_intensity/100.;
        value += gaussian(index, pos + (endIndex - startIndex), sd)*gaussian_intensity/100.;
        value += gaussian(index, pos - (endIndex - startIndex), sd)*gaussian_intensity/100.;
        (*leds)[index].c = color;
        (*leds)[index].c.setBrightness(value*color.getBrightness());
      }
    };
    float gaussian(float x, float mean, float variance) {  
      float dx = x - mean;  
      //return (1.f / sqrtf(TWO_PI * variance)) * expf(-(dx * dx) / (2 * variance));  
      return expf(-(dx * dx) / (2 * variance));  
    }  

  private:
    vector<Led> *leds;
    int startIndex;
    int endIndex;
    int length;
    float speed;
    ofColor color;
    float pos;
};
