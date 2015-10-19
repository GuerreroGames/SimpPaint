#ifndef __GraphicsMathLib_H__
#define __GraphicsMathLib_H__

class Point2D{
public:
	Point2D();
	Point2D(float inX, float inY);
	float x;
	float y;
};

class Vec2D{
public:
	Vec2D();
	Vec2D(float inX, float inY, float mag);
	float x;
	float y;
	float magnitude;
	float calcMag();
};

class Colour{
public:
	Colour();
	Colour(float red, float green, float blue);
	float r;
	float g;
	float b;
};

class MathLib{
public:
	MathLib();
	float distBetween(Point2D p1, Point2D p2);
	float fastDist(Point2D p1, Point2D p2);
	float vecLength(Vec2D vec);
	Vec2D normalize(Vec2D vec);
	Vec2D vectorMultiply(Vec2D vec, float scalar);
	Vec2D createVector(Point2D first, Point2D second);
	Point2D movePoint(Point2D point, Vec2D vec);
};

#endif