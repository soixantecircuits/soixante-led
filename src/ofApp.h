#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxOPC.h"

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

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
		ofxTimeline timeline;
    void initTimeline();
    void initVideo();
		void bangFired(ofxTLBangEventArgs& args);
    void loadVideo(string videoPath);
    void addVideoTracksInPages(string videoPath);
    void addColorTracks();
    bool loaded;
    ofxXmlSettings settings;
    ofRectangle contentRectangle; //matched to video size
    ofRectangle outputRectangle;
    bool mute;
    void initRings();
    void drawRings();

    vector <Led> leds;
    ofxOPC opcClient;

};
