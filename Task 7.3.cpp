/*

 AUTHOR:		Taavi Kappak

 TASK:			Create a Shape type that can be a triangle (length of 3 sides as parameters), 
				a rectangle  (length of 2 sides) or an ellipse (axes) and contains 2 methods, 
				get_perimeter, get_area, that return its perimeter and area respectively.

 DATE:			18.07.2014

*/

#include <iostream>
#include <math.h> 

using std::cout;
using std::endl;

class Shape {
public:
	virtual double getPerimeter() = 0;
	virtual double getArea() = 0;
};
class Triangle : public Shape {
public:
	Triangle();
	Triangle(double one, double two, double three) {
		a = one;
		b = two;
		c = three;
	}
	double getPerimeter() {
		return a + b + c;
	}
	double getArea() {
		double s = ((a + b + c)/2);
		return sqrt(s*(s-a)*(s-b)*(s-c));
	}
private:
	double a, b, c;
};
class Rectangle : public Shape {
public:
	Rectangle();
	Rectangle(double one, double two) {
		a = one;
		b = two;
	}
	double getPerimeter() {
		return (2*(a + b));
	}
	double getArea() {
		return a * b;
	}
private:
	double a, b;
};
class Ellipse : public Shape {
public:
	Ellipse();
	Ellipse(double one, double two) {
		x = one;
		y = two;
	}
	double getPerimeter() {
		return (2 * 3.14 * sqrt((pow(x, 2) + pow(y, 2))/2));
	}
	double getArea() {
		return (3.14 * x * y);
	}
private:
	double x, y;
};
int main() {
	Shape *shape;
	Triangle triangle(3, 4, 5); // Triangle has to possible to create with real measurments
	Rectangle rectangle(4, 9.9);
	Ellipse ellipse(22, 10);

	shape = &triangle;
	cout << "Triangle perimeter: \t" << shape->getPerimeter() << endl;
	cout << "Triangle area: \t\t" << shape->getArea() << endl;
	shape = &rectangle;
	cout << "Rectangle perimeter: \t" << shape->getPerimeter() << endl;
	cout << "Rectangle area: \t" << shape->getArea() << endl;
	shape = &ellipse;
	cout << "Ellipse perimeter: \t" << shape->getPerimeter() << endl;
	cout << "Ellipse area: \t\t" << shape->getArea() << endl;
	return 0;
}