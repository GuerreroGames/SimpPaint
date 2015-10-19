#include "GraphicsMathLib.h"
#include <stdio.h>
#include <math.h>



Point2D::Point2D(){
	x = 0.0;
	y = 0.0;
}

Point2D::Point2D(float inX, float inY){
	x = inX;
	y = inY;
}




Vec2D::Vec2D(){
	x = 0.0;
	y = 0.0;
	magnitude = 0.0;
}

Vec2D::Vec2D(float inX, float inY, float mag){
	x = inX;
	y = inY;
	magnitude = mag;
}

float Vec2D::calcMag(){
	float mag = sqrt( pow(x, 2) + pow(y, 2) );
	magnitude = mag;
	return round(mag);
}




Colour::Colour(){
	r = 0.0;
	g = 0.0;
	b = 0.0;
}

Colour::Colour(float red, float green, float blue){
	r = red;
	g = green;
	b = blue;
}




MathLib::MathLib(){}

float MathLib::distBetween(Point2D p1, Point2D p2){
	float dist = sqrt( pow((p2.x - p1.x), 2)
					 + pow((p2.y - p1.y), 2) ); 
	return dist;
}

float MathLib::fastDist(Point2D p1, Point2D p2){
	float dist = ( (p2.x - p1.x) * (p2.x - p1.x) )
			   + ( (p2.y - p1.y) * (p2.y - p1.y) );
	return dist;
}

float MathLib::vecLength(Vec2D vec){
	vec.calcMag();
	return vec.magnitude;
}

Vec2D MathLib::normalize(Vec2D vec){
	float length = vec.calcMag();

	float normX = vec.x / length;
	float normY = vec.y / length;

	Vec2D normVec = Vec2D(normX, normY, 0.0);
	// update magnitude 
	normVec.calcMag();

	return normVec;
}

Vec2D MathLib::vectorMultiply(Vec2D vec, float scalar){
	// v * S = (S * x, S * y)
	float multX = vec.x * scalar;
	float multY = vec.y * scalar;

	Vec2D multVec = Vec2D(multX, multY, 0.0);
	// update magnitude
	multVec.calcMag();
	return multVec;
}

Vec2D MathLib::createVector(Point2D first, Point2D second){
	// p1p2 = (x2 - x1, y2 - y1)
	float x = second.x - first.x;
	float y = second.y - first.y;
	Vec2D vec = Vec2D(x, y, 0.0);

	// update magnitude
	vec.calcMag();
	return vec;
}

Point2D MathLib::movePoint(Point2D point, Vec2D vec){
	float x = point.x + vec.x;
	float y = point.y + vec.y;
	Point2D movedPoint = Point2D(x, y);
	return movedPoint;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *		TESTING
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
int main(int argc, char* argv[]){

	MathLib testLib = MathLib();

	Point2D point1 = Point2D(0, 0);
	Point2D point2 = Point2D(4, -3);
	printf("the distance between is: %f\n", testLib.distBetween(point1, point2));
	printf("the distance between is: %f\n\n", testLib.fastDist(point1, point2));

	Vec2D testVec = Vec2D(4, 3, 0);
	printf("the value at x: %f\nthe value at y: %f\nthe magnitude: %f\n\n", testVec.x, testVec.y, testVec.magnitude);

	printf("the magnitude is: %f\n\n", testLib.vecLength(testVec));

	printf("the magnitude is: %f\n\n", testVec.magnitude);

	Vec2D normVec = testLib.normalize(testVec);
	printf("the value at x: %f\nthe value at y: %f\nthe magnitude: %f\n\n", normVec.x, normVec.y, normVec.magnitude);

	Vec2D multVec = testLib.vectorMultiply(testVec, 10);
	printf("the value at x: %f\nthe value at y: %f\nthe magnitude: %f\n\n", multVec.x, multVec.y, multVec.magnitude);

	Vec2D createVec = testLib.createVector(point1, point2);
	printf("the value at x: %f\nthe value at y: %f\nthe magnitude: %f\n\n", createVec.x, createVec.y, createVec.magnitude);

	Point2D movedPoint = testLib.movePoint(point1, testVec);
	printf("the value at x: %f\nthe value at y: %f\n", movedPoint.x, movedPoint.y);
}
*/