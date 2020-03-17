#include<GL/glut.h>
#include "Object.h"
#include<string>
#include<cstring>
#include<cstdlib>
#include<iostream>
using namespace std;
//#include "Scene.h"
#define PI 3.141592653589793

float Resolution = 400;
Material* m[10];
int n_material = 10;

float fov = 60;
int rayTreeDepth = 4;
Light *background = new Light({0,0,0});
Light *ambient = new Light({0,0,0});

int n_lights = 2;
Light* L[10];

typedef Object* pObj;

int size = 10;
pObj Obj[10];

void init()
{
    glViewport(0, 0, Resolution, Resolution);
    glClearColor(0, 0, 0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, 1, 0.1, 400);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_POINT_SMOOTH);
}
void scene();
void render();
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    glTranslatef(-200,-200,-300);
    render();
    glFlush();
}

typedef Vec3<float> vec3f;
vec3f trace(Ray r, int depth)
{
    if(depth<=0)
        return background->intensity;
    
    float t=INFINITY;
    pObj object = Obj[0];
    for(int i=0; i<size; i++)
    {
        float temp = Obj[i]->intersect(r);
        if (temp < 0) temp = INFINITY;
        if (temp < t) {
            t = temp;
            object = Obj[i];
        }
    }
    
    if(t<0 || t== INFINITY)
        return background->intensity;
    
    vec3f p_t = r.v*t;//nearest intersection
    vec3f n=0; //normal
    if(object->getName()=="Sphere")
        n = p_t-object->getNormal();
    else
        n = object->getNormal();
    n.normalize();
    vec3f V = {0,0,-1};
    vec3f I=0;  //Color intensity
    
     //for light source
    I = I+ object->surface->k_d*ambient->intensity;
    for(int i=0; i<n_lights; i++)
    {
        Ray Shadow;
        bool isShadow=false;
        
        Shadow.v =  L[i]->location - p_t;
        Shadow.p_0 = p_t;
        Shadow.v.normalize();
        for(int i=0; i<size && isShadow==false; i++)
        {
            if(Obj[i]!=object && Obj[i]->intersect(Shadow) < t && Obj[i]->intersect(Shadow) > 0)
            {
                isShadow=true;
                break;
            }
        }
        vec3f L_dir = L[i]->location - p_t;
        L_dir.normalize();
        vec3f R = L_dir - 2*n*(n.dot(L_dir)); //specular reflection
        
        R.normalize();
        float R_dot_V = R.dot(V);
        
        //Calculate specular reflection
        if(!isShadow && R_dot_V>0)
            I = I+ L[i]->intensity * object->surface->k_s * (pow(R_dot_V,object->surface->q));
        
        //Diffuse reflection
        if(!isShadow && n.dot(L_dir)>0)
            I = I+L[i]->intensity*(object->surface->k_d*(n.dot(L_dir)));
    }
    
    if(depth>0 && object->surface->k_r>0 && t>0)
    {
//        specular reflection
        Ray R;
        if(object->getName()=="Plane")
            R.v= 2*n*(n.dot(r.v)) + r.v;
        else
            R.v= 2*n*(n.dot(-r.v)) + r.v;
        R.p_0 = p_t;//t*vec3f(0,0,1);
        
        vec3f Refl = trace(R, depth-1);

        I = I*(1-object->surface->k_r)+ Refl*object->surface->k_r;
    }
    
    bool inside = false;
    
    if(r.v.dot(n)>0) {n = -n; inside = true;}
    
    if(depth>0 && object->surface->k_t>0)
    {
        //refraction
        float eta = inside ? object->surface->k_t : 1.0/object->surface->k_t;
        //float cosi = -n.dot(r.v);
        float c1 = n.dot(-r.v);
        float c2 = sqrt(1 - eta*eta*(1-pow(c1,2)));
        vec3f Refr_dir = (-eta*r.v) + (eta*c1-c2)*n;
        Ray T;
        T.v = Refr_dir;
        T.p_0 = p_t;
        T.v.normalize();
        
        vec3f Refr = trace(T, depth-1);
        I = I+ Refr*object->surface->k_t;
    }
    
    return I;
}

void Scene1()
{
//Phong lighting, diffuse and specular reflections

Material* m1 = new Material{{0.1, 0.9, 0.8}, {0.9, 0.9, 0.9}, 20, 0, 0, 0}; //q=20
Material* m2 = new Material{{0.5, 1.0, 0.5}, {0.9, 0.9, 0.9}, 500, 0, 0, 0}; // q=500 (shiny)
Material* m3 = new Material{{0.8, 0.25, 0.25}, {0.5, 0.5, 0.5}, 1000, 0, 0, 0};
//q=1000 (very shiny)
Material* m4 = new Material{{0.3, 0.5, 0.8}, {0.7, 0.7, 0.7}, 32, 0, 0, 0};
   
    m[0] = m1;
    m[1] = m2;
    m[2] = m3;
    m[3] = m4;
n_material = 4;

fov = 60;
rayTreeDepth = 4;

n_lights = 2;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
    L[0] = l1;
    L[1] = l2;

typedef Object* pObj;
size = 4;

Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m[0]);    //less shiny sphere
Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m[1]);   //shiny sphere
Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m[2]);   //shiny sphere
Plane *p1= new Plane(0.0, -1, 0, 1, m[3]);

    Obj[0] = p1;
    Obj[1] = s1;
    Obj[2] = s2;
    Obj[3] = s3;
return;
}

void Scene2()
{
    //Reflection + Shadow
Material* m1 = new Material{{0.1, 0.9, 0.8}, {0.9, 0.9, 0.9}, 20, 0.5, 0, 0}; //q=20
Material* m2 = new Material{{0.5, 1.0, 0.5}, {0.9, 0.9, 0.9}, 500, 0.3, 0, 0}; // q=500 (shiny)
Material* m3 = new Material{{0.8, 0.25, 0.25}, {0.5, 0.5, 0.5}, 1000, 0.6, 0, 0};
//q=1000 (very shiny)
Material* m4 = new Material{{0.3, 0.5, 0.8}, {0.7, 0.7, 0.7}, 32, 0.5, 0, 0};
     m[0] = m1;
        m[1] = m2;
        m[2] = m3;
        m[3] = m4;
    n_material = 4;

    fov = 60;
    rayTreeDepth = 4;

    n_lights = 2;
    //Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
    Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
    Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
        L[0] = l1;
        L[1] = l2;

    typedef Object* pObj;
    size = 4;

    Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m[0]);    //less shiny sphere
    Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m[1]);   //shiny sphere
    Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m[2]);   //shiny sphere
    Plane *p1= new Plane(0.0, -1, 0, 1, m[3]);

        Obj[0] = p1;
        Obj[1] = s1;
        Obj[2] = s2;
        Obj[3] = s3;
    return;
}

void Scene3()
{
    //Successful demonstration of Only Refraction

Material* m1 = new Material{{0.4, 1.0, 0.4}, {0.9, 0.9, 0.9}, 20, 0, 0.9, 1.517}; //q=20
Material* m2 = new Material{{0.2, 0.2, 0.5}, {0.9, 0.9, 0.9}, 500, 0, 0.5, 1.33}; // q=500 (shiny)
Material* m3 = new Material{{0.8, 0.25, 0.25}, {0.5, 0.5, 0.5}, 1000, 0, 0.3, 1.47};
//q=1000 (very shiny)
Material* m4 = new Material{{0.3, 0.5, 0.8}, {0.7, 0.7, 0.7}, 32, 0, 0.2, 1};

 m[0] = m1;
    m[1] = m2;
    m[2] = m3;
    m[3] = m4;
n_material = 4;

fov = 60;
rayTreeDepth = 4;

n_lights = 2;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
    L[0] = l1;
    L[1] = l2;

typedef Object* pObj;
size = 4;

Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m[0]);    //less shiny sphere
Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m[1]);   //shiny sphere
Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m[2]);   //shiny sphere
Plane *p1= new Plane(0.0, -1, 0, 1, m[3]);
    Obj[0] = p1;
    Obj[1] = s1;
    Obj[2] = s2;
    Obj[3] = s3;

return;
}

void Scene4()
{
    //Uses 6 spheres and Ray Tree of depth 7, 3 light sources and one plane
Material* m1 = new Material{{0.99, 20.0/255, 147.0/255 }, {0.9, 0.9, 0.9}, 20, 0.5, 0.9, 1.517}; //q=20
Material* m2 = new Material{{0.2, 0.2, 0.5}, {0.9, 0.9, 0.9}, 500, 0.3, 0.5, 1.33}; // q=500 (shiny)
Material* m3 = new Material{{0.8, 0.25, 0.25}, {0.5, 0.5, 0.5}, 1000, 0.6, 0.3, 1.47};
//q=1000 (very shiny)
Material* m4 = new Material{{0.3, 0.5, 0.8}, {0.7, 0.7, 0.7}, 32, 0.4, 0.2, 1};
Material* m5 = new Material{{1, 1, 1}, {0, 0, 0}, 1000, 0, 0.9, 1.8};
Material* m6 = new Material{{1.0, 191.0/256, 0.3}, {0.7, 0.7, 0.7}, 32, 0.5, 0.2, 1.67};

    m[0] = m1;
    m[1] = m2;
    m[2] = m3;
    m[3] = m4;
    m[4] = m5;
    m[5] = m6;
n_material = 6;
    
fov = 60;
rayTreeDepth = 7;

n_lights = 3;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, -20}, {2.5, 2.5, 2.0});
Light *l2=new Light({-10, -10, -10}, {1, 1, 1});
Light *l3=new Light({0, -70, -100}, {1, 0, 0});
    L[0] = l1;
    L[1] = l2;
    L[2] = l3;

size = 7;

Sphere *s1 = new Sphere({1.5, 0.0, -5.0}, 1, m[0]);
Sphere *s2 = new Sphere({-3.4, -1.0, -12.0}, 2.0, m[1]);
Sphere *s3 = new Sphere({10, -10, -60}, 7, m[2]);
Sphere *s4 = new Sphere({0.0, -1.0, -2.0}, 3.0, m[4]);
Sphere *s5 = new Sphere({0.0, -1.0, -1.0}, 3.0, m[4]);
Sphere *s6 = new Sphere({0.0, -6.0, -24.0}, 3.0, m[5]);

Plane *p1= new Plane(0.0, -1, 0, 1, m[3]);
    Obj[0] = p1;
    Obj[1] = s1;
    Obj[2] = s2;
    Obj[3] = s3;
    Obj[4] = s4;
    Obj[5] = s5;
    Obj[6] = s6;

return;
}

int temp=1;
int parse_args(char* argv[])
{
    if(strcmp(argv[1],"1")==0)
        return 1;
    else if(strcmp(argv[1],"2")==0)
        return 2;
    else if(strcmp(argv[1],"3")==0)
        return 3;
    else if(strcmp(argv[1],"4")==0)
        return 4;
}
void scene()
{
    if(temp==1)
        Scene1();
    else if(temp==2)
        Scene2();
    else if(temp==3)
        Scene3();
    else if(temp==4)
        Scene4();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    cout<<argv[1]<<endl;
    temp=parse_args(argv);
    cout<<temp<<endl;
    scene();
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //Single buffer display
    glutInitWindowPosition(150,50);
    glutInitWindowSize(Resolution, Resolution);
    glutCreateWindow("Ray Tracing");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

typedef Vec3<float> vec3f;
void render()
{
    vec3f pixel = 0;
    float angle = tan(PI*fov/360.0);
    
    for(int i=0; i<Resolution; i++)
    {
        for(int j=0; j<Resolution; j++)
        {
            float vx = angle*(2.0*((i+0.5)/Resolution)-1);
            float vy = angle*(1-2.0*((j+0.5)/Resolution));
            Ray ray;
            ray.v = {vx, vy, -1.0};
            ray.v.normalize();
            ray.computeRay();
            pixel = trace(ray, rayTreeDepth); //rayTreeDepth define in scene
            
            //glColor3f(0.5, 0.5, 0.5);
            glColor3f(pixel.x, pixel.y, pixel.z);
            glBegin(GL_POINTS);
                glVertex3f(i, j, -1);
            glEnd();
        }
    }
}

