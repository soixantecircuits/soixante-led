#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "MyofxOPC.h"
#include "Led.h"
#include "Chase.h"


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
		void bangFired(ofxTLBangEventArgs& args);
    bool loaded;
    ofxXmlSettings settings;
    ofRectangle contentRectangle; //matched to video size
    ofRectangle outputRectangle;
    void initRings();
    void drawRings();
    void initChase();
    void updateOPC();

    vector <Led> leds;
    MyofxOPC opcClient;
    Chase chase;

};
