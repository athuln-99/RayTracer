#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>

using namespace std;

Color Scene::trace(Ray const &ray)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          // the hit objects material
    Point hit = ray.at(min_hit.t);              // the hit point
    Vector N = min_hit.N;                       // the normal at hit point
    Vector V = -ray.D;                          // the view vector

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector + Vector    vector sum
    *        Vector - Vector    vector difference
    *        Point - Point      yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double * Color     scales each color component (r,g,b)
    *        Color * Color      ditto
    *        pow(a,b)           a to the power of b
    ****************************************************/

    // ambient component I_A of the Phone illumination model
    Color ambient = material.color * material.ka;

    // test the normal calculation before adding the diffuse component
    // Color color = ambient;
    // color = (N + 1) / 2;
    
    Vector L = (lights[0]->position - hit).normalized();
    Color lightColor = lights[0]->color;

    Color diffuse;

    //Check if the normal is pointing in the direction of the camera/eye
    if (N.dot(V) > 0) {
        diffuse = L.dot(N) * material.color * lights[0]->color * material.kd;
    } else {
        diffuse = N.dot(-L) * material.color * lights[0]->color * material.kd;
    }

    Vector R =  ((2 * (N.dot(L)) * N) - L).normalized();
    Color specular = (R.dot(V)) * lightColor * material.ks;

    Color color = ambient + diffuse + specular;

    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
            Ray ray(eye, (pixel - eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x, y) = col;
        }
    }
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
