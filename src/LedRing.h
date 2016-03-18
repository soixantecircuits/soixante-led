
#pragma once
#include "ofMain.h"
#include "Led.h"

class LedRing {
  public:
    vector <Led> leds;

    //--------------------------------------------------------------
    void draw()
    {
        // dessine les cercles
        for (int i = 0; i < leds.size(); i++)
        {
            ofFill();
            ofSetColor(leds[i].c);
            ofDrawCircle(leds[i].pos.x, leds[i].pos.y, 4);
        }
    }

    //--------------------------------------------------------------
    void black()
    {
      // todo: add this as a method of class ring
      for (int i = 0; i < leds.size(); i++)
      {
          leds[i].c = ofColor::black;
      }
    } 
};
