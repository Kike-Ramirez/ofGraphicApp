#include "ofApp.h"

/*
    All components instantiated outside of a gui
    https://github.com/braitsch/ofxDatGui @braitsch
*/

void ofApp::setup()
{
    //ofSetFullscreen(true);
    ofSetWindowPosition(100, 100);
    ofSetWindowShape(1024, 768);
    ofSetWindowTitle("ofGraphicApp v0.1");
    ofSetFrameRate(60);

    dWidth = int(ofGetWidth() / 100.0);
    dHeight = int(ofGetHeight() / 100.0);

    loadGui();

    myengine.setup();
    ofEnableSmoothing();

}

void ofApp::update()
{

    for(int i=0; i<components.size(); i++) components[i]->update();
    myengine.update();

}

void ofApp::draw()
{
    ofBackgroundGradient(ofColor(0), ofColor(255), OF_GRADIENT_LINEAR);
    fitCanvas();
    for(int i=components.size()-1; i>=0; i--) components[i]->draw();
}

void ofApp::loadGui() {

    int x = dWidth;
    int y = dHeight;
    int p = 0;
    int x11 = 30 * dWidth;


    component = new ofxDatGuiFRM();
    component->setPosition(x, y);
    component->setWidth(x11, 0.7);
    components.push_back(component);

    y += component->getHeight() + p + dHeight;
    ancho = new ofxDatGuiTextInput("Ancho", "1024");
    ancho->setPosition(x, y);
    ancho->setWidth(x11, 0.7);
    ancho->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(ancho);

    y += ancho->getHeight() + p;
    alto = new ofxDatGuiTextInput("Alto", "768");
    alto->setPosition(x, y);
    alto->setWidth(x11, 0.7);
    alto->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(alto);

    y += alto->getHeight() + p + dHeight;
    component = new ofxDatGuiButton("uploadImg");
    component->setPosition(x, y);
    component->setWidth(x11 / 3, 0.7);
    component->setLabel("Archivo");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("defineMsk");
    component->setPosition(x + x11 / 3, y);
    component->setWidth(x11 / 3, 0.7);
    component->setLabel("Definir");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteMsk");
    component->setPosition(x + x11 / 3 * 2, y);
    component->setWidth(x11 / 3, 0.7);
    component->setLabel("Borrar");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiColorPicker("Low", ofColor::fromHex(0x000000));
    component->setPosition(x, y);
    component->setWidth(x11/2, 0.3);
    component->setLabel("Low");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    component = new ofxDatGuiColorPicker("High", ofColor::fromHex(0xFFFFFF));
    component->setPosition(x + x11/2, y);
    component->setWidth(x11/2, 0.3);
    component->setLabel("High");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight;
    component = new ofxDatGuiSlider("density", 4, 40, 10);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Densidad");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("noise", 0, 100, 0);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Ruido");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiToggle("showInput", true);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Mostrar Imagen");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight;
    component = new ofxDatGuiToggle("punto", false);
    component->setPosition(x, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("Punto");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("cuadrado", false);
    component->setPosition(x + x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("Cuadrado");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("triangulo", false);
    component->setPosition(x + 2 * x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("Triangulo");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("archivo", false);
    component->setPosition(x + 3 * x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("Archivo");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("size", 0, 50, 5);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Tamaño");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("stroke", 0, 10, 1);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Linea");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiColorPicker("color", ofColor::fromHex(0xFFFFFF));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Color");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight;
    uploadBackground = new ofxDatGuiToggle("loadBackground", false);
    uploadBackground->setPosition(x, y);
    uploadBackground->setWidth(x11, 0.3);
    uploadBackground->setLabel("Archivo Fondo");
    uploadBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(uploadBackground);

    y += uploadBackground->getHeight() + p;
    defineBackground = new ofxDatGuiToggle("defineBackground", false);
    defineBackground->setPosition(x, y);
    defineBackground->setWidth(x11, 0.3);
    defineBackground->setLabel("Degradado");
    defineBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(defineBackground);

    y += defineBackground->getHeight() + p;
    component = new ofxDatGuiColorPicker("colorOne", ofColor::fromHex(0x000000));
    component->setPosition(x, y);
    component->setWidth(x11 / 2, 0.6);
    component->setLabel("Color A");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    component = new ofxDatGuiColorPicker("colorTwo", ofColor::fromHex(0xFFFFFF));
    component->setPosition(x + x11/2, y);
    component->setWidth(x11/2, 0.6);
    component->setLabel("Color B");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("angle", 0, 360, 0);
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Angulo");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight;
    component = new ofxDatGuiToggle("jpg", true);
    component->setPosition(x, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("jpg");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("png", false);
    component->setPosition(x + x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("png");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("pdf", false);
    component->setPosition(x + 2 * x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("pdf");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    component = new ofxDatGuiToggle("tiff", false);
    component->setPosition(x + 3 * x11/4, y);
    component->setWidth(x11/4, 0.3);
    component->setLabel("tiff");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiButton("saveImg");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("Guardar Archivo");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight * 3;
    component = new ofxDatGuiButton("exit");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("Salir");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);


}

void ofApp::fitCanvas()
{

    float ratioCanvas = myengine.canvas.getWidth() / myengine.canvas.getHeight();
    float ratioDisplay = (ofGetWidth() - 35 * dWidth) / (ofGetHeight() - 2 * dHeight);

    if (ratioCanvas > ratioDisplay)
    {

        if (myengine.canvas.getWidth() < ofGetWidth() - 35 * dWidth)
        {
            int xCanvas = 33 * dWidth + (ofGetWidth() - 35 * dWidth - myengine.canvas.getWidth())/2;
            int yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;

            myengine.draw(xCanvas, yCanvas, myengine.canvas.getWidth(), myengine.canvas.getHeight());


        }

        else
        {
            int xCanvas = 33 * dWidth;
            int yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - float(ofGetWidth() - 35 * dWidth) / float(ratioCanvas)) / 2;

            myengine.draw(xCanvas, yCanvas, ofGetWidth() - 35 * dWidth, float(ofGetWidth() - 35 * dWidth) / float(ratioCanvas));

        }

    }

    else
    {

        if (myengine.canvas.getHeight() < ofGetHeight() - 2 * dHeight)
        {
            int xCanvas = 32 * dWidth + (ofGetWidth() - 32 * dWidth - myengine.canvas.getWidth())/2;
            int yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;

            myengine.draw(xCanvas, yCanvas, myengine.canvas.getWidth(), myengine.canvas.getHeight());

        }

        else
        {
            int xCanvas = 32 * dWidth + (ofGetWidth() - 32 * dWidth - (ofGetHeight() - 2 * dHeight) * ratioCanvas)/2;
            int yCanvas = dHeight;

            myengine.draw(xCanvas, yCanvas, (ofGetHeight() - 2 * dHeight) * ratioCanvas, ofGetHeight() - 2 * dHeight);

        }
    }


}


/*
    event listeners
*/

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;

    if (e.target->is("UploadImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          string path = result.getPath();
          myengine.setInput(path);
          myengine.needsUpdate = true;
          ancho->setText(std::to_string(myengine.input.getWidth()));
          alto->setText(std::to_string(myengine.input.getHeight()));
          myengine.updateBackground();

          // load your file at `path`
        }
    }

    else if (e.target->is("saveImg")) {

        ofFileDialogResult result = ofSystemSaveDialog("default.jpg", "Save");

        if(result.bSuccess) {
          string path = result.getPath();
          ofPixels pixels;
          myengine.canvas.readToPixels(pixels);
          ofImage output;
          output.setFromPixels(pixels);
          output.save(path, OF_IMAGE_QUALITY_BEST);
          cout << "Saved in: " << path << endl;

        }

    }

    else if (e.target->is("exit")) {

        ofExit();

    }

}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getLabel() << "::" <<  e.target->getChecked() << endl;
    if (e.target->is("showInput")) {

        myengine.showInput = e.target->getChecked();


    }

    else if (e.target->is("defineBackground")) {

        myengine.showBackground = e.target->getChecked();

        if (uploadBackground->getChecked()) {
            uploadBackground->toggle();
            myengine.showBackgroundFile = uploadBackground->getChecked();
        }
        myengine.updateBackground();


    }

    else if (e.target->is("loadBackground")) {

        myengine.showBackgroundFile = e.target->getChecked();

        if (myengine.showBackgroundFile) {

            ofFileDialogResult result = ofSystemLoadDialog("Load file");

            if(result.bSuccess) {

              string path = result.getPath();
              myengine.setBackground(path);

              // load your file at `path`
            }
        }

        if (defineBackground->getChecked()) {
            defineBackground->toggle();
            myengine.showBackground = defineBackground->getChecked();
        }

        myengine.updateBackground();


    }
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
    if (e.target->is("Density")) {

        myengine.density = e.value;
        myengine.needsUpdate = true;

    }

    else if (e.target->is("Noise")) {

        myengine.noise = e.value;
        myengine.needsUpdate = true;

    }

    else if (e.target->is("prob")) {

        myengine.prob = e.value;
        myengine.needsUpdate = true;

    }

    else if (e.target->is("size")) {

        myengine.pointSize = e.value;
        myengine.needsUpdate = true;

    }

    else if (e.target->is("stroke")) {

        myengine.lineWidth = e.value;
        myengine.needsUpdate = true;

    }

    else if (e.target->is("angle")) {

        myengine.rotation = e.value;
        myengine.updateBackground();

    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.child << endl;
}


void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.color << endl;
    if (e.target->is("Low")) {
        myengine.low = e.color;
        myengine.needsUpdate = true;
    }
    else if (e.target->is("High")) {
        myengine.high = e.color;
        myengine.needsUpdate = true;
    }
    else if (e.target->is("color")) {
        myengine.colorTriangle = e.color;
        myengine.needsUpdate = true;
    }
    else if (e.target->is("colorOne")) {
        myengine.colorOne = ofColor(e.color);
        myengine.updateBackground();
    }
    else if (e.target->is("colorTwo")) {
        myengine.colorTwo = ofColor(e.color);
        myengine.updateBackground();
    }
}


void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onButtonEvent: " << e.text << endl;
    if (e.target->is("Ancho")) {

        myengine.setResolution(std::stoi(e.text), myengine.height);
        myengine.updateBackground();
        myengine.needsUpdate = true;


    }

    else if (e.target->is("Alto")) {

        myengine.setResolution(myengine.width, std::stoi(e.text));
        myengine.updateBackground();
        myengine.needsUpdate = true;

    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
