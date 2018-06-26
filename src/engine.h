#ifndef ENGINE_H
#define ENGINE_H

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxEditableSVG.h"
#include "ofxCV.h"

class engine
{
public:
    engine();
    void setup();
    void update();
    void draw(int x, int y, int w, int h);

    ofImage input, origInput;
    ofImage maskInput, origMaskInput;
    ofImage maskGrid, origMaskGrid;
    ofImage maskPoints, origMaskPoints;
    ofImage backgroundInput, origBackgroundInput;
    ofMesh pathInput, pathGrid, pathPoints;
    ofFbo fboInput, fboGrid, fboPoints;
    ofFbo canvas;
    ofFbo grid, points;
    ofFbo background;
    vector<ofPoint> triangles;
    int width, height;
    ofColor colorTriangle, colorOne, colorTwo, colorPoint, colorSVG, colorMaskPoint;
    float min, max, density, noise, prob, lineWidth, rotation, pointSize, opacityImg, opacityGrid, opacityPoints;
    float minP, maxP, densityP, noiseP, levelMsk, angleBackground;
    bool needsCalculateGrid;
	bool needsUpdateGrid;
    bool needsCalculatePoints;
    bool needsUpdatePoints;
	bool needsUpdateMask;
    bool showInput;
    bool showGrid;
    bool showPoints;
    bool showBackgroundFile;
    bool showBackground;
    bool showBackgroundColor;
    bool showTextures;
    bool saveVector;
    bool definingMaskImg, definingMaskGrid, definingMaskPoints, definingSvgCenter;
    bool coloringMaskImg, coloringMaskGrid, coloringMaskPoints;
    int shapeDrawing;
	ofxEditableSVG shapefile;
    ofxDelaunay triangulation;
    ofShader shaderAlpha;
    vector<ofxEditableSVG> svgTextures;
    int numSVG;
	float svgSize;
    ofPoint centerSVG;
    ofPath pathSvgGrid, pathSvgPoints;


    void setResolution(int width_, int height_);
    void setInput(string file);
    void setMask(string file);
    void setMaskGrid(string file);
    void setMaskPoints(string file);
	void setShape(string file);
    void deleteImg();
    void deleteMask();
    void deleteMaskGrid();
    void deleteMaskPoints();
    void setBackground(string file);
    void calculateGrid();
    void calculatePoints();
	void updateMask();
	void updateGrid();
    void updatePoints();
    void drawVectors(string path);
    void updateBackground();
	ofImage blur(ofImage img, int radio);
    void backgroundGradient(const ofColor& start, const ofColor& end);


};

#endif // ENGINE_H
