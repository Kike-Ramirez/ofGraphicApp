#ifndef ENGINE_H
#define ENGINE_H

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxEditableSVG.h"
#include "ofxCV.h"
#include "ofxGpuThicklines.h"

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
    bool needsUpdateGrid;
	bool needsUpdateDrawGrid;
    bool needsUpdateDrawPoints;
    bool needsDrawPoints;
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
	ofxGpuThicklines gpuTriangulation;
    ofShader shaderAlpha, shaderThick;
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
    void updateGrid();
    void updatePoints();
	void updateMask();
	void drawGrid();
    void drawPoints();
    void drawVectors(string path);
    void updateBackground();
	ofImage blur(ofImage img, int radio);
    void backgroundGradient(const ofColor& start, const ofColor& end);


};

#endif // ENGINE_H
