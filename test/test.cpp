#include <iostream>
#include <algorithm>
#include <cmath>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


using namespace std;
const double PI = 3.141592653589793;
struct Point {
    double x, y, z;
};
struct Vector {
    double x, y, z;
};
class Shape {
public:
    virtual Point curve_point(double t) const = 0;
    virtual Vector curve_vector(double t) const = 0;
};
class Circle :public Shape {
    double _radii;
public:
    Circle(double radii) { _radii = radii; };
    virtual Point curve_point(double t) const override
    {
        Point point = Point{  _radii * sin(t),_radii * cos(t),0.0 };
        return point;
    }
    virtual Vector curve_vector(double t) const override
    {
        Vector vector = Vector{ -1 * _radii * sin(t),_radii * cos(t) };
        return vector;
    }
    double getRadius() { return _radii; };

};
class Elipse :public Shape {
    double _radiix,_radiiy;
public:
    Elipse(double radiix, double radiiy) { _radiix = radiix; _radiiy = radiiy; };
    virtual Point curve_point(double t) const override
    {
        Point point = Point{ _radiix * sin(t),_radiix * cos(t), 0.0 };
        return point;
    }
    virtual Vector curve_vector(double t) const override
    {
        Vector vector = Vector{ -1 * _radiix * sin(t), _radiiy * cos(t) };
        return vector;
    }
};
class Helix :public Shape {
    double _radii, _step;
public:
    Helix(double radii, double step) { _radii = radii; _step = step; };
    virtual Point curve_point(double t) const override
    {
        Point point = Point{ _radii * t * cos(t), _radii * t * sin(t), _step * t };
        return point;
    }
    virtual Vector curve_vector(double t) const override
    {
        Vector vector = Vector{ _radii * (cos(t) - t * sin(t)), _radii * (sin(t) + t * cos(t)), _step };
        return vector;
    }
};
int getRandomNumber(int min, int max)
{
    static constexpr double fraction{ 1.0 / (RAND_MAX + 1.0) };
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}
using PShape = shared_ptr<Shape>;
using PCircle = shared_ptr<Circle>;
class MakingRandomShape
{
    
public:
    PShape circle()
    { 
        double radii;
        radii = getRandomNumber(0, 10);
        return make_shared<Circle>(radii);
    }

    PShape elipse()
    {   double radiix,radiiy;
        radiix = getRandomNumber(0, 10);
        radiiy = getRandomNumber(0, 10);
        return make_shared<Elipse>(radiix, radiiy);
    }

    PShape helix()
    {   
        double radii,step;
        radii = getRandomNumber(0, 10);
        step = getRandomNumber(0, 10);
     
        return make_shared<Helix>(radii, step);
    }

    PShape randomShape()
    {   
        int randompick = getRandomNumber(0, 2);
        switch (randompick)
        {
        case 0:
            return circle();
        case 1:
            return elipse();
        case 2:
            return helix();
        default:
            return circle(); 
        }
    }
};

int main()
{
    srand(time(NULL));
    list<PShape> shapes;
    list<PCircle> circles;
    MakingRandomShape rs;
    double radiusSumOfCircles = 0.0;
    for (int i = 0;i < 10;i++) {
        shapes.push_back(rs.randomShape());
    }
    cout << "==================================================================================" << endl;
    cout << "First Container" << endl;
    for (auto pos : shapes) {
        const type_info& ti{ typeid(*pos) };
        Point point = pos->curve_point(PI / 4);
        Vector vector = pos->curve_vector(PI / 4);
        cout << ti.name()<< "\tPoint:" << "\tx:"<< point.x << "\ty:" << point.y<< "\tz:"<<point.z << "\n\t\tVector:" << "\tx:" << vector.x << "\ty:" << vector.y << "\tz:" << vector.z << endl;
        if (ti.hash_code() == typeid(Circle).hash_code())
        {
            circles.push_back(dynamic_pointer_cast<Circle>(pos));
            
        }
    }
    cout << "==================================================================================" << endl;
    if (!circles.empty())
    {
        cout << "Second Container" << endl;
//#pragma omp parallel for reduction(+:radiusSumOfCircles)
        for (auto pos : circles)
        {

            const type_info& ti{ typeid(*pos) };
            Point point = pos->curve_point(PI / 4);
            Vector vector = pos->curve_vector(PI / 4);

            radiusSumOfCircles += pos->getRadius();
            cout << ti.name() << "\tPoint:" << "\tx:" << point.x << "\ty:" << point.y << "\tz:" << point.z << "\tradius:" << pos->getRadius() << "\n\t\tVector:" << "\tx:" << vector.x << "\ty:" << vector.y << "\tz:" << vector.z<< endl;
        }
        cout << "Sum of Radius Circles: "<<radiusSumOfCircles << endl;
        circles.sort([](const auto& lh, const auto& rh) {
            return lh->getRadius() < rh->getRadius();
            });

        cout << "==================================================================================" << endl;
        cout << "Sorted Circles" << endl;
        for (auto& pos : circles)
        {
            const type_info& ti{ typeid(*pos) };
            cout << ti.name() << " " << pos->getRadius() << endl;
        }
    }
    else cout << "Zero circles" << endl;

       

}

