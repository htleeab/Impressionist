
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

	int len1, len2;
	Point p_source;
	for (int y = target.y - size / 2; y <= target.y + size / 2; y++) {
		p_source.x = target.x;
		p_source.y = y;
		if (rand() % 8 == 0) {
			len1 = size; len2 = size;
			if (pDoc->edgeClippingBool) {
				if (pDoc->m_ucEdgeImage) {
					findEdgePoint(target, size, angle, len1, len2, pDoc->m_ucEdgeImage);//return len1,len2
				}
				else {
					fl_alert("please load edge image");
				}
			}
			SetColor(p_source);
			glVertex2d(target.x - len1 / 2.0 * cos(angle), y - len1 / 2.0 * sin(angle));
			glVertex2d(target.x + len2 / 2.0 * cos(angle), y + len2 / 2.0 * sin(angle));
		}
	}

	glEnd();
}

void ScatterLinesBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void ScatterLinesBrush::findEdgePoint(const Point center, const int size, const float angle, int& len1, int& len2, unsigned char* edgeImage) {//output len1, len2
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