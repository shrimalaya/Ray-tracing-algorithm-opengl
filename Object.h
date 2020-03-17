#include "Ray.h"
#include<string>
using namespace std;

typedef Vec3<float> vec3f;
struct Material{
    vec3f k_d, k_s; //d = diffuse reflection, s=specular reflection coeff
    float q, k_r, k_t, ior; //q = specular exponent, k_r= reflection coeff, k_t = refraction,
                            //ior = indexOfRefraction
};

void findRoots(float a,float b,float c, float *s)
{
    if(sqrt(pow(b,2)-4*a*c)<0)
        return;
    s[0] = (-1*b + sqrt(pow(b,2)-4*a*c))/2*a;
    s[1] = (-1*b - sqrt(pow(b,2)-4*a*c))/2*a;
    for(int i=0; i>2; i++)
        if(s[i]<0)
            s[i]=INFINITY;
    return;
}

float findMin(float *s, int size)
{
    float min=INFINITY;
    for(int i=0; i<size; i++)
        if(s[i]<=min)
            min=s[i];
    return min;
}


class Object {
public:
    struct Material* surface;
    virtual float intersect(Ray t) const= 0;
    virtual string getName() const=0;
    virtual vec3f getNormal() const=0;
};

typedef Vec3<float> vec3f;
class Sphere : public Object{
public:
    vec3f center={0,0,0};
    float radius=1.0;
    //struct Material* surface;
    Sphere(vec3f center, float radius, struct Material* material): center(center), radius(radius){this->surface=material;}
    Sphere(){surface = new Material{{0.1, 0.9, 0.8}, {0.9, 0.9, 0.9}, 32, 1.31, 0, 1.5};}
    float intersect(Ray r) const
    {
        vec3f q = r.p_0;
        q=q-center;
        vec3f a = r.v*r.v;
        vec3f b = 2*r.v*q;
        
        vec3f c = q*q;
        
        float A=a.x+a.y+a.z;
        float B=b.x+b.y+b.z;
        float C=c.x+c.y+c.z - pow(radius, 2);
        
        float s[2] = {INFINITY};
        findRoots(A, B, C, s);
        
        float min=-1;
        float temp = findMin(s, 2);
        if(temp!=INFINITY)
            min=temp;
        
        return min;
    }
    string getName() const
    {
        return "Sphere";
    }
    vec3f getNormal() const
    {
        return this->center;
    }
};

class Plane: public Object{
public:
    float A=0;
    float B=1;
    float C=0;
    float D=1;
    //struct Material* surface;
    Plane(float A, float B, float C, float D, struct Material* material):A(A), B(B), C(C), D(D){this->surface=material;}
    Plane(){surface = new Material{{0.1, 0.9, 0.8}, {0.9, 0.9, 0.9}, 32, 1.31, 0, 1.5};}
    float intersect(Ray r) const
       {
           float a= A*r.v.x + B*r.v.y + C*r.v.z;
           float b= A*r.p_0.x + B*r.p_0.y + C*r.p_0.z + D;
           
           float t=-1.0*b/a;
           
           if(t>=0)
               return t;
           else
               return -1;
       }
    string getName() const
    {
        return "Plane";
    }
    vec3f getNormal() const
    {
        return vec3f(A,B,C);
    }
};

typedef Vec3<float> vec3f;
class Light {
public:
    vec3f location = {0,0,0};
    vec3f intensity = {2,2,2};
    Light(vec3f location,vec3f intensity) {this->location=location; this->intensity=intensity;}
    Light(vec3f intensity) {this->intensity=intensity;}
};

