#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"

extern float frand();


CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}


CircleBrush::~CircleBrush()
{
}


void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	
	glPointSize((float)size);
	glEnable(GL_POINT_SMOOTH);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POINTS);
	

	//glBegin(GL_TRIANGLE_FAN);
	SetColor(source);
	//glVertex2f(target.x, target.y); // center of circle
	//GLint triangleAmount = 20;
	//GLfloat twicePi = 2.0 * 3.141592654;
	//for (int i = 0; i <= triangleAmount; i++) {
	//	glVertex2f(
	//		target.x + (pDoc->getSize() * cos(i * twicePi / triangleAmount)),
	//		target.y + (pDoc->getSize() * sin(i * twicePi / triangleAmount))
	//	);
	//}
	//glEnd();

	glVertex2d(target.x, target.y);

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}