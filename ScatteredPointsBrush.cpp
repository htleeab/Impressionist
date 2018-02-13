
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointsBrush.h"

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}


ScatteredPointsBrush::~ScatteredPointsBrush()
{
}

void ScatteredPointsBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize(1.0);

	BrushMove(source, target);
}

void ScatteredPointsBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointsBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POINTS);

	int size = pDoc->getSize();

	for (int x = target.x - size / 2; x <= target.x + size / 2; x++) {
		for (int y = target.y - size / 2; y <= target.y + size / 2; y++) {
			if (rand() % 8 ==0) { 
				Point p_source;
				p_source.x = x;
				p_source.y = y;
				SetColor(p_source);
				glVertex2d(x, y);
			}
		}
	}

	glEnd();
}

void ScatteredPointsBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
