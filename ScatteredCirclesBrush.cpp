#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCirclesBrush.h"

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

ScatteredCirclesBrush::~ScatteredCirclesBrush()
{
}

void ScatteredCirclesBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	glPointSize((float)size);
	glEnable(GL_POINT_SMOOTH);

	BrushMove(source, target);
}

void ScatteredCirclesBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POINTS);
	int size = pDoc->getSize();
	int number_of_circles = 4;

	for (int i = 0; i < number_of_circles; i++) {
		Point p_source;
		p_source.x = target.x + rand()%size- size/2;
		p_source.y = target.y + rand() % size - size / 2;
		SetColor(p_source);
		glVertex2d(p_source.x, p_source.y);
	}
	glEnd();
}

void ScatteredCirclesBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
