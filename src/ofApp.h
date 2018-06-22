#pragma once

#include "ofMain.h"
#include "engine.h"
#include "ofxDatGui.h"
#include "ofxDelaunay.h"
#include "ofxXmlSettings.h"
#include "ColorSelector.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void loadGui();
    void fitCanvas();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    engine myengine;

	ofxDatGui* gui;
	void toggleFullscreen();
	vector<ofxDatGuiTheme*> themes;
	vector<ofColor> backgrounds;
	vector<ofColor> corporativeColors;
	vector<string> corporativeColorNames;
	int tIndex;




	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);


	ofShader shaderBlurX, shaderBlurY;
    ofFbo fboBlurOnePass, fboBlurTwoPass;
	ofxXmlSettings xmlParameters;
	string pathImg;
	string pathMskImg;
	string pathMskGrid;
	string pathMskPoints;
	string pathBackground;
	string pathShape;
	string pathProject;

	int numSVG;

    int dWidth, dHeight;
    float xCanvas, yCanvas, widthCanvas, heightCanvas;
	bool mFullscreen;

	void resetSettings();
	void loadSettings();
	void saveSettings();
	void updateValues();
	void changeTheme();
	

};
