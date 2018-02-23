#include "MosaicMaker.h"



double getSquareDiff(GLubyte * a, GLubyte * b) {
	double diff = 0.0;
	for (int i = 0; i != 3; i++) {
		diff += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return diff;
}


MosaicMaker::MosaicMaker(ImpressionistDoc* pDoc)
{
	m_pDoc = pDoc;

}


MosaicMaker::~MosaicMaker()
{
}

int MosaicMaker::findBestFitTail(GLubyte ** tailcolors, GLubyte * avgColor)
{
	double min_diff = 2e20;
	int best_tail = 0;
	for (int i = 0; i != 8; i++) {
		if (!tailcolors[i]) continue;
		double diff = getSquareDiff(tailcolors[i], avgColor);
		if (diff < min_diff) {
			best_tail = i;
			min_diff = diff;
		}
	}
	return best_tail;
}

void MosaicMaker::generateMosaic(double alpha, int size)
{
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;

	int width_tails = width / size;
	int height_tails = height / size;


	ImpressionistUI* m_pUI = m_pDoc->m_pUI;

	for (int i = 0; i != height_tails; i++) {
		for (int j = 0; j != width_tails; j++) {
			Point up_left(j * size, i * size);
			Point bottom_right((j + 1) * size - 1, (i + 1) * size - 1);

			GLubyte* avgColor;

			avgColor = findRegionAverageColor(up_left, bottom_right);
			int bestTail = findBestFitTail(m_pDoc->tailColors, avgColor);

			drawMosaic(bestTail, up_left, alpha, size);

			delete[] avgColor;
		}
	}
	m_pUI->m_paintView->SaveCurrentContent();
	m_pUI->m_paintView->RestoreContent();
}

GLubyte* MosaicMaker::findTailAverageColor(int size, unsigned char * bitmap)
{
	float color_sum[3] = { 0.0, 0.0, 0.0 };
	for (int i = 0; i != size*size; i++) {
		color_sum[0] += bitmap[3 * i];
		color_sum[1] += bitmap[3 * i+1];
		color_sum[2] += bitmap[3 * i+2];
	}
	GLubyte* c = new GLubyte[3];

	for (int i = 0; i != 3; i++) {
		color_sum[i] = color_sum[i] / size / size;
		c[i] = (unsigned char)((int)color_sum[i]);
	}

	return c;

}

GLubyte* MosaicMaker::findRegionAverageColor(Point up_left, Point bottom_right, unsigned char* image, int width, int height)
{
	//printf("change region\n");
	GLubyte* color = new GLubyte[3];
	float color_sum[3] = { 0.0, 0.0, 0.0 };
	for (int i = up_left.y; i != bottom_right.y; i++) {
		for (int j = up_left.x; j != bottom_right.x; j++) {
			Point target(j, i);

			//printf("%d, %d", target.x, target.y);

			memcpy(color, m_pDoc->GetTargetImagePixel(target.x, target.y, image, width, height), 3);

			//printf(" ok after copy\n");
			color_sum[0] += color[0];
			color_sum[1] += color[1];
			color_sum[2] += color[2];
		}
	}
	delete[] color;

	GLubyte* c = new GLubyte[3];

	int size = bottom_right.y - up_left.y + 1;

	for (int i = 0; i != 3; i++) {
		color_sum[i] = color_sum[i] / size / size;
		c[i] = (unsigned char)((int)color_sum[i]);
	}

	return c;
}

GLubyte * MosaicMaker::findRegionAverageColor(Point up_left, Point bottom_right)
{
	return findRegionAverageColor(up_left, bottom_right, m_pDoc->m_ucBitmap, m_pDoc->m_nWidth, m_pDoc->m_nHeight);
}

void MosaicMaker::drawMosaic(int tail, Point up_left, double alpha, int size)
{

	//just like point brush
	glPointSize((float)1.0);
	glDisable(GL_POINT_SMOOTH);
	Point target(0, 0);
	glBegin(GL_POINTS);

	for (int i = 0; i != size; i++) {
		for (int j = 0; j != size ; j++) {
			target.x = up_left.x + j; 
			target.y = up_left.y + i;
			// Find dissolve color
			GLubyte color[3];
			memcpy(color, m_pDoc->GetTargetImagePixel(j, i, m_pDoc->m_ucTails[tail], size, size), 3);
			glColor3ub(color[0], color[1], color[2]);
			glVertex2i(target.x, target.y);
		}
	}

	glEnd();

}


