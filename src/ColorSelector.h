#pragma once

#include "ofMain.h"


class ColorSelector
{
public:
	ColorSelector();

	ofVec3f position;
	ofVec3f size;
	vector<ofColor> colors;
	ofColor backColor;
	ofColor lineColor;
	ofColor fontColor;
	ofColor selectedColor;
	ofTrueTypeFont font;
	int numColors;

	void setup(ofVec3f position_, ofVec3f size_);
	void mousePressed(int x, int y, int button);
	void display();

};

