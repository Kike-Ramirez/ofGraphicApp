#pragma once

#include "ofMain.h"
#include "engine.h"
#include "ofxDatGui.h"
#include "ofxDelaunay.h"
#include "ofxCV.h"

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
    ofxDatGuiToggle* uploadBackground;
    ofxDatGuiToggle* defineBackground;
    ofxDatGuiToggle* shapeCircle;
    ofxDatGuiToggle* shapeSquare;
    ofxDatGuiToggle* shapeTriangle;
    ofxDatGuiToggle* shapeFile;
    ofShader shaderBlurX, shaderBlurY;
    ofFbo fboBlurOnePass, fboBlurTwoPass;

    int dWidth, dHeight;
    float xCanvas, yCanvas, widthCanvas, heightCanvas;

    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    ofImage blur(ofImage img, int radio);

};
