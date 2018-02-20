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
	int size = pDoc->getSize();
	float angle = (float)pDoc->getAngle()/360 *2* M_PI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	int len1, len2;
	len1 = size; len2 = size;
	if (pDoc->edgeClippingBool) {
		if (pDoc->m_ucEdgeImage) {
			findEdgePoint(target, size, angle, len1, len2, pDoc->m_ucEdgeImage);//return len1,len2
		}
		else {
			fl_alert("Cannot find loaded edge image. Generate edge image");
			pDoc->generateEdgeImage();
		}
	}	

	glVertex2d(target.x - len1 / 2.0 * cos(angle), target.y - len1 / 2.0 * sin(angle));
	glVertex2d(target.x + len2 / 2.0 * cos(angle), target.y + len2 / 2.0 * sin(angle));

	glEnd();
}

void LineBrush::findEdgePoint(const Point center, const int size, const float angle, int& len1, int& len2, unsigned char* edgeImage) {//output len1, len2
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];

	for (len1=0;len1<=size;len1++){
		memcpy(color, pDoc->GetTargetImagePixel((int)(center.x - len1 / 2.0 * cos(angle)), (int)(center.y - len1/ 2.0 * sin(angle)),edgeImage), 3);
		if (GetColorValue(color) > 0.9) {//=color = white = on edge
			len1--;
			break;
		}		
	}

	for (len2 = 0; len2 <= size; len2++) {
		memcpy(color, pDoc->GetTargetImagePixel((int)center.x + len2 / 2.0 * cos(angle), (int)center.y + len2 / 2.0 * sin(angle), edgeImage), 3);
		if (GetColorValue(color) > 0.9) {//=color = white = on edge
			len2--;
			break;
		}
	}
	if (len1 < 0)len1 = 0;
	if (len2 < 0)len2 = 0;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}