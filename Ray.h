#include "Vec3.h"
#include "cstdlib"

typedef Vec3<float> vec3f;
class Ray
{
public:
    vec3f p_0 = {0.0, 0, 0};
    vec3f v = {0.0, 0, -1.0};
    float t=1.0;
    vec3f p_t;
    
    void computeRay();
    
};

void Ray::computeRay()
{
    p_t = p_0 + t*v;
    return;
}
