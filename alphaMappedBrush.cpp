#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "alphaMappedBrush.h"

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

AlphaMappedBrush::~AlphaMappedBrush()
{
}


void AlphaMappedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (!pDoc->brushBitmap) {//equal to null
		//init [[1]] with size 1x1
		pDoc->brushBitmap = new float*[1];
		pDoc->brushBitmap[0] = new float[1];
		pDoc->brushBitmap[0][0]=0.0;
	}

	glPointSize((float)1);
	glDisable(GL_POINT_SMOOTH);

	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("Filter Brush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POINTS);
	int width = pDoc->brushBitmapWidth;
	int height = pDoc->brushBitmapHeight;

	for (int i = -height / 2; i < height / 2; i++) {
		for (int j = -width / 2; j < width / 2; j++) {
			Point p_target(target.x + j, target.y + i);
			SetColor(source, i + height /2 , j+ width /2); //override setColor
			glVertex2d(p_target.x, p_target.y);
		}
	}

	glEnd();
}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


void AlphaMappedBrush::SetColor(const Point source,int i, int j)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	float alpha = pDoc->brushBitmap[i][j];
	
	if (pDoc->colorPicked) {
		GLubyte pickedcolor[3];
		memcpy(pickedcolor, pDoc->Pickedcolor, 3);
		for (int i = 0; i != 3; i++) {
			color[i] = color[i] * (pickedcolor[i] / 255.0);
		}
	}

	GLubyte alphaByte = (byte)static_cast<unsigned int>(alpha * 255);
	glColor4ub(color[0], color[1], color[2], alphaByte);
}