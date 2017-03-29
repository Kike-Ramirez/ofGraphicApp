#pragma once

#include "ofMain.h"
#include "engine.h"
#include "ofxDatGui.h"
#include "ofxDelaunay.h"
#include "ofxCV.h"
#include "ofxXmlSettings.h"

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
    vector<ofxDatGuiComponent*> components;
    ofxDatGuiComponent* component;
    ofxDatGuiTextInput* ancho;
    ofxDatGuiTextInput* alto;
	ofxDatGuiToggle* showInput;


	ofxDatGuiSlider* opacityImg;
	ofxDatGuiSlider* levelMsk;
	ofxDatGuiToggle* showGrid;
	ofxDatGuiSlider* opacityGrid;
	ofxDatGuiSlider* min;
	ofxDatGuiSlider* max;
	ofxDatGuiSlider* density;
	ofxDatGuiSlider* noise;
	ofxDatGuiSlider* stroke;
	ofxDatGuiColorPicker* colorGrid;
	ofxDatGuiToggle* showPoints;
	ofxDatGuiSlider* opacityPoints;
	ofxDatGuiSlider* minP;
	ofxDatGuiSlider* maxP;
	ofxDatGuiSlider* densityP;
	ofxDatGuiSlider* noiseP;
	ofxDatGuiSlider* size;
	ofxDatGuiToggle* punto;
	ofxDatGuiToggle* cuadrado;
	ofxDatGuiToggle* triangulo;
	ofxDatGuiToggle* archivo;
	ofxDatGuiColorPicker* color;
	ofxDatGuiToggle* graphicElements;
	ofxDatGuiToggle* loadBackground;
	ofxDatGuiToggle* colorBackground;
	ofxDatGuiToggle* defineBackground;
	ofxDatGuiSlider* angleBackground;
	ofxDatGuiColorPicker* colorOne;
	ofxDatGuiColorPicker* colorTwo;

	ofShader shaderBlurX, shaderBlurY;
    ofFbo fboBlurOnePass, fboBlurTwoPass;
	ofxXmlSettings xmlParameters;
	string pathImg;
	string pathMskImg;
	string pathMskGrid;
	string pathMskPoints;
	string pathBackground;
	int numSVG;

    int dWidth, dHeight;
    float xCanvas, yCanvas, widthCanvas, heightCanvas;

    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    ofImage blur(ofImage img, int radio);
	void resetSettings();
	void loadSettings();
	void saveSettings();
	void updateValues();
	

};
