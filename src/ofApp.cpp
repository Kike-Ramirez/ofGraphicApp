#include "ofApp.h"

/*
    All components instantiated outside of a gui - 
    https://github.com/braitsch/ofxDatGui @braitsch
*/

void ofApp::setup()
{
    //ofSetFullscreen(true);
    ofSetWindowPosition(100, 100);
    ofSetWindowShape(1024, 768);
    ofSetWindowTitle("ofGraphicApp v0.13");
    ofSetFrameRate(60);
    ofSetEscapeQuitsApp(false);

    dWidth = int(ofGetWidth() / 100.0);
    dHeight = int(ofGetHeight() / 100.0);

	// createSettings();

	xmlParameters.loadFile("settings.xml");

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

    if ((myengine.definingMaskImg) || (myengine.definingMaskGrid) || (myengine.definingMaskPoints)) {

        if ((mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {
            
            ofSetColor(255,0,0);
            ofLine(mouseX - 5, mouseY, mouseX + 5, mouseY);
            ofLine(mouseX, mouseY - 5, mouseX, mouseY + 5);
            
        }

        ofDrawBitmapString("Definiendo Mascara", xCanvas + 10, yCanvas + 20);
        ofDrawBitmapString("Cierra haciendo click en el primer punto para aplicar mascara", xCanvas + 10, yCanvas + 40);
        ofDrawBitmapString("Pulsa <Q> para salir sin aplicar", xCanvas + 10, yCanvas + 60);
        ofSetColor(255);
    }

    if (myengine.coloringMaskImg) {

        if ((mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {
            
            ofSetColor(255,0,0);
            ofLine(mouseX - 5, mouseY, mouseX + 5, mouseY);
            ofLine(mouseX, mouseY - 5, mouseX, mouseY + 5);
        
        }
        
        ofDrawBitmapString("Seleccion de Color", xCanvas + 10, yCanvas + 20);
        ofDrawBitmapString("Pulsa <A> para aceptar los cambios", xCanvas + 10, yCanvas + 40);
        ofSetColor(255);
        
    }

}

void ofApp::loadGui() {

    int x = dWidth;
    int y = dHeight;
    int p = 0;
    int x11 = 38 * dWidth;


//    component = new ofxDatGuiFRM();
//    component->setPosition(x, y);
//    component->setWidth(x11, 0.7);
//    components.push_back(component);

//    y += component->getHeight() + p + 3 * dHeight;
    component = new ofxDatGuiLabel("AREA DE TRABAJO");
    component->setPosition(x, y);
    component->setWidth(x11, 0.7);
    components.push_back(component);

    y += component->getHeight() + p;
    ancho = new ofxDatGuiTextInput("Definir Ancho", xmlParameters.getValue("settings:width", "1022"));
    ancho->setPosition(x, y);
    ancho->setWidth(x11, 0.7);
    ancho->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(ancho);

    y += ancho->getHeight() + p;
    alto = new ofxDatGuiTextInput("Definir Alto", xmlParameters.getValue("settings:height", "762"));
    alto->setPosition(x, y);
    alto->setWidth(x11, 0.7);
    alto->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(alto);

    y += alto->getHeight() + p + 4 * dHeight;
    component = new ofxDatGuiToggle("showInput", xmlParameters.getValue("settings:showInput", true));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("MOSTRAR IMAGEN");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("uploadImg");
    component->setPosition(x, y);
    component->setWidth(x11 / 2, 0.7);
    component->setLabel("Archivo");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteImg");
    component->setPosition(x + x11 / 2, y);
    component->setWidth(x11 / 2, 0.7);
    component->setLabel("Borrar");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("opacityImg", 0, 100, xmlParameters.getValue("settings:opacityImg", 100));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Opacidad");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskImg");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Mascara Imagen");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("defineMskImg");
    component->setPosition(x + x11 * 0.25, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Def. Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("colorMskImg");
    component->setPosition(x + x11 * 0.5, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Def. Color");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteMskImg");
    component->setPosition(x + x11 * 0.75, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Borrar Mascara");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("levelMsk", 0, 100, xmlParameters.getValue("settings:levelMsk", 100));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Umbral Masc. Color");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    
    y += component->getHeight() + p + 4 * dHeight;
    component = new ofxDatGuiToggle("showGrid", xmlParameters.getValue("settings:showGrid", true));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("MOSTRAR MALLA");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("opacityGrid", 0, 100, xmlParameters.getValue("settings:opacityGrid", 100));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Opacidad");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("min", 0, 255, xmlParameters.getValue("settings:min", 0));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Minimo");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("max", 0, 255, xmlParameters.getValue("settings:mask", 255));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Maximo");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("density", 4, 40, xmlParameters.getValue("settings:density", 10));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Espaciado");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("noise", 0, 100, xmlParameters.getValue("settings:noise", 0));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Desplazamiento");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("stroke", 0, 10, xmlParameters.getValue("settings:stroke", 1));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Grosor Linea");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskGrid");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.4, 0.7);
    component->setLabel("Mascara Rejilla");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("defineMskGrid");
    component->setPosition(x + x11 * 0.4, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Definir Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("deleteMskGrid");
    component->setPosition(x + x11 * 0.7, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Borrar Mascara");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiColorPicker("colorGrid", ofColor::fromHex(xmlParameters.getValue("settings:colorGrid", 0xFFFFFF)));
	cout << "SettingsXML:: " << xmlParameters.getValue("settings:colorGrid", 0xFFFFFF) << endl;
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Color");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);
    

    
    
    y += component->getHeight() + p + 4 * dHeight;
    component = new ofxDatGuiToggle("showPoints", xmlParameters.getValue("settings:showPoints", true));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("MOSTRAR PUNTOS DE MALLA");
    component->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("opacityPoints", 0, 100, xmlParameters.getValue("settings:opacityPoints", 100));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Opacidad");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("minP", 0, 255, xmlParameters.getValue("settings:minP", 0));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Minimo");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("maxP", 0, 255, xmlParameters.getValue("settings:maxP", 255));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Maximo");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("densityP", 4, 40, xmlParameters.getValue("settings:densityP", 10));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Espaciado");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("noiseP", 0, 100, xmlParameters.getValue("settings:noiseP", 0));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Desplazamiento");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("size", 0, 100, xmlParameters.getValue("settings:size", 5));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Diametro");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    shapeCircle = new ofxDatGuiToggle("punto", xmlParameters.getValue("settings:punto", true));
    shapeCircle->setPosition(x, y);
    shapeCircle->setWidth(x11/4, 0.3);
    shapeCircle->setLabel("Circulo");
    shapeCircle->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(shapeCircle);

    shapeSquare = new ofxDatGuiToggle("cuadrado", xmlParameters.getValue("settings:cuadrado", false));
    shapeSquare->setPosition(x + x11/4, y);
    shapeSquare->setWidth(x11/4, 0.3);
    shapeSquare->setLabel("Cuadrado");
    shapeSquare->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(shapeSquare);

    shapeTriangle = new ofxDatGuiToggle("triangulo", xmlParameters.getValue("settings:triangulo", false));
    shapeTriangle->setPosition(x + 2 * x11/4, y);
    shapeTriangle->setWidth(x11/4, 0.3);
    shapeTriangle->setLabel("Triangulo");
    shapeTriangle->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(shapeTriangle);

    shapeFile = new ofxDatGuiToggle("archivo", xmlParameters.getValue("settings:archivo", false));
    shapeFile->setPosition(x + 3 * x11/4, y);
    shapeFile->setWidth(x11/4, 0.3);
    shapeFile->setLabel("Archivo");
    shapeFile->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(shapeFile);

    y += shapeFile->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskPoints");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.4, 0.7);
    component->setLabel("Mascara Puntos");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("defineMskPoints");
    component->setPosition(x + x11 * 0.4, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Definir Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteMskPoints");
    component->setPosition(x + x11 * 0.7, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Borrar Masc.");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);



    y += component->getHeight() + p;
    component = new ofxDatGuiColorPicker("color", ofColor::fromHex(xmlParameters.getValue("settings:color", 0x000000)));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Color");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight + 8*dHeight;
    uploadBackground = new ofxDatGuiToggle("graphicElements", xmlParameters.getValue("settings:graficElements", false));
    uploadBackground->setPosition(x, y);
    uploadBackground->setWidth(x11 , 0.3);
    uploadBackground->setLabel("MOSTRAR ELEMENTOS GRAFICOS");
    uploadBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(uploadBackground);

    y += component->getHeight() + p;
    component = new ofxDatGuiButton("graphicRandom");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("Generar graficos");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p + 4 * dHeight;
    uploadBackground = new ofxDatGuiToggle("loadBackground", xmlParameters.getValue("settings:loadBackground", false));
    uploadBackground->setPosition(x, y);
    uploadBackground->setWidth(x11 * 0.4, 0.3);
    uploadBackground->setLabel("ARCHIVO FONDO");
    uploadBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(uploadBackground);

    colorBackground = new ofxDatGuiToggle("colorBackground", xmlParameters.getValue("settings:colorBackground", true));
    colorBackground->setPosition(x + 0.4 * x11, y);
    colorBackground->setWidth(0.3 * x11, 0.3);
    colorBackground->setLabel("COLOR");
    colorBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(colorBackground);
    
    defineBackground = new ofxDatGuiToggle("defineBackground", xmlParameters.getValue("settings:defineBackground", false));
    defineBackground->setPosition(x + 0.7 * x11, y);
    defineBackground->setWidth(0.3 * x11, 0.3);
    defineBackground->setLabel("DEGRADADO");
    defineBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(defineBackground);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("angleBackground", 0, 180, xmlParameters.getValue("settings:angleBackground", 0));
    component->setPosition(x, y);
    component->setWidth(x11, 0.3);
    component->setLabel("Angulo Degradado");
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += defineBackground->getHeight() + p;
    component = new ofxDatGuiColorPicker("colorOne", ofColor::fromHex(xmlParameters.getValue("settings:colorOne", 0x000000)));
    component->setPosition(x, y);
    component->setWidth(x11 / 2, 0.6);
    component->setLabel("Color A");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    component = new ofxDatGuiColorPicker("colorTwo", ofColor::fromHex(xmlParameters.getValue("settings:colorTwo", 0xFFFFFF)));
    component->setPosition(x + x11/2, y);
    component->setWidth(x11/2, 0.6);
    component->setLabel("Color B");
    component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(component);

    y += component->getHeight() + p + dHeight + 8*dHeight;
    component = new ofxDatGuiButton("saveImg");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.5, 1);
    component->setLabel("GUARDAR ARCHIVO BITMAP");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("saveVector");
    component->setPosition(x + 0.5 * x11, y);
    component->setWidth(x11 * 0.5, 1);
    component->setLabel("GUARDAR ARCHIVO VECTOR");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    y += component->getHeight() + p + 4 * dHeight;
    component = new ofxDatGuiButton("exit");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("SALIR");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);


}

void ofApp::fitCanvas()
{

    float x11 = 40 * dWidth;

    float ratioCanvas = myengine.canvas.getWidth() / myengine.canvas.getHeight();
    float ratioDisplay = (ofGetWidth() - x11) / (ofGetHeight() - 2 * dHeight);

    if (ratioCanvas > ratioDisplay)
    {

        if (myengine.canvas.getWidth() < ofGetWidth() - x11)
        {
            xCanvas = x11 + (ofGetWidth() - x11 - 2 * dWidth - myengine.canvas.getWidth())/2;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;
            widthCanvas = myengine.canvas.getWidth();
            heightCanvas = myengine.canvas.getHeight();
            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);


        }

        else
        {
            xCanvas = x11;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - float(ofGetWidth() - x11) / float(ratioCanvas)) / 2;
            widthCanvas = ofGetWidth() - x11 - 2 * dWidth;
            heightCanvas = float(ofGetWidth() - x11) / float(ratioCanvas);

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

        }

    }

    else
    {

        if (myengine.canvas.getHeight() < ofGetHeight() - 2 * dHeight)
        {
            xCanvas = x11 - 2 * dWidth + (ofGetWidth() - x11 - myengine.canvas.getWidth())/2;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;
            widthCanvas = myengine.canvas.getWidth();
            heightCanvas = myengine.canvas.getHeight();

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

        }

        else
        {
            xCanvas = x11 - 2 * dWidth + (ofGetWidth() - x11 - (ofGetHeight() - 2 * dHeight) * ratioCanvas)/2;
            yCanvas = dHeight;
            widthCanvas = (ofGetHeight() - 2 * dHeight) * ratioCanvas;
            heightCanvas = ofGetHeight() - 2 * dHeight;

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

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
          myengine.needsUpdateGrid = true;
            myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;
          ancho->setText(std::to_string(myengine.input.getWidth()));
          alto->setText(std::to_string(myengine.input.getHeight()));
          myengine.updateBackground();

        }
    }

    if (e.target->is("deleteImg")) {

        myengine.deleteImg();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
    }

    else if (e.target->is("uploadMskImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          string path = result.getPath();
          myengine.setMask(path);
          myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskImg")) {

        myengine.definingMaskImg = true;

    }

    else if (e.target->is("colorMskImg")) {

        myengine.coloringMaskImg = true;

    }

    else if (e.target->is("deleteMskImg")) {

        myengine.deleteMask();
        myengine.input.getTexture().disableAlphaMask();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

        while (myengine.pathInput.getNumVertices() > 0) {

            myengine.pathInput.removeVertex(0);

        }

    }

    else if (e.target->is("uploadMskGrid")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          string path = result.getPath();
          myengine.setMaskGrid(path);
          myengine.needsUpdateGrid = true;
            myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskGrid")) {

        myengine.definingMaskGrid = true;

    }

    else if (e.target->is("deleteMskGrid")) {

        myengine.deleteMaskGrid();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

        while (myengine.pathGrid.getNumVertices() > 0) {

            myengine.pathGrid.removeVertex(0);

        }

    }

    else if (e.target->is("uploadMskPoints")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          string path = result.getPath();
          myengine.setMaskPoints(path);
          myengine.needsUpdateGrid = true;
          myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskPoints")) {

        myengine.definingMaskPoints = true;

    }

    else if (e.target->is("deleteMskPoints")) {

        myengine.deleteMaskPoints();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

        while (myengine.pathPoints.getNumVertices() > 0) {

            myengine.pathPoints.removeVertex(0);

        }

    }

    else if (e.target->is("saveImg")) {

        ofFileDialogResult result = ofSystemSaveDialog("default.jpg", "Save");

        if(result.bSuccess) {
          string path = result.getPath();
            
            
          ofPixels pixels;
          myengine.canvas.readToPixels(pixels);
          pixels.setImageType(OF_IMAGE_COLOR);
            
          ofImage output;
          output.allocate(myengine.canvas.getWidth(), myengine.canvas.getHeight(), OF_IMAGE_COLOR);
          output.setFromPixels(pixels);
          output.save(path, OF_IMAGE_QUALITY_BEST);
          cout << "Saved in: " << path << endl;

        }

    }
    else if (e.target->is("saveVector")) {
        
        ofFileDialogResult result = ofSystemSaveDialog("default.svg", "Save");
        
        if(result.bSuccess) {
            string path = result.getPath();
            
            myengine.drawVectors(path);
            
            cout << "Saved in: " << path << endl;
            
        }
        
    }
    
    else if (e.target->is("graphicRandom")) {
        
        myengine.numSVG++;
        if (myengine.numSVG == myengine.svgTextures.size()) myengine.numSVG = 0;
        
        //myengine.numSVG = abs(ofRandom(myengine.svgTextures.size()));
        cout << myengine.numSVG << endl;
        myengine.centerSVG = ofPoint(ofRandom(myengine.width), ofRandom(myengine.height));
        
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
    
    else if (e.target->is("showGrid")) {
        
        myengine.showGrid = e.target->getChecked();
        myengine.needsDrawPoints = true;
        
        
    }

    else if (e.target->is("showPoints")) {
        
        myengine.showPoints = e.target->getChecked();
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("defineBackground")) {

        myengine.showBackground = e.target->getChecked();

        if (uploadBackground->getChecked()) {
            uploadBackground->toggle();
            myengine.showBackgroundFile = uploadBackground->getChecked();
        }

        if (colorBackground->getChecked()) {
            colorBackground->toggle();
            myengine.showBackgroundColor = colorBackground->getChecked();
        }
        
        myengine.updateBackground();


    }

    else if (e.target->is("colorBackground")) {
        
        myengine.showBackgroundColor = e.target->getChecked();
        
        if (uploadBackground->getChecked()) {
            uploadBackground->toggle();
            myengine.showBackgroundFile = uploadBackground->getChecked();
        }
        
        if (defineBackground->getChecked()) {
            defineBackground->toggle();
            myengine.showBackground = defineBackground->getChecked();
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
        
        if (colorBackground->getChecked()) {
            colorBackground->toggle();
            myengine.showBackgroundColor = colorBackground->getChecked();
        }
        myengine.updateBackground();


    }

    else if (e.target->is("punto")) {

        myengine.shapeDrawing = 1;
        if (shapeSquare->getChecked()) shapeSquare->toggle();
        if (shapeTriangle->getChecked()) shapeTriangle->toggle();
        if (shapeFile->getChecked()) shapeFile->toggle();
        myengine.needsDrawPoints = true;


    }

    else if (e.target->is("cuadrado")) {

        myengine.shapeDrawing = 2;
        if (shapeCircle->getChecked()) shapeCircle->toggle();
        if (shapeTriangle->getChecked()) shapeTriangle->toggle();
        if (shapeFile->getChecked()) shapeFile->toggle();
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("triangulo")) {

        myengine.shapeDrawing = 3;
        if (shapeCircle->getChecked()) shapeCircle->toggle();
        if (shapeSquare->getChecked()) shapeSquare->toggle();
        if (shapeFile->getChecked()) shapeFile->toggle();
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("archivo")) {

        myengine.shapeDrawing = 4;
        if (shapeCircle->getChecked()) shapeCircle->toggle();
        if (shapeSquare->getChecked()) shapeSquare->toggle();
        if (shapeTriangle->getChecked()) shapeTriangle->toggle();
        myengine.needsDrawPoints = true;

    }

    
    else if (e.target->is("graphicElements")) {

        myengine.showTextures = e.target->getChecked();

    }
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
    if (e.target->is("Density")) {

        myengine.density = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("Noise")) {

        myengine.noise = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("min")) {

        myengine.min = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("max")) {

        myengine.max = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    if (e.target->is("DensityP")) {
        
        myengine.densityP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("NoiseP")) {
        
        myengine.noiseP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }

    else if (e.target->is("levelMsk")) {
        
        myengine.levelMsk = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
        
        
        if ((myengine.coloringMaskImg)) {
            
            if (myengine.input.isAllocated()) {
                
                float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
                float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
                ofColor colorSelected = myengine.input.getColor(xPoint, yPoint);
                
                myengine.fboInput.begin();
                
                // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
                ofClear(0, 0, 0, 0);
                
                ofMesh points;
                points.setMode(OF_PRIMITIVE_POINTS);
                
                for (int i = 0; i < myengine.input.getWidth(); i++) {
                    
                    for (int j = 0; j < myengine.input.getHeight(); j++) {
                        
                        ofColor colorPoint = myengine.input.getColor(i,j);
                        
                        float distanciaColor = sqrt( pow(colorSelected.r - colorPoint.r, 2) + pow(colorSelected.g - colorPoint.g, 2) + pow(colorSelected.b - colorPoint.b, 2));
                        
                        if (distanciaColor > myengine.levelMsk) points.addVertex(ofPoint(i,j,0));
                        
                    }
                    
                }
                
                ofSetColor(255);
                points.draw();
                
                myengine.fboInput.end();
                
                ofPixels pixels;
                myengine.fboInput.readToPixels(pixels);
                myengine.maskInput.setFromPixels(pixels);
                myengine.maskInput = blur(myengine.maskInput, 5);
                
                myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());
                
                
                
            }
            
            // myengine.coloringMaskImg = false;
            
        }
        
        
        
        
        
    }
    
    else if (e.target->is("minP")) {
        
        myengine.minP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("maxP")) {
        
        myengine.maxP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("prob")) {

        myengine.prob = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("size")) {

        myengine.pointSize = e.value;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("stroke")) {

        myengine.lineWidth = e.value;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("angleBackground")) {

        myengine.angleBackground = e.value;
        myengine.updateBackground();

    }

    else if (e.target->is("opacityImg")) {
        
        myengine.opacityImg = e.value;
        myengine.updateBackground();
        
    }

    else if (e.target->is("opacityGrid")) {
        
        myengine.opacityGrid = e.value;
        myengine.needsDrawPoints = true;
    }
    
    else if (e.target->is("opacityPoints")) {
        
        myengine.opacityPoints = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
}


void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.child << endl;
}


void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.color << endl;

    if (e.target->is("color")) {
        myengine.colorPoint = e.color;
        myengine.needsDrawPoints = true;

    }
    else if (e.target->is("colorOne")) {
        myengine.colorOne = ofColor(e.color);
        myengine.updateBackground();
    }
    else if (e.target->is("colorTwo")) {
        myengine.colorTwo = ofColor(e.color);
        myengine.updateBackground();
    }
    else if (e.target->is("colorGrid")) {
        myengine.colorTriangle = ofColor(e.color);
        myengine.updateBackground();
    }
}


void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onButtonEvent: " << e.text << endl;
    if (e.target->is("Definir Ancho")) {

        myengine.setResolution(std::stoi(e.text), myengine.height);
        myengine.updateBackground();
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;



    }

    else if (e.target->is("Definir Alto")) {

        myengine.setResolution(myengine.width, std::stoi(e.text));
        myengine.updateBackground();
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;


    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if ((key == 'q') || (key == 'Q')) {

        if (myengine.definingMaskImg) myengine.definingMaskImg = false;
        if (myengine.definingMaskGrid) myengine.definingMaskGrid = false;
        if (myengine.definingMaskPoints) myengine.definingMaskPoints = false;

    }

    else if ((key == 'a') || (key == 'A')) {
        
        if (myengine.coloringMaskImg) myengine.coloringMaskImg = false;
        
    }

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

    if ((myengine.definingMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathInput.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathInput.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathInput.getVertex(0);
        ofPoint last = myengine.pathInput.getVertex(myengine.pathInput.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathInput.getNumVertices() > 2)) {

            myengine.definingMaskImg = false;
            myengine.fboInput.begin();
            ofBackground(0);
            myengine.pathInput.draw();
            myengine.fboInput.end();

            ofPixels pixels;
            myengine.fboInput.readToPixels(pixels);
            myengine.maskInput.setFromPixels(pixels);
            myengine.maskInput = blur(myengine.maskInput, myengine.input.getWidth()/10);

            //myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());

        }

    }

    if ((myengine.definingMaskGrid) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathGrid.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathGrid.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathGrid.getVertex(0);
        ofPoint last = myengine.pathGrid.getVertex(myengine.pathGrid.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathGrid.getNumVertices() > 2)) {

            myengine.definingMaskGrid = false;
            myengine.fboGrid.begin();
            ofBackground(0, 0, 0, 0);
            myengine.pathGrid.draw();
            myengine.fboGrid.end();

            ofPixels pixels;
            myengine.fboGrid.readToPixels(pixels);
            myengine.maskGrid.setFromPixels(pixels);

            myengine.needsUpdateGrid = true;
            myengine.needsDrawPoints = true;

        }

    }

    if ((myengine.definingMaskPoints) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathPoints.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathPoints.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathPoints.getVertex(0);
        ofPoint last = myengine.pathPoints.getVertex(myengine.pathPoints.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathPoints.getNumVertices() > 2)) {

            myengine.definingMaskPoints = false;
            myengine.fboPoints.begin();
            ofBackground(0, 0, 0, 0);
            myengine.pathPoints.draw();
            myengine.fboPoints.end();

            ofPixels pixels;
            myengine.fboPoints.readToPixels(pixels);
            myengine.maskPoints.setFromPixels(pixels);

            myengine.needsUpdateGrid = true;
            myengine.needsDrawPoints = true;
        }

    }

    if ((myengine.coloringMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        if (myengine.input.isAllocated()) {

            float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
            float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
            ofColor colorSelected = myengine.input.getColor(xPoint, yPoint);

            myengine.fboInput.begin();

            // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
            ofClear(0, 0, 0, 0);

            ofMesh points;
            points.setMode(OF_PRIMITIVE_POINTS);

            for (int i = 0; i < myengine.input.getWidth(); i++) {

                for (int j = 0; j < myengine.input.getHeight(); j++) {
                    
                    ofColor colorPoint = myengine.input.getColor(i,j);
                    
                    float distanciaColor = sqrt( pow(colorSelected.r - colorPoint.r, 2) + pow(colorSelected.g - colorPoint.g, 2) + pow(colorSelected.b - colorPoint.b, 2));
                    
                    if (distanciaColor > myengine.levelMsk) points.addVertex(ofPoint(i,j,0));

                }

            }

            ofSetColor(255);
            points.draw();

            myengine.fboInput.end();

            ofPixels pixels;
            myengine.fboInput.readToPixels(pixels);
            myengine.maskInput.setFromPixels(pixels);
            myengine.maskInput = blur(myengine.maskInput, 5);

            myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());



        }

    }

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


ofImage ofApp::blur(ofImage img, int radio) {

    ofImage imgReturn;

    ofxCv::copy(img, imgReturn);
    ofxCv::blur(imgReturn, radio);
    imgReturn.update();
    
    return imgReturn;

}

void ofApp::createSettings() {

	xmlParameters.setValue("settings:width", 1024);
	xmlParameters.setValue("settings:height", 768);
	xmlParameters.setValue("settings:showInput", true);
	xmlParameters.setValue("settings:opacityImg", 100);
	xmlParameters.setValue("settings:levelMsk", 100);
	xmlParameters.setValue("settings:showGrid", true);
	xmlParameters.setValue("settings:opacityGrid", 100);
	xmlParameters.setValue("settings:min", 0);
	xmlParameters.setValue("settings:max", 255);
	xmlParameters.setValue("settings:density", 4);
	xmlParameters.setValue("settings:noise", 0);
	xmlParameters.setValue("settings:stroke", 1);
	xmlParameters.setValue("settings:colorGrid", 0xFFFFFF);
	xmlParameters.setValue("settings:showPoints", true);
	xmlParameters.setValue("settings:opacityPoints", 100);
	xmlParameters.setValue("settings:minP", 0);
	xmlParameters.setValue("settings:maxP", 255);
	xmlParameters.setValue("settings:densityP", 100);
	xmlParameters.setValue("settings:noiseP", 0);
	xmlParameters.setValue("settings:size", 5);
	xmlParameters.setValue("settings:punto", true);
	xmlParameters.setValue("settings:cuadrado", false);
	xmlParameters.setValue("settings:triangulo", false);
	xmlParameters.setValue("settings:archivo", false);
	xmlParameters.setValue("settings:color", 0x000000);
	xmlParameters.setValue("settings:graphicElements", false);
	xmlParameters.setValue("settings:loadBackground", false);
	xmlParameters.setValue("settings:colorBackground", true);
	xmlParameters.setValue("settings:defineBackground", false);
	xmlParameters.setValue("settings:angleBackground", 0);
	xmlParameters.setValue("settings:colorOne", 0x000000);
	xmlParameters.setValue("settings:colorTwo", 0xFFFFFF);

	xmlParameters.saveFile("settings.xml");




}
