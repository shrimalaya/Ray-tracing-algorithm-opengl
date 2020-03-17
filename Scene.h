
Material* m[10];
int n_material = 10;

float fov = 60;
int rayTreeDeoth = 4;
Light *background = new Light({0,0,0});
Light *ambient = new Light({0,0,0});

int n_lights = 2;
Light* L[10];

typedef Object* pObj;

int size = 10;
pObj Obj[10];


void Scene1()
{
//Phong lighting, diffuse and specular reflections

Material* m1 = new Material{{0.1, 0.9, 0.8}, {0.9, 0.9, 0.9}, 20, 0, 0, 0}; //q=20
Material* m2 = new Material{{0.5, 1.0, 0.5}, {0.9, 0.9, 0.9}, 500, 0, 0, 0}; // q=500 (shiny)
Material* m3 = new Material{{0.8, 0.25, 0.25}, {0.5, 0.5, 0.5}, 1000, 0, 0, 0};
//q=1000 (very shiny)
Material* m4 = new Material{{0.3, 0.5, 0.8}, {0.7, 0.7, 0.7}, 32, 0, 0, 0};
*m = {m1, m2, m3, m4};
n_material = 4;

fov = 60;
rayTreeDepth = 4;

n_lights = 2;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
*L = {l1, l2};

typedef Object* pObj;
size = 4;

Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m1);    //less shiny sphere
Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m2);   //shiny sphere
Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m3);   //shiny sphere
Plane *p1= new Plane(0.0, -1, 0, 1, m4);

*Obj = {s1, p1, s2, s3};
render();
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
    *m = {m1, m2, m3, m4};
    n_material = 4;

    fov = 60;
    rayTreeDepth = 4;

    n_lights = 2;
    //Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
    Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
    Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
    *L = {l1, l2};

    typedef Object* pObj;
    size = 4;

    Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m1);    //less shiny sphere
    Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m2);   //shiny sphere
    Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m3);   //shiny sphere
    Plane *p1= new Plane(0.0, -1, 0, 1, m4);

    *Obj = {s1, p1, s2, s3};
    render();
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

*m = {m1, m2, m3, m4};
n_material = 4;

fov = 60;
rayTreeDepth = 4;

n_lights = 2;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, 0}, {2.5, 2.5, 2.0});
Light *l2=new Light({-7, -10, -10}, {1, 1, 1});
*L = {l1, l2};

typedef Object* pObj;
size = 4;

Sphere *s1 = new Sphere({1.5, 0.0, -7.0}, 1, m1);    //less shiny sphere
Sphere *s2 = new Sphere({-4.0, -1.0, -14.0}, 2.0, m2);   //shiny sphere
Sphere *s3 = new Sphere({0.0, -6.0, -20.0}, 3.0, m3);   //shiny sphere
Plane *p1= new Plane(0.0, -1, 0, 1, m4);

*Obj = {s1, p1, s2, s3};
render();
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

*m = {m1, m2, m3, m4, m5, m6};
n_material = 6;
    
fov = 60;
rayTreeDepth = 7;

n_lights = 3;
//Light *l2=new Light({50, 10, -20}, {2.5, 2.5, 2.0});
Light *l1=new Light({10, -15, -20}, {2.5, 2.5, 2.0});
Light *l2=new Light({-10, -10, -10}, {1, 1, 1});
Light *l3=new Light({0, -70, -100}, {1, 0, 0});
*L = {l1, l2, l3};

size = 7;

Sphere *s1 = new Sphere({1.5, 0.0, -5.0}, 1, m1);
Sphere *s2 = new Sphere({-3.4, -1.0, -12.0}, 2.0, m2);
Sphere *s3 = new Sphere({10, -10, -60}, 7, m3);
Sphere *s4 = new Sphere({0.0, -1.0, -2.0}, 3.0, m5);
Sphere *s5 = new Sphere({0.0, -1.0, -1.0}, 3.0, m5);
Sphere *s6 = new Sphere({0.0, -6.0, -24.0}, 3.0, m6);

Plane *p1= new Plane(0.0, -1, 0, 1, m4);

*Obj = {s1, p1, s2, s3, s4, s5, s6};
render();
return;
}
