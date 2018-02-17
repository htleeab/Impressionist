//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>
#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

void ImpressionistUI::cb_swap_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	pDoc->swapImage();
	
}

void ImpressionistUI::cb_pick_color(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	GLubyte Pickedcolor[3];
	memcpy(Pickedcolor, pDoc->Pickedcolor, 3);
	int picked = fl_color_chooser("Color", Pickedcolor[0], Pickedcolor[1], Pickedcolor[2],-1);
	if (picked) {
		memcpy(pDoc->Pickedcolor, Pickedcolor, 3);
		pDoc->colorPicked = true;
	}
}

void ImpressionistUI::cb_auto_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->autoPaint();
}
//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
	whoami(o)->filterDialog->hide();
}

void ImpressionistUI::cb_blur_filter_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc ->blurringKernel();
}

void ImpressionistUI::cb_sharpen_filter_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->sharpeningKernel();
}

void ImpressionistUI::cb_customize_filter_button(Fl_Widget* o, void* v)
{
	//ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	pUI->filterDialog->show();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;

	pDoc->setBrushType(type);

	if (type == 1 || type == 4){
		pUI->m_BrushDirectionChoice->activate();
		pUI->m_BrushLineWidthSlider->activate();
		pUI->m_BrushLineAngleSlider->activate();
	}else{
		pUI->m_BrushDirectionChoice->deactivate();
		pUI->m_BrushLineWidthSlider->deactivate();
		pUI->m_BrushLineAngleSlider->deactivate(); 
	}
	if (type == 7) {
		pUI->m_FilterBlurButton->activate();
		pUI->m_FilterSharpenButton->activate();
		pUI->m_FilterCustomizeButton->activate();
	}
	else {
		pUI->m_FilterBlurButton->deactivate();
		pUI->m_FilterSharpenButton->deactivate();
		pUI->m_FilterCustomizeButton->deactivate();
	}

}

void ImpressionistUI::cb_brushDirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setBrushDirection(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_alpha = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_filterRowInput(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	pUI->deleteFilterInput();
	pUI->filterRow = atoi(((Fl_Int_Input *)o)->value());
	pUI->applyFilterButton->deactivate();
	pUI->applyConvolutionButton->deactivate();
}
void ImpressionistUI::cb_filterColInput(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	pUI->deleteFilterInput();
	pUI->filterCol = atoi(((Fl_Int_Input *)o)->value());
	pUI->applyFilterButton->deactivate();
	pUI->applyConvolutionButton->deactivate();
}

void ImpressionistUI::cb_init_filter(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	printf("cb_init_filter\n");
	pUI->deleteFilterInput();
	pUI->filterDialog->begin();		
		int row = pUI->filterRow;
		int col = pUI->filterCol;
		pUI->filterValueInput = new Fl_Int_Input**[row];
		pUI->filterValue = new int*[row];
		for (int i = 0; i < row; i++) {
			pUI->filterValueInput[i] = new Fl_Int_Input*[col];
			pUI->filterValue[i] = new int[col];
			for (int j = 0; j < col; j++) {
				pUI->filterValue[i][j] = 1;
				pUI->filterValueInput[i][j]=new Fl_Int_Input(30+300/row*i, 50+200/col*j, 20, 20,"");
				//pUI->filterValueInput[i][j]->user_data((void*)(this));
				filterValueUserData* value_data = new filterValueUserData(pUI,i, j);
				pUI->filterValueInput[i][j]->user_data(value_data); 
				pUI->filterValueInput[i][j]->value("1");
				pUI->filterValueInput[i][j]->callback(cb_update_filter);
			}
		}
	pUI->filterDialog->end();
	pUI->applyFilterButton->activate();
	pUI->applyConvolutionButton->activate();
	pUI->filterDialog->redraw();
}

void ImpressionistUI::cb_update_filter(Fl_Widget* o, void* v) {
	filterValueUserData data = *(filterValueUserData*)(o->user_data());
	ImpressionistUI* pUI = (ImpressionistUI*)(data.self);
	int row = data.row_index;
	int col = data.col_index;
	int value = atoi(((Fl_Int_Input *)o)->value());//self value
	pUI->filterValue[row][col] = value;
}

void ImpressionistUI::cb_apply_filter(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	ImpressionistDoc * pDoc = pUI->getDocument();
	pDoc->applyKernel(pUI->filterRow, pUI->filterCol, pUI->filterValue);
}


void ImpressionistUI::cb_convolution(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = (ImpressionistUI*)(o->user_data());
	ImpressionistDoc * pDoc = pUI->getDocument();
	pDoc->applyKernel(pUI->filterRow, pUI->filterCol, pUI->filterValue);
	pDoc->autoPaint();
}



//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

void ImpressionistUI::deleteFilterInput() {
	if (filterValueInput) {//When updating the size of filter, delete before update
		for (int i = 0; i < filterRow; i++) {
			for (int j = 0; j < filterCol; j++) {
				filterDialog->remove(filterValueInput[i][j]);
				delete filterValueInput[i][j];
			}
			delete[] filterValueInput[i];
			delete[] filterValue[i];
		}
		delete[] filterValueInput;
		delete[] filterValue;
	}
	filterValueInput = NULL;
	filterValue = NULL;	
	filterDialog->redraw();
	printf("filter clear\n");
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getWidth()
{
	return m_lWidth;
}

void ImpressionistUI::setWidth(int width)
{
	m_lWidth = width;

	if (width <= 40)
		m_BrushLineWidthSlider->value(m_lWidth);
}


int ImpressionistUI::getAngle()
{
	return m_lAngle;
}

void ImpressionistUI::setAngle(int angle)
{
	m_lAngle = angle;

	if (angle <= 359)
		m_BrushLineAngleSlider->value(m_lAngle);
}

float ImpressionistUI::getAlpha()
{
	return m_alpha;
}


void ImpressionistUI::rightMouseSet(float angle, float size) {
	m_lAngle = angle;
	m_nSize = size;
	m_lWidth = 1.0;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Swap Images",	FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_swap_image ,0, FL_MENU_DIVIDER },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&Color...",	FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_pick_color },
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Highlighter",	FL_ALT + 'h', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_HIGHLIGHTER },
  { "Filter",	FL_ALT + 'h', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_FILTER },
  {0}
};


Fl_Menu_Item ImpressionistUI::brushDirectionMenu[4] = {
	{ "Slider/Right Mouse",			FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)0},
	{ "Gradient",				FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)1},
	{ "Brush Direction",			FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushDirectionChoice, (void *)2},
	{ 0 }
};

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_lWidth = 1;
	m_lAngle = 0;
	m_alpha = 1.0;
	filterRow = 3;
	filterCol = 3;
	filterValue = NULL;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_BrushDirectionChoice = new Fl_Choice(110, 40, 150, 25, "&Stroke Direction");
		m_BrushDirectionChoice->user_data((void*)(this));
		m_BrushDirectionChoice->menu(brushDirectionMenu);
		m_BrushDirectionChoice->callback(cb_brushDirectionChoice);
		m_BrushDirectionChoice->deactivate();

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 70, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		m_BrushLineWidthSlider = new Fl_Value_Slider(10, 100, 300, 20, "Line Width");
		m_BrushLineWidthSlider->user_data((void*)(this));
		m_BrushLineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushLineWidthSlider->labelfont(FL_COURIER);
		m_BrushLineWidthSlider->labelsize(12);
		m_BrushLineWidthSlider->minimum(1);
		m_BrushLineWidthSlider->maximum(40);
		m_BrushLineWidthSlider->step(1);
		m_BrushLineWidthSlider->value(m_lWidth);
		m_BrushLineWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushLineWidthSlider->callback(cb_widthSlides);
		m_BrushLineWidthSlider->deactivate();

		m_BrushLineAngleSlider = new Fl_Value_Slider(10, 130, 300, 20, "Line Angle");
		m_BrushLineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushLineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushLineAngleSlider->labelfont(FL_COURIER);
		m_BrushLineAngleSlider->labelsize(12);
		m_BrushLineAngleSlider->minimum(0);
		m_BrushLineAngleSlider->maximum(360);
		m_BrushLineAngleSlider->step(1);
		m_BrushLineAngleSlider->value(m_lAngle);
		m_BrushLineAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushLineAngleSlider->callback(cb_angleSlides);
		m_BrushLineAngleSlider->deactivate();

		m_BrushAlphaSlides = new Fl_Value_Slider(10, 160, 300, 20, "Alpha");
		m_BrushAlphaSlides->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlides->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlides->labelfont(FL_COURIER);
		m_BrushAlphaSlides->labelsize(12);
		m_BrushAlphaSlides->minimum(0);
		m_BrushAlphaSlides->maximum(1);
		m_BrushAlphaSlides->step(0.01);
		m_BrushAlphaSlides->value(m_alpha);
		m_BrushAlphaSlides->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlides->callback(cb_alphaSlides);

		m_FilterBlurButton = new Fl_Button(20, 190, 85, 25, "&Blurring");
		m_FilterBlurButton->user_data((void*)(this));
		m_FilterBlurButton->callback(cb_blur_filter_button);
		m_FilterBlurButton->deactivate();

		m_FilterSharpenButton = new Fl_Button(130, 190, 85, 25, "&Sharpening");
		m_FilterSharpenButton->user_data((void*)(this));
		m_FilterSharpenButton->callback(cb_sharpen_filter_button);
		m_FilterSharpenButton->deactivate();

		m_FilterCustomizeButton = new Fl_Button(260, 190, 100, 25, "&Customize");
		m_FilterCustomizeButton->user_data((void*)(this));
		m_FilterCustomizeButton->callback(cb_customize_filter_button);
		m_FilterCustomizeButton->deactivate();

		m_ClearCanvasButton = new Fl_Button(10, 230, 150, 25, "&Auto Paint");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_auto_paint_button);

    m_brushDialog->end();	

	//FilterDialog

	filterDialog = new Fl_Window(350, 350, "Filter Kernel");
	
		filterRowInput = new Fl_Int_Input(40, 20, 25, 25, "&Row");
		filterRowInput->user_data((void*)(this));
		filterRowInput->callback(cb_filterRowInput);

		filterColInput = new Fl_Int_Input(120, 20, 25, 25, "&Col");
		filterColInput->user_data((void*)(this));
		filterColInput->callback(cb_filterColInput);

		initFilterButton = new Fl_Button(250, 20, 75, 25, "&Init. Filter");
		initFilterButton->user_data((void*)(this));
		initFilterButton->callback(cb_init_filter);

		filterValueInput = NULL;

		applyFilterButton = new Fl_Button(50, 280, 100, 25, "&Apply As Brush");
		applyFilterButton->user_data((void*)(this));
		applyFilterButton->callback(cb_apply_filter);
		applyFilterButton->deactivate();

		applyConvolutionButton = new Fl_Button(175, 280, 125, 25, "&Apply Convolution");
		applyConvolutionButton->user_data((void*)(this));
		applyConvolutionButton->callback(cb_convolution);
		applyConvolutionButton->deactivate();

	filterDialog->end();
}

