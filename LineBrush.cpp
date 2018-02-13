#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

LineBrush::~LineBrush()
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int width = pDoc->getWidth();
	glLineWidth((float)width);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	float pi = 3.141592654;
	int size = pDoc->getSize();
	float angle = (float)pDoc->getAngle()/360 *2* pi;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	glVertex2d(target.x - size / 2 * cos(angle), target.y - size / 2 * sin(angle));
	glVertex2d(target.x + size / 2 * cos(angle), target.y + size / 2 * sin(angle));

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}