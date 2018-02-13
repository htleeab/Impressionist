#ifndef CIRCLEBRUSH_H
#define CIRCLEBRUSH_H

#pragma once
#include "ImpBrush.h"
class CircleBrush :
	public ImpBrush
{
public:
	CircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL); 
	~CircleBrush();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};


#endif