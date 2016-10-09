#ifndef ENGINE_H
#define ENGINE_H

#include "ofMain.h"
#include "ofxDelaunay.h"

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
    ofFbo canvas;
    ofFbo grid;
    ofFbo background;
    vector<ofPoint> triangles;
    int width, height;
    ofColor colorTriangle, colorOne, colorTwo;
    float min, max, density, noise, prob, lineWidth, rotation, pointSize;
    bool needsUpdateGrid;
    bool needsUpdatePoints;
    bool showInput;
    bool showBackgroundFile;
    bool showBackground;
    bool showMasking;
    int shapeDrawing;
    ofxDelaunay triangulation;

    void setResolution(int width_, int height_);
    void setInput(string file);
    void setMask(string file);
    void setMaskGrid(string file);
    void setMaskPoints(string file);
    void deleteMask();
    void setBackground(string file);
    void updateGrid();
    void drawPoints();
    void updateBackground();
    void backgroundGradient(const ofColor& start, const ofColor& end);


};

#endif // ENGINE_H
