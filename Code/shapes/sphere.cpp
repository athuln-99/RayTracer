#include "sphere.h"
#include <iostream>

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (position) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    Point dis = ray.O - position;

    // solve with the abc-rule
    double a = ray.D.dot(ray.D);
    double b = 2.0 * dis.dot(ray.D);
    double c = dis.dot(dis) - pow(r, 2.0);

    double disc = pow(b, 2.0) - 4.0 * a * c;

    bool inside = false;

    // The ray doesn't interset the sphere because the discriminant is lower than 0; therefore, the sphere is behind the ray's origin.
    if (disc < 0) {
      return Hit::NO_HIT();
    } else if (disc == 0) {
      inside = true;
    }

    // discriminant of a quadratic equation
    double t1 = (-b + sqrt(disc)) / (2.0 * a);
    double t2 = (-b - sqrt(disc)) / (2.0 * a);

    // At most 2 answers for t
    // We take the smallest positive value for t as the hit.
    double t;
    if (t1 < t2) { 
        t = t1;
        if (t < 0) t = t2;
    } else {
        t = t2;
        if (t < 0) t = t1;
    }

    // double t;
    // if (t1 < 0 && t2 < 0) {
    //   return Hit::NO_HIT();
    // } else if (t1 < 0 && t2 > 0) {
    //   t = t2;
    // } else if (t1 > 0 && t2 < 0) {
    //   t = t1;
    // } else if (t1 > 0 && t2 > 0) {
    //   if (t1 < t2) {
    //     t = t1;
    //   } else {
    //     t = t2;
    //   }
    // }

    // If no t is positive, the sphere is behind the ray's origin, so no intersection
    if (t < 0) return Hit::NO_HIT();

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, position, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    // sphere's normal at the intersection point
    Vector N = (ray.at(t) - position).normalized();

    if (inside) {
      // std::cout << "NEW LIN" << " ";
      N.x = N.x * -1.0;
      N.y = N.y * -1.0;
      N.z = N.z * -1.0;
    }

    return Hit(t, N);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}
