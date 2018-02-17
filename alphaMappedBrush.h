#pragma once
#include "ImpBrush.h"
class AlphaMappedBrush :
	public ImpBrush
{
public:
	AlphaMappedBrush(ImpressionistDoc * pDoc, char * name);
	~AlphaMappedBrush();
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void SetColor(const Point source, int i, int j);
};

