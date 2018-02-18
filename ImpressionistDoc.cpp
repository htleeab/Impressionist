// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointsBrush.h"
#include "ScatterLinesBrush.h"
#include "ScatteredCirclesBrush.h"
#include "Highlighter.h"
#include "FilterBrush.h"
#include "AlphaMappedBrush.h"
#include <math.h>


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucAnotherImage = NULL;
	m_ucEdgeImage	= NULL;
	m_ucUndoBuffer	= NULL;
	brushBitmap		= NULL;
	filterKernel	= NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointsBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatterLinesBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCirclesBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_HIGHLIGHTER]
		= new Highlighter(this, "Highlighter");
	ImpBrush::c_pBrushes[BRUSH_FILTER]
		= new FilterBrush(this, "Filter");
	ImpBrush::c_pBrushes[BRUSH_ALPHA_MAPPED]
		= new AlphaMappedBrush(this, "Alpha-Mapped");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

	m_pCurrentBrushDirection = 0;

	colorPicked = false;

	useAnotherGradientBool = false;

	edgeClippingBool = false;
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

void ImpressionistDoc::setSize(int size)
{
	m_pUI->setSize(size);
}

int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

int ImpressionistDoc::getAngle()
{
	if (m_pCurrentBrushDirection==0)
		return m_pUI->getAngle();
	else if (m_pCurrentBrushDirection == 1) {
		return gradientAngle;
	}
	else if (m_pCurrentBrushDirection == 2) {
		return movementAngle;
	}
}

int ImpressionistDoc::getBrushDirection()
{
	return m_pCurrentBrushDirection;
}

float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

void ImpressionistDoc::setBrushDirection(int type)
{
	m_pCurrentBrushDirection = type;
}

void ImpressionistDoc::updateBrushDirection(const Point source, const Point target, bool start)
{
	if (m_pCurrentBrushDirection == 0)
		return;
	else if (m_pCurrentBrushDirection == 1) {//Gradient
		setGradientDirection(target);
	}
	else if (m_pCurrentBrushDirection == 2) {
		setMovementDirection(target, start);
	}
}

void ImpressionistDoc::setMovementDirection(const Point target, bool start) {
	if (start) {
		movementAngle=0;
	}
	else {
		movementAngle = atan2((previousPoint.y - target.y), (previousPoint.x - target.x)) * 180 / M_PI;
	}
	previousPoint = target;
}

void ImpressionistDoc::setGradientDirection(const Point source) {
	int sobelX[3][3] = { { -1,0,1 },{ -2,0,2 } ,{ -1,0,1 } };
	int sobelY[3][3] = { { 1,2,1 },{ 0,0,0 } ,{ -1,-2,-1 } };
	GLfloat grayscale[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j <  3; j++) {
			GLubyte color[3] = {0,0,0};
			if (m_ucAnotherImage&&useAnotherGradientBool) {
				memcpy(color, GetAnotherImagePixel(source.x + i - 1, source.y + j - 1), 3);
			}
			else {
				memcpy(color, GetOriginalPixel(source.x + i - 1, source.y + j - 1), 3);
			}
			grayscale[i][j] = (static_cast<GLfloat>(color[0])/255 + static_cast<GLfloat>(color[1])/255 + static_cast<GLfloat>(color[2])/255) / 3;
		}
	}

	float sobelXValue = 0;
	float sobelYValue = 0;
	// apply sobel kernel
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sobelXValue += grayscale[i][j] * sobelX[i][j];
			sobelYValue += grayscale[i][j] * sobelY[i][j];
		}
	}
	gradientAngle = atan2(sobelYValue, sobelXValue) * 180 / M_PI;
}

void ImpressionistDoc::useAnotherGradient(bool v)
{
	useAnotherGradientBool = v;
}

void ImpressionistDoc::deleteFilterKernel() {
	if (filterKernel) {
		for (int i = 0; i < filterKernelRow; i++) {
			delete[] filterKernel[i];
		}
		delete[] filterKernel;
		filterKernel = NULL;
	}
}

void ImpressionistDoc::normalizeKernel() {
	if (!filterKernel)return;
	float sum=0;
	for (int i = 0; i < filterKernelRow; i++) {
		for (int j = 0; j < filterKernelCol; j++)
			sum += filterKernel[i][j];
	}
	for (int i = 0; i < filterKernelRow; i++) {
		for (int j = 0; j < filterKernelCol; j++)
			filterKernel[i][j]= filterKernel[i][j]/sum;
	}
}

void ImpressionistDoc::blurringKernel() 
{
	if (filterKernel) deleteFilterKernel();
	int size = getSize();//filter size equal to brush size
	filterKernelRow = size;
	filterKernelCol = size;
	filterKernel = new float*[size];
	for (int i = 0; i < size; i++) {
		filterKernel[i] = new float[size];
		for (int j = 0; j < size; j++) {
			filterKernel[i][j] = (float)1 / (size*size);
		}
	}
}

void ImpressionistDoc::sharpeningKernel() 
{
	if (filterKernel) deleteFilterKernel();
	int size = 3;
	filterKernelRow = size;
	filterKernelCol = size;
	filterKernel = new float*[size];
	for (int i = 0; i < size; i++) {
		filterKernel[i] = new float[size];
		for (int j = 0; j < size; j++) {
			filterKernel[i][j] = (float)-1;
		}
	}
	filterKernel[1][1] = 9.0;
	normalizeKernel();
}

void ImpressionistDoc::applyKernel(int row, int col, int ** filter, bool normalize)
{
	if (filterKernel) deleteFilterKernel();
	filterKernelRow = row;
	filterKernelCol = col;
	filterKernel = new float*[row];
	for (int i = 0; i < row; i++) {
		filterKernel[i] = new float[col];
		for (int j = 0; j < col; j++) {
			//reflect for convolution
			filterKernel[i][j] = filter[row-i-1][col-j-1];
			//casting
		}
	}
	if (normalize)
		normalizeKernel();
}


//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

int ImpressionistDoc::changeMuralImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// check dimension
	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Different Dimension!");
		delete[] data;
		return 0;
	}

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;

	m_ucBitmap = data;


	// display it on origView
	m_pUI->m_origView->refresh();

	return 1;
}

int ImpressionistDoc::swapImage() {
	unsigned char*	tmpSwapData;
	tmpSwapData = m_ucPainting;
	m_ucPainting = m_ucBitmap;
	m_ucBitmap = tmpSwapData;

	m_pUI->m_origView->refresh();
	m_pUI->m_paintView->refresh();
	return 1;
}

int ImpressionistDoc::loadAnotherImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
					height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// check dimension
	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Different Dimension!");
		delete[] data;
		return 0;
	}

	// release old storage
	if (m_ucAnotherImage) delete[] m_ucAnotherImage;

	m_ucAnotherImage = data;

	return 1;
}

int ImpressionistDoc::loadEdgeImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// check dimension
	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Different Dimension!");
		delete[] data;
		return 0;
	}

	// release old storage
	if (m_ucEdgeImage) delete[] m_ucEdgeImage;

	m_ucEdgeImage = data;

	return 1;
}

int ImpressionistDoc::loadBrushBitmap(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
					height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// release old storage
	if (brushBitmap) {
		for (int i = 0; i < brushBitmapHeight; i++) {
			delete[] brushBitmap[i];
		}
		delete[] brushBitmap; 
	}

	brushBitmapWidth = width;
	brushBitmapHeight = height;

	brushBitmap = new float*[height];
	for (int i = 0; i < height; i++) {
		brushBitmap[i] = new float[width];
	}

	GLubyte color[3];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			memcpy(color, data + 3 * (j*width + i),3);
			brushBitmap[i][j] = ((float)color[0] / 255.0 + (float)color[1] / 255.0 + (float)color[2]/255.0) / 3.0;
		}
	}
	
	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::autoPaint()
{
	m_pUI->m_paintView->autoPaint();

}

void ImpressionistDoc::saveUndobuffer()
{
	if (!m_ucUndoBuffer) m_ucUndoBuffer = new unsigned char[3 * m_nWidth*m_nHeight];
	memcpy(m_ucUndoBuffer, m_ucPainting, 3* m_nWidth*m_nHeight);
}

void ImpressionistDoc::undo()
{
	memcpy(m_ucPainting, m_ucUndoBuffer, 3 * m_nWidth*m_nHeight);
	m_pUI->m_paintView->refresh();
}

GLubyte* ImpressionistDoc::GetAnotherImagePixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucAnotherImage + 3 * (y*m_nWidth + x));
}

GLubyte * ImpressionistDoc::GetEdgeImagePixel(int x, int y)
{
	return GetTargetImagePixel(x, y, m_ucEdgeImage);
}

/*assume the size is same to original image*/
GLubyte * ImpressionistDoc::GetTargetImagePixel(int x, int y, unsigned char* image)
{

	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(image + 3 * (y*m_nWidth + x));
}