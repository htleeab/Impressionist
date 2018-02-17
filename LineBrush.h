#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#pragma once
#include "ImpBrush.h"
class LineBrush :
	public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	~LineBrush();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void findEdgePoint(const Point center, const int size, const float angle, int & len1, int & len2, unsigned char * edgeImage);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};


#endif