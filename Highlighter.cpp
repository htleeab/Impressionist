#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "Highlighter.h"
#include <math.h>



Highlighter::Highlighter(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name) 
{
}


Highlighter::~Highlighter()
{
}

void Highlighter::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//int width = pDoc->getWidth();
	glLineWidth(1);

	previousPoint = target;
	
	//BrushMove(source, target);
}

void Highlighter::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();
	float angle = (float)pDoc->getAngle() / 360 * 2 * M_PI;

	if (pDoc == NULL) {
		printf("Highlighter::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POLYGON);
	SetColor(source);

	glVertex2d(previousPoint.x - size / 2 * cos(angle), previousPoint.y - size / 2 * sin(angle));
	glVertex2d(previousPoint.x + size / 2 * cos(angle), previousPoint.y + size / 2 * sin(angle));

	glVertex2d(target.x + size / 2 * cos(angle), target.y + size / 2 * sin(angle));
	glVertex2d(target.x - size / 2 * cos(angle), target.y - size / 2 * sin(angle));

	glVertex2d(previousPoint.x - size / 2 * cos(angle), previousPoint.y - size / 2 * sin(angle));
	glEnd();

	previousPoint = target;
}

void Highlighter::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}