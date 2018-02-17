#pragma once

#ifndef SCATTERLINESBRUSH_H
#define SCATTERLINESBRUSH_H

#include "ImpBrush.h"
class ScatterLinesBrush :
	public ImpBrush
{
public:
	ScatterLinesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	~ScatterLinesBrush();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void findEdgePoint(const Point center, const int size, const float angle, int & len1, int & len2, unsigned char * edgeImage);
	char* BrushName(void);
};

#endif