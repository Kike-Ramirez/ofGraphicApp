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
	colors.push_back(ofColor(177, 228, 227));
	colors.push_back(ofColor(107, 164, 184));
	colors.push_back(ofColor(144, 40, 130));
	colors.push_back(ofColor(255, 177, 187));
	colors.push_back(ofColor(220, 134, 153));
	colors.push_back(ofColor(255, 141, 109));
	colors.push_back(ofColor(99, 198, 191));
	colors.push_back(ofColor(0, 45, 66));

	colors.push_back(ofColor(65, 12, 55));
	colors.push_back(ofColor(238, 33, 87));
	colors.push_back(ofColor(166, 9, 61));
	colors.push_back(ofColor(238, 39, 55));
	colors.push_back(ofColor(248, 224, 142));
	colors.push_back(ofColor(80, 166, 132));

	colors.push_back(ofColor(215, 210, 203));
	colors.push_back(ofColor(255, 205, 0));
	colors.push_back(ofColor(0, 104, 94));
	colors.push_back(ofColor(39, 37, 31));

	backColor = ofColor(0);
	lineColor = ofColor(255, 255, 0);
	fontColor = ofColor(255);

	font.loadFont("ofxbraitsch/fonts/DINNextLTPro-MediumCond.otf", 10);

}

void ColorSelector::mousePressed(int x, int y, int button)
{
	float rWidth = 0.8 * size.x * 2 / colors.size();
	selectedColor = ofColor(0);

	for (int i = 0; i < colors.size(); i++) {

		int ii = i - colors.size() / 2;

		if (i < colors.size() / 2) {

			if (x > (position.x + 0.2 * size.x + i * rWidth + 0.1 * rWidth) &&
				x <= (position.x + 0.2 * size.x + i * rWidth + 0.9 * rWidth) &&
				y > (position.y + 0.05 * size.y) &&
				y <= (position.y + 0.45 * size.y)) {

				selectedColor = colors[i];
			}

		}

		else {

			if (x > (position.x + 0.2 * size.x + ii * rWidth + 0.1 * rWidth) &&
				x <= (position.x + 0.2 * size.x + ii * rWidth + 0.9 * rWidth) &&
				y > (position.y + 0.55 * size.y) &&
				y <= (position.y + 0.95 * size.y)) {

				selectedColor = colors[i];
			}
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

	float rWidth = 0.8 * size.x * 2 / colors.size();

	for (int i = 0; i < colors.size(); i++) {
	
		ofSetColor(colors[i]);
		int ii = i - colors.size() / 2;

		if (i < colors.size() / 2) {
			ofDrawRectangle(position.x + i * rWidth + 0.2 * size.x + 0.1 * rWidth, position.y + 0.05 * size.y, 0.8 * rWidth, 0.4 * size.y);
		}

		else {
			ofDrawRectangle(position.x + ii * rWidth + 0.2 * size.x + 0.1 * rWidth, position.y + 0.55 * size.y, 0.8 * rWidth, 0.4 * size.y);
		}
	}
	ofSetColor(255);
}

