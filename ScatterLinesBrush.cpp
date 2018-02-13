
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatterLinesBrush.h"
#include <math.h>

ScatterLinesBrush::ScatterLinesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}


ScatterLinesBrush::~ScatterLinesBrush()
{
}



void ScatterLinesBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int width = pDoc->getWidth();
	glLineWidth((float)width);

	BrushMove(source, target);
}

void ScatterLinesBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	float pi = 3.14159265;
	float angle = (float)pDoc->getAngle() / 360 * 2 * pi;

	glBegin(GL_LINES);

	for (int y = target.y - size / 2; y <= target.y + size / 2; y++) {
		Point p_source;
		p_source.x = target.x;
		p_source.y = y;
		SetColor(p_source);
		if (rand() % 8 == 0) {
			glVertex2d(target.x - size / 2 * cos(angle), y - size / 2 * sin(angle));
			glVertex2d(target.x + size / 2 * cos(angle), y + size / 2 * sin(angle));
		}
	}

	glEnd();
}

void ScatterLinesBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
