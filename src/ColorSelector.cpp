#include "ColorSelector.h"



ColorSelector::ColorSelector()
{
}

void ColorSelector::setup(ofVec3f position_, ofVec3f size_)
{
	position.set(position_);
	size.set(size_);

	colors.push_back(ofColor(199, 91, 18));
	colors.push_back(ofColor(77, 83, 87));
	colors.push_back(ofColor(0, 45, 66));
	colors.push_back(ofColor(221, 220, 212));
	colors.push_back(ofColor(144, 40, 130));
	colors.push_back(ofColor(172, 184, 52));
	colors.push_back(ofColor(99, 198, 191));
	colors.push_back(ofColor(238, 33, 87));
	colors.push_back(ofColor(54, 114, 55));
	colors.push_back(ofColor(65, 12, 56));

	backColor = ofColor(0);
	lineColor = ofColor(255, 255, 0);
	fontColor = ofColor(255);

	font.loadFont("ofxbraitsch/fonts/DINNextLTPro-MediumCond.otf", 10);

}

void ColorSelector::mousePressed(int x, int y, int button)
{
	float rWidth = 0.8 * size.x / colors.size();
	selectedColor = ofColor(0);

	for (int i = 0; i < colors.size(); i++) {
	
		if (x > (position.x + 0.2 * size.x + i * rWidth + 0.1 * rWidth) &&
			x <= (position.x + 0.2 * size.x + i * rWidth + 0.9 * rWidth) &&
			y > (position.y + 0.1 * size.y) &&
			y <= (position.y + 0.9 * size.y)) {

			selectedColor = colors[i];
		}
	}

}

void ColorSelector::display()
{
	ofSetColor(backColor);
	ofFill();;
	ofDrawRectangle(position, size.x, size.y);
	ofSetColor(lineColor);
	ofDrawRectangle(position, 2, size.y);

	float rWidth = 0.8 * size.x / colors.size();

	for (int i = 0; i < colors.size(); i++) {
	
		ofSetColor(colors[i]);
		ofDrawRectangle(position.x + i * rWidth + 0.2 * size.x + 0.1 * rWidth, position.y + 0.1 * size.y, 0.8 * rWidth, 0.8 * size.y);

	}
	ofSetColor(255);
}

