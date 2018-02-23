
#ifndef MOSAICMAKER_H
#define MOSAICMAKER_H

#include "impressionistDoc.h"
#include "impressionistUI.h"

class MosaicMaker
{
public:
	MosaicMaker(ImpressionistDoc* pDoc = NULL);
	~MosaicMaker();

	int findBestFitTile(GLubyte** tilecolors, GLubyte* avgColor);
	void generateMosaic(double alpha = 0.3 ,int size = 24);
	static GLubyte* findTileAverageColor(int size, unsigned char* bitmap);
	GLubyte * findRegionAverageColor(Point up_left, Point bottom_right, unsigned char * image, int width, int height);
	GLubyte* findRegionAverageColor(Point up_left, Point bottom_right);

	void drawMosaic(int tile, Point up_left, double alpha, int size);

	void dissolve(float alpha);

private:
	ImpressionistDoc*	m_pDoc;


};

#endif // !MOSAICMAKER_H