#pragma once

#ifndef SCATTERCIRCLESBRUSH_H
#define SCATTERCIRCLESBRUSH_H

#include "ImpBrush.h"
class ScatteredCirclesBrush :
	public ImpBrush
{
public:
	ScatteredCirclesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	~ScatteredCirclesBrush();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif