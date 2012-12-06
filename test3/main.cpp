#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Vector3.h"
#include <math.h>

#define GRAVITY  9.81
#define dt  0.01

// ---- Prototypes ----
typedef struct{
    
    float x;
    float y;
    float z;
    float m;
    int id;
    Vector3 speed;
    
}Particle;

void Render(void);
void createParticles(void);
void calculateForce(void);
float distance(float a, float b);
float RandomFloat(float a, float b);
void move(void);



const int numberParticles =  100;
float pointSize = 2;
float coeffs[] = {1.0f, .0f, 0.0001f};
int width = 800;
int height = 600;

Particle particles[numberParticles];

// ---- Main Function ----
int main(int argc, char** argv)
{
    
	glutInit(&argc,argv); 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);  
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0,0);
    createParticles();
	glutCreateWindow("GlutApp"); 
    
    
    glutDisplayFunc(Render);
	//glutIdleFunc(move);
	glutMainLoop();
}

// ---- Render Function ----
void Render(void)
{   
    glViewport(0,0,800,600);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(45.0f,800.0f/600.0f,1.0f,500.0f);
    glMatrixMode(GL_MODELVIEW);
    glEnable (GL_POINT_SMOOTH); 
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glFrustum ( 10, 10, -10, 10, 10, 10 );
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(pointSize);
    
    //Les coefficients d'atténuation à appliquer 
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, coeffs);
    
    //Taille du point
    glPointParameterf (GL_POINT_SIZE_MAX, 32.0f);
    glPointParameterf (GL_POINT_SIZE_MIN, 1.0f);
    
	glBegin(GL_POINTS);
    
    for(int i=0; i<numberParticles;i++){
        glVertex3f(particles[i].x, particles[i].y, particles[i].z);
    }
    
	glEnd();
    
    glViewport(0, 0, (GLsizei) width,  (GLsizei) height);
    
	glFlush();
    
}

void createParticles(void){
    srand(time(NULL));
    for(int i = 0; i < numberParticles; i++){
        particles[i].x = RandomFloat(-1.0f, 1.0f);
        particles[i].y = RandomFloat(-1.0f, 1.0f);
        particles[i].z = RandomFloat(-1.0f, 1.0f);
        particles[i].m = 0.2f;
        particles[i].speed = RandomFloat(0, 1.0f);
        particles[i].id = i;
        std::cout<<"x: "<<particles[i].x<<"y: "<<particles[i].y<<"z: "<<particles[i].z<<std::endl;
    }
    
    //particles[0].m = 2.0f;
    
        
    
}

Vector3 calculateForces(Particle p){
    Vector3 force = (0.0f, 0.0f, 0.0f);
    for(int i = 0; i < numberParticles; i++){
        if(particles[i].id != p.id){
        force.x += (particles[i].m*p.m*GRAVITY)/pow(distance(particles[i].x,p.x), 2);
        force.y += (particles[i].m*p.m*GRAVITY)/pow(distance(particles[i].y,p.y), 2);
        force.z += (particles[i].m*p.m*GRAVITY)/pow(distance(particles[i].z,p.z), 2);
        std::cout<<"pow x-----" << pow(distance(particles[i].x,p.x), 2)<<std::endl;
        std::cout<<"force x: "<<force.x<<" force y: "<<force.y<<" force z: "<<force.z<<std::endl;
        }
    }
    
    return force;
    
}


float distance(float a, float b){
    return b - a;
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void move(void){
    for(int i= 0; i < numberParticles;i++){
        Vector3 previousPosition = (particles[i].x, particles[i].y, particles[i].z);
        Vector3 forceTot = calculateForces(particles[i]);
        particles[i].x = particles[i].x+particles[i].speed.x*dt+(((1/2)*forceTot.x)/particles[i].m)*pow(dt, 2);
        particles[i].y = particles[i].y+particles[i].speed.y*dt+(((1/2)*forceTot.y)/particles[i].m)*pow(dt, 2);
        particles[i].z = particles[i].z+particles[i].speed.z*dt+(((1/2)*forceTot.z)/particles[i].m)*pow(dt, 2);
        particles[i].speed.x = distance(particles[i].x, previousPosition.x)/dt;
        particles[i].speed.y = distance(particles[i].y, previousPosition.y)/dt;
        particles[i].speed.z = distance(particles[i].z, previousPosition.z)/dt;
        std::cout<<"x: "<<particles[i].x<<" y: "<<particles[i].y<<" z: "<<particles[i].z<<std::endl;
        std::cout<<"speed x: "<<particles[i].speed.x<<" speed y: "<<particles[i].speed.y<<" speed z: "<<particles[i].speed.z<<std::endl;
    }
    
    glutPostRedisplay();

}

