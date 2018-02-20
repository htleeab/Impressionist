#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "FilterBrush.h"

extern float frand();


FilterBrush::FilterBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

FilterBrush::~FilterBrush()
{
}


void FilterBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (!pDoc->filterKernel) {//equal to null
		pDoc->blurringKernel();
	}
	
	glPointSize((float)1);
	glDisable(GL_POINT_SMOOTH);

	BrushMove(source, target);
}

void FilterBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glPointSize((float)1.0);
	glDisable(GL_POINT_SMOOTH);
	//BrushMove(source, target);

	if (pDoc == NULL) {
		printf("Filter Brush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POINTS);
	int brushSize = pDoc->getSize();
	for (int i = - brushSize / 2; i < brushSize / 2; i++) {
		for (int j = - brushSize / 2; j < brushSize / 2; j++) {
			Point p_target(target.x + i, target.y + j);
			Point p_source( source.x + i,source.y + j);
			SetColor(p_source); //override setColor
			glVertex2d(p_target.x, p_target.y);
		}
	}
	
	glEnd();
}

void FilterBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


void FilterBrush::SetColor(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	float** filterKernel = pDoc->filterKernel; //pointer

	GLfloat colorFloat[3];

	for (int c = 0; c < 3; c++) {
		colorFloat[c] = 0;
	}
	
	GLubyte sourceColor[3];
	int row = pDoc->filterKernelRow;
	int col = pDoc->filterKernelRow;
	Point p_source;	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			p_source.x = source.x + i - row / 2;
			p_source.y = source.y + j - col / 2;
			memcpy(sourceColor, pDoc->GetOriginalPixel(p_source), 3);
			for (int c = 0; c < 3; c++) {
				colorFloat[c] += (sourceColor[c] * filterKernel[i][j] / 255.0);
			}
		}
	}
	GLbyte color[3];
	for (int c = 0; c < 3; c++) {
		colorFloat[c] = max(colorFloat[c], 0.0);
		colorFloat[c] = min(colorFloat[c], 1.0);
		color[c] = (byte)static_cast<unsigned int>(colorFloat[c] * 255);
	}
	
	if (pDoc->colorPicked) {
		GLubyte pickedcolor[3];
		memcpy(pickedcolor, pDoc->Pickedcolor, 3);
		for (int i = 0; i != 3; i++) {
			color[i] = color[i] * (pickedcolor[i] / 255.0);
		}
	}

	GLubyte alphaByte = (byte)static_cast<unsigned int>(pDoc->getAlpha() * 255);
	glColor4ub(color[0], color[1], color[2], alphaByte);
}
