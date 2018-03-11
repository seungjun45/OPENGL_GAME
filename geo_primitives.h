#pragma once
#include<stdio.h>
#include<time.h>
#include<math.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <GL/glew.h>		
#include <gl/glut.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <FreeImage.h>



//Declare and Consturct geometric primitives



typedef struct _Point {
	GLfloat x;
	GLfloat y;
}Point;

typedef struct _to_remove {
	int x;
	int y;
	struct _to_remove* next = NULL;
}to_remove;





//============================================================================================================================

bool find_match(bool store);//  This function must be called to refresh whenever there sth to be updated in table ********************

//============================================================================================================================

void Swap_in_Prim_Table_Target(int TargetX, int TargetY, int MyX, int MyY);

//static int primitive_table[15][15] = { 0 }; // table that contains strategic information of game it-self

// Instead of detecting empty Prim_table index using primitive_table, we can use "if( get_Prim_Table(x,y) == NULL )"


static int score = 0;



class Primitives
{
public:
	Primitives();
	~Primitives();

protected:
	
	Point MyLoc_center; // Center-Of-Mass location of a primitive
	int Primitive_Type=0;

public:
	virtual void remove_(Point Original) = 0;
	
	virtual void move_to(Point Original, Point Target) = 0;
	void swap_primitive_inTable(Primitives** original, Primitives** target);// This function is called after "swap" animation
	void remove_primitive_inTable(to_remove* node_to_remove);// This function is called after removing animation
	Point getMyLoc_center();
	void setMyLoc_center(Point center);
	int getMyType();
	void setMyType(int type);
};


static Primitives*** Prim_table = new Primitives**[15]; // table to dock all information about primitives
 


class Sphere : public Primitives
{
public:
	Sphere(Point loc);
	glm::vec3 Sphere_vertices[288]; // since radius is fixed, pi and psi are only existing variables
	glm::fvec3 color[3];
	
	void remove_(Point Original){}
	void move_to(Point Original, Point Target) {}// set vertice ¶û ºñ½Á
};


class Cube : public Primitives
{
public:
	Cube(Point loc);
	glm::vec3 Cube_vertices[16];
	glm::fvec3 color[3];

	void remove_(Point Original) {}
	void move_to(Point Original, Point Target) {} // set vertice ¶û ºñ½Á
};


class Diamond : public Primitives
{
public:
	Diamond(Point loc);
	glm::vec3 Diamond_vertices[12];
	glm::fvec3 color[3];
	
	void remove_(Point Original) {}
	void move_to(Point Original, Point Target) {}// set vertice ¶û ºñ½Á
};

void Initialize_all_Primitives();

void Fill_In_Empty_Primitives();

void attach_primitive_to_Prim_table(Primitives* Primitive, int x, int y);

void remove_primitive_from_Prim_table(int x, int y);

Primitives* get_Prim_table(int x, int y);

void Nullify_Prim_table(int x, int y);

GLfloat SpotlightEffect(glm::vec3 LightSource, glm::vec3 CameraLocation, glm::vec3 ObjectVertex[3], Point ObjectCenter, GLfloat flash_radius);


