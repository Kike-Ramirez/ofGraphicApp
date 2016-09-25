#include "engine.h"

engine::engine()
{

    width = 1024;
    height = 768;
    density = 10;
    noise = 0;
    prob = 0.5;
    lineWidth = 1;
    pointSize = 3;


}

void engine::setup()
{

    low = ofColor(0);
    high = ofColor(255);
    colorOne = ofColor(0);
    colorTwo = ofColor(255);

    setResolution(width, height);
    needsUpdate = true;
    showInput = true;
    showBackground = false;
    showBackgroundFile = false;
    shapeDrawing = 1;
    updateBackground();

}

void engine::updateBackground()
{

    background.begin();
    ofClear(0);
    if (showBackground) backgroundGradient(colorOne, colorTwo);
    else if (showBackgroundFile) backgroundInput.draw(0,0);
    background.end();

}

void engine::update()
{

    if (needsUpdate) updateGrid();

    canvas.begin();
    ofClear(ofFloatColor(0));

    if (showBackground || showBackgroundFile) background.draw(0,0);

    float ratioInput = input.getWidth()/input.getHeight();
    float ratioCanvas = canvas.getWidth()/canvas.getHeight();

    if ((showInput) && (input.isAllocated())) {

        input.draw(0,0);

    }

    grid.draw(0,0);

    canvas.end();

}

void engine::draw(int x, int y, int w, int h)
{

    canvas.draw(x, y, w, h);

}

void engine::setResolution(int width_, int height_)
{
    width = width_;
    height = height_;

    canvas.allocate(width, height);
    grid.allocate(width, height);
    background.allocate(width, height);

    if (input.isAllocated()) {

        input.clone(origInput);
        float ratioInput = float(input.getWidth())/input.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioInput) {

            input.resize(width, width / ratioInput);
        }

        else {

            input.resize(height * ratioInput, height);


        }
    }

    if (backgroundInput.isAllocated()) {

        backgroundInput.clone(origBackgroundInput);

        float ratioBackground = float(backgroundInput.getWidth())/backgroundInput.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioBackground) {

            backgroundInput.resize(width, width / ratioBackground);
        }

        else {

            backgroundInput.resize(height * ratioBackground, height);


        }

    }



}

void engine::setInput(string file_)
{
    origInput.load(file_);
    input.clone(origInput);
    setResolution(input.getWidth(), input.getHeight());

}

void engine::setBackground(string file)
{
    origBackgroundInput.load(file);
    backgroundInput.clone(origBackgroundInput);
    setResolution(width, height);

}

void engine::updateGrid()
{

    grid.begin();
    ofClear(ofColor(0, 0));

    ofSetColor(255);

    triangles.clear();
    triangulation.reset(input);

    for (int i = 0; i < input.getWidth(); i+=density) {

        for(int j = 0; j < input.getHeight(); j += density) {

            ofPoint punto(i + ofRandom(-noise, noise), j + ofRandom(-noise, noise));
            if ((punto.x < 0) || (punto.x > input.getWidth())) punto.x = i;
            if ((punto.y < 0) || (punto.y > input.getHeight())) punto.y = j;

            float lightnessPoint = input.getColor(punto.x, punto.y).getLightness();

            if ((lightnessPoint > low.getLightness()) && (lightnessPoint < high.getLightness()))
                {
                    // cout << "Low Lightness: " << low.getLightness() << endl;
                    if (ofRandom(255) < lightnessPoint )
                    {
                        triangles.push_back(punto);
                        triangulation.addPoint(punto);
                    }
                }
        }
    }

    triangulation.triangulate();
    ofNoFill();
    ofSetColor(colorTriangle);
    ofSetLineWidth(lineWidth);

    triangulation.draw();

    // Dibujamos puntos

    if (shapeDrawing == 1) {
        ofFill();
        for (int i = 0; i < triangles.size(); i++ ) {
            ofPoint punto = ofPoint(triangles[i].x, triangles[i].y);
            ofSetColor(input.getColor(punto.x, punto.y));
            float radio = ofMap(input.getColor(punto.x, punto.y).getLightness(), 0, 255, 0, pointSize);
            ofDrawEllipse(triangles[i].x, triangles[i].y, radio, radio);
        }
        ofNoFill();
    }

    // Dibujamos cuadrados

    else if (shapeDrawing == 2) {
        ofFill();
        for (int i = 0; i < triangles.size(); i++ ) {
            ofPoint centro = ofPoint(triangles[i].x, triangles[i].y);
            ofSetColor(input.getColor(centro.x, centro.y));
            float lado = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, 0, pointSize);
            float angle = ofRandom(0, PI);
            ofDrawRectangle(centro.x - lado/2, centro.y - lado/2, lado, lado);
        }
        ofNoFill();
    }

    // Dibujamos triangulos

    else if (shapeDrawing == 3) {
        ofFill();
        for (int i = 0; i < triangles.size(); i++ ) {
            ofPoint centro = ofPoint(triangles[i].x, triangles[i].y);
            ofSetColor(input.getColor(centro.x, centro.y));
            float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, 0, pointSize);
            float angle = ofRandom(0, PI);
            ofPoint punto1(centro.x + radio * cos(angle), centro.y + radio * sin(angle));
            ofPoint punto2(centro.x + radio * cos(angle + 2*PI/3), centro.y + radio * sin(angle + 2*PI/3));
            ofPoint punto3(centro.x + radio * cos(angle + 4*PI/3), centro.y + radio * sin(angle + 4*PI/3));
            ofDrawTriangle(punto1, punto2, punto3);
        }
        ofNoFill();
    }

    grid.end();
    needsUpdate = false;
}

void engine::backgroundGradient(const ofColor& start, const ofColor& end) {
    float w = background.getWidth(), h = background.getHeight();
    ofMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);


    gradientMesh.addVertex({0.f, 0.f, 0.f});
    gradientMesh.addVertex({w, 0.f, 0.f});
    gradientMesh.addVertex({w, h, 0.f});
    gradientMesh.addVertex({0.f, h, 0.f});
    gradientMesh.addColor(start);
    gradientMesh.addColor(start);
    gradientMesh.addColor(end);
    gradientMesh.addColor(end);

    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}
