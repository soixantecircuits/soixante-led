#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxXmlSettings.h"
#include "MyofxOPC.h"
#include "LedRing.h"
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
		ofxXmlSettings xmlSettings;
    void initTimeline();
		void bangFired(ofxTLBangEventArgs& args);
    void initRings();
    void drawRings();
    void initCurves();
    void initChases();
    void updateOPC();

    MyofxOPC opcClient;
    vector<LedRing> rings;
    vector<Chase> chases;
};
