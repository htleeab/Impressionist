//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "ImpBrush.h"
#include <math.h>
#include <vector>
#include <algorithm>



#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_PAINT			7
#define CONVOLUTION			8
#define DISSOLVE			9


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}



	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	


		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->updateBrushDirection(source, target, true);
			if (isInRange(target)) {
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
				m_pDoc->saveUndobuffer();
			}
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->updateBrushDirection(source, target);
			if (isInRange(target))
				m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->updateBrushDirection(source, target);
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			RightMouseBegin(target);
			break;
		case RIGHT_MOUSE_DRAG:
			RightMouseMove(target);
			break;
		case RIGHT_MOUSE_UP:
			RightMouseEnd(target);
			break;

		case AUTO_PAINT:
			autoPaintDetails();
			break;

		case CONVOLUTION:
			m_pDoc->applyConvolution();
			break;

		case DISSOLVE:
			m_pDoc->dissolve();
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}

	Point target(coord.x, m_nWindowHeight - coord.y);

	this->m_pDoc->m_pUI->m_origView->setSource(target);

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::autoPaintDetails()
{
	Point target(0, 0);


	int original_size = m_pDoc->getSize();

	std::vector<Point> points_v;

	while (target.x < m_nDrawWidth) {
		target.x += original_size / 4 + 2;
		while (target.y < m_nDrawHeight) {
			target.y += original_size / 4 + 2;
			points_v.push_back(target);

		}
		target.y = 0;
	}

	std::random_shuffle(points_v.begin(), points_v.end());
	m_pDoc->updateBrushDirection(points_v.front(), points_v.front(), true);
	m_pDoc->m_pCurrentBrush->BrushBegin(points_v.front(), points_v.front());
	for (std::vector<Point>::iterator it = points_v.begin(); it != points_v.end(); it++) {
		int size = original_size + rand() % original_size - original_size / 2;
		m_pDoc->setSize(size);
		m_pDoc->updateBrushDirection(*it, *it);
		m_pDoc->m_pCurrentBrush->BrushMove(*it, *it);
		m_pDoc->m_pCurrentBrush->BrushEnd(*it, *it);
	}

	m_pDoc->setSize(original_size);
	SaveCurrentContent();
	RestoreContent();

}

void PaintView::autoPaint()
{
	isAnEvent = 1;
	eventToDo = AUTO_PAINT;
	refresh();
}

void PaintView::convolution() {
	isAnEvent = 1;
	eventToDo = CONVOLUTION;
	refresh();
}

void PaintView::dissolve() {
	isAnEvent = 1;
	eventToDo = DISSOLVE;
	refresh();
}

// Get line from right click

void PaintView::RightMouseBegin( const Point target)
{
	glLineWidth(1.0);
	rightStartPoint = target;
}

void PaintView::RightMouseMove(const Point point2)
{
	RestoreContent();
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	//glColor3ubv(glColor3f(1, 0, 0));
	glVertex2d(rightStartPoint.x, rightStartPoint.y);
	glVertex2d(point2.x, point2.y);
	glEnd();
}

void PaintView::RightMouseEnd(const Point point2)
{
	float pi = 3.141592654;
	float angle = atan2((rightStartPoint.y - point2.y) , (rightStartPoint.x - point2.x)) * 180 / pi;
	float size = sqrt(pow(rightStartPoint.y - point2.y, 2) + pow(rightStartPoint.x - point2.x, 2));
	//set angle, size
	m_pDoc->m_pUI->rightMouseSet(angle, size);
	RestoreContent();
	//refresh();
}

bool PaintView::isInRange(const Point target) {
	if (target.x > m_nDrawWidth || (m_nWindowHeight -target.y)>m_nDrawHeight)
		return false;
	return true;
}