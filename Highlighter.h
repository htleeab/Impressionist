#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#pragma once
#include "ImpBrush.h"
class Highlighter :
	public ImpBrush
{
private:
	Point previousPoint;
public:
	Highlighter(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	~Highlighter();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
