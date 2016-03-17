
#pragma once
#include "ofMain.h"
#include "Led.h"

class Led {
  public:
    Led (ofVec2f pos, int index){
      this->pos = pos;
      this->index = index;
    }
    ofVec2f pos;
    ofColor c;
    int index;
};
