// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image
	int		changeMuralImage(char *iname);
	int		swapImage();
	int		loadAnotherImage(char *iname);
	int		loadEdgeImage(char* iname);
	int		loadBrushBitmap(char * iname);
	int		loadTailBitmap(char *iname, int i, int size = 24);

	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get brush size from UI 
	void	setSize(int size);				// set brush size 
	int		getWidth();
	int		getAngle();
	int		getBrushDirection();
	float	getAlpha();
	void	setBrushDirection(int type);
	void	updateBrushDirection(const Point source, const Point target, bool start = false);
	void	setMovementDirection(const Point target, bool start = false);
	void	setGradientDirection(const Point source);
	void	useAnotherGradient(bool v);

	void	autoPaint();
	void	applyConvolution();
	unsigned char* generateEdgeImage();

	//for undo
	void	saveUndobuffer();
	void	undo();

	char*	getImageName();					// get the current image name

	void	deleteFilterKernel();
	void	normalizeKernel();
	void	blurringKernel();
	void	sharpeningKernel();
	void	applyKernel(int row, int col, int** filter, bool normalize = TRUE);
	void	dissolve();
	void	drawMosaic();
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char*	m_ucAnotherImage;
	unsigned char*	m_ucEdgeImage;
	float**			brushBitmap;
	unsigned int	displayMode;

	unsigned char*	m_ucUndoBuffer;

	unsigned char**	m_ucTails;
	GLubyte** tailColors;

	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int				m_nSize;		
	int				m_pCurrentBrushDirection;
	Point			previousPoint;
	float			movementAngle;
	float			gradientAngle;
	bool			useAnotherGradientBool;
	float			dissolveFactor;

	bool			edgeClippingBool;

	int				brushBitmapWidth;
	int				brushBitmapHeight;

	bool			colorPicked;
	GLubyte			Pickedcolor[3];
	float**			filterKernel;
	int				filterKernelRow;
	int				filterKernelCol;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );
	// Get the color of the another picture at the specified point
	GLubyte* GetAnotherImagePixel(int x, int y);
	// Get the color of the edge picture at the specified point
	GLubyte* GetEdgeImagePixel(int x, int y);
	GLubyte * GetTargetImagePixel(int x, int y, unsigned char * image);
	GLubyte * GetTargetImagePixel(int x, int y, unsigned char * image, int width, int height);
	unsigned char*	currentDisplay();

private:
	char			m_imageName[256];
};

extern void MessageBox(char *message);

#endif
