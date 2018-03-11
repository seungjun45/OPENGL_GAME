#define GLM_ENABLE_EXPERIMENTAL
#include "geo_primitives.h"


#define M_PI       3.14159265358979323846   // pi
#define Rad2Deg(rad) rad*M_PI/180


#define BUFFER_OFFSET(bytes) ((GLubyte *) NULL + (bytes))
#define NumberOf(array) (sizeof(array)/sizeof(array[0])) 

void Swap_in_Prim_Table_Target(int TargetX, int TargetY, int MyX, int MyY) { //  This function must be called to refresh whenever there sth to be updated in table

	Primitives** Target_Addr = &Prim_table[TargetX][TargetY];
	Primitives** My_Addr = &Prim_table[MyX][MyY];
	Primitives* _tmp_Pointer_;
	_tmp_Pointer_ = *My_Addr;
	*My_Addr = *Target_Addr;
	*Target_Addr = _tmp_Pointer_;

}


Primitives::Primitives()
{
}

Primitives::~Primitives()
{
}



void Primitives::remove_primitive_inTable(to_remove* node_to_remove) { // This function is called after removing animation
	//primitive_table[node_to_remove->x][node_to_remove->y] = 0; // type 0 is a node to be deleted
	Nullify_Prim_table(node_to_remove->x, node_to_remove->y);
	delete node_to_remove;
}

void Primitives::swap_primitive_inTable(Primitives** original, Primitives** target) { // This function is called after "swap" animation

	/*int temp = primitive_table[(int) original.x][(int)original.y];
	primitive_table[(int)original.x][(int)original.y] = primitive_table[(int)target.x][(int)target.y];
	primitive_table[(int)target.x][(int)target.y] = temp;*/
	Primitives* tmp_pointer;
	tmp_pointer = *original;
	*original = *target;
	*target = tmp_pointer;

}

Point Primitives::getMyLoc_center() {
	return this->MyLoc_center;
}

void Primitives::setMyLoc_center(Point center) {
	this->MyLoc_center = center;
}

int Primitives::getMyType() {
	return this->Primitive_Type;
}
void Primitives::setMyType(int type) {
	this->Primitive_Type = type;
}



Sphere::Sphere(Point loc) {
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			float del_x, del_y, del_z;
			del_x = 0.5*sin(Rad2Deg(30 * i))*cos(Rad2Deg(30 * j));
			del_y = 0.5*sin(Rad2Deg(30 * i))*sin(Rad2Deg(30 * j));
			del_z = 0.5*cos(Rad2Deg(30 * j));
			this->Sphere_vertices[i * 24 +2* j] = glm::vec3(loc.x+del_x, loc.y+del_y, del_z);
			this->Sphere_vertices[i * 24 + 2*j+1] = glm::vec3(loc.x - del_x, loc.y - del_y, del_z);
		}
	}
	this->MyLoc_center.x = loc.x;
	this->MyLoc_center.y = loc.y;

	//primitive_table[(int)loc.x][(int)loc.y] = 1;

	this->color[0] = glm::fvec3(0.1f, 0.6f, 0.8f);
	this->color[1] = glm::fvec3(0.4f, 0.1f, 0.8f);
	this->color[2] = glm::fvec3(0.7f, 0.0f, 0.8f);
	
	this->setMyType(1);
}

Cube::Cube(Point loc) {
	this->Cube_vertices[0] = glm::vec3(loc.x - 0.5, loc.y - 0.5, -0.5);
	this->Cube_vertices[1] = glm::vec3(loc.x + 0.5, loc.y - 0.5, -0.5);
	this->Cube_vertices[2] = glm::vec3(loc.x - 0.5, loc.y - 0.5, +0.5);
	this->Cube_vertices[3] = glm::vec3(loc.x + 0.5, loc.y - 0.5, +0.5);
	this->Cube_vertices[4] = glm::vec3(loc.x - 0.5, loc.y + 0.5, +0.5);
	this->Cube_vertices[5] = glm::vec3(loc.x + 0.5, loc.y + 0.5, +0.5);
	this->Cube_vertices[6] = glm::vec3(loc.x - 0.5, loc.y + 0.5, -0.5);
	this->Cube_vertices[7] = glm::vec3(loc.x + 0.5, loc.y + 0.5, -0.5);
	this->Cube_vertices[8] = glm::vec3(loc.x + 0.5, loc.y + 0.5, +0.5);
	this->Cube_vertices[9] = glm::vec3(loc.x + 0.5, loc.y - 0.5, +0.5);
	this->Cube_vertices[10] = glm::vec3(loc.x + 0.5, loc.y + 0.5, -0.5);
	this->Cube_vertices[11] = glm::vec3(loc.x + 0.5, loc.y - 0.5, -0.5);
	this->Cube_vertices[12] = glm::vec3(loc.x - 0.5, loc.y + 0.5, -0.5);
	this->Cube_vertices[13] = glm::vec3(loc.x - 0.5, loc.y - 0.5, -0.5);
	this->Cube_vertices[14] = glm::vec3(loc.x - 0.5, loc.y + 0.5, +0.5);
	this->Cube_vertices[15] = glm::vec3(loc.x - 0.5, loc.y - 0.5, +0.5);

	this->MyLoc_center.x = loc.x;
	this->MyLoc_center.y = loc.y;

	//primitive_table[(int)loc.x][(int)loc.y] = 2;

	this->color[0] = glm::fvec3(0.9f, 0.2f, 0.4f);
	this->color[1] = glm::fvec3(0.6f, 0.5f, 0.1f);
	this->color[2] = glm::fvec3(0.7f, 0.3f, 0.1f);

	this->setMyType(2);
}


Diamond::Diamond(Point loc) {

	this->Diamond_vertices[0] = glm::vec3(loc.x - 0.5, loc.y, 0.0f);
	this->Diamond_vertices[1] = glm::vec3(loc.x, loc.y - 0.5, 0.0f);
	this->Diamond_vertices[2] = glm::vec3(loc.x, loc.y, -0.5f);
	this->Diamond_vertices[3] = glm::vec3(loc.x + 0.5, loc.y, 0.0f);
	this->Diamond_vertices[4] = glm::vec3(loc.x, loc.y + 0.5, 0.0f);
	this->Diamond_vertices[5] = glm::vec3(loc.x, loc.y, 0.5f);
	this->Diamond_vertices[6] = glm::vec3(loc.x + 0.5, loc.y, 0.0f);
	this->Diamond_vertices[7] = glm::vec3(loc.x, loc.y - 0.5, 0.0f);
	this->Diamond_vertices[8] = glm::vec3(loc.x, loc.y, 0.5f);
	this->Diamond_vertices[9] = glm::vec3(loc.x - 0.5, loc.y, 0.0f);
	this->Diamond_vertices[10] = glm::vec3(loc.x, loc.y + 0.5, 0.0f);
	this->Diamond_vertices[11] = glm::vec3(loc.x, loc.y, -0.5f);


	this->MyLoc_center.x = loc.x;
	this->MyLoc_center.y = loc.y;

	//primitive_table[(int)loc.x][(int)loc.y] = 3;

	this->color[0] = glm::fvec3(0.0f, 0.1f, 0.4f);
	this->color[1] = glm::fvec3(0.4f, 0.0f, 0.1f);
	this->color[2] = glm::fvec3(0.1f, 0.7f, 0.1f);

	this->setMyType(3);
}




void Initialize_all_Primitives() {
	int primitive;
	srand(time(NULL));
	for (int i = 0; i < 15; i++) {
		Prim_table[i] = new Primitives*[15];
		for (int j = 0; j < 15; j++) {

			primitive = (rand() % 3) + 1;
			switch (primitive)
			{
			case 1:
				//generate sphere at (i,j)
				Prim_table[i][j] = new Sphere({(GLfloat) i,(GLfloat) j });
				break;
			case 2:
				//generate cube at (i,j)
				Prim_table[i][j] = new Cube({ (GLfloat) i,(GLfloat) j });
				break;
			case 3:
				//generate diamond at (i,j)
				Prim_table[i][j] = new Diamond({ (GLfloat) i,(GLfloat) j });
				break;
				//
			default:
				assert("primitive generation error");
			}
		}
	}
}

void Fill_In_Empty_Primitives() {
	int primitive;
	srand(time(NULL));
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {

			if (Prim_table[i][j] == NULL) { // MAKE A NEW PRIMITIVE IFF THE INDEX IS NOT FILLED
				primitive = (rand() % 3) + 1;
				switch (primitive)
				{
				case 1:
					//generate sphere at (i,j)
					Prim_table[i][j] = new Sphere({ (GLfloat)i,(GLfloat)j });
					break;
				case 2:
					//generate cube at (i,j)
					Prim_table[i][j] = new Cube({ (GLfloat)i,(GLfloat)j });
					break;
				case 3:
					//generate diamond at (i,j)
					Prim_table[i][j] = new Diamond({ (GLfloat)i,(GLfloat)j });
					break;
					//
				default:
					assert("primitive generation error");
				}
			}

		}
	}
}

void attach_primitive_to_Prim_table(Primitives* Primitive, int x, int y) {
	Prim_table[x][y] = Primitive;
}

void remove_primitive_from_Prim_table(int x, int y) {
	// fading away animation here
	delete Prim_table[x][y];
}

Primitives* get_Prim_table(int x, int y) {
	return Prim_table[x][y];
}

void Nullify_Prim_table(int x, int y) {
	
		delete Prim_table[x][y];
		Prim_table[x][y] = NULL;

}

GLfloat SpotlightEffect(glm::vec3 LightSource, glm::vec3 CameraLocation, glm::vec3 SurfaceVertex[3], Point ObjectCenter, GLfloat flash_radius) {
	glm::vec3 _vec1_ = SurfaceVertex[1] - SurfaceVertex[0];
	glm::vec3 _vec2_ = SurfaceVertex[2] - SurfaceVertex[1];
	glm::vec3 _N_ =glm::normalize( glm::cross(_vec1_, _vec2_));
	
	glm::vec3 Surface_Center(SurfaceVertex[0] + SurfaceVertex[1] + SurfaceVertex[2]);
	Surface_Center.x /= 3.0f;
	Surface_Center.y /= 3.0f;
	Surface_Center.z /= 3.0f;
	glm::vec3 Object_Center(ObjectCenter.x, ObjectCenter.y, 0.f);

	if (glm::dot(Surface_Center- Object_Center, _N_)<0)
		_N_ = -_N_;

	//glm::vec3 _N_ = glm::normalize(Surface_Center - Object_Center);

	
	glm::vec3 _L_ = glm::normalize(LightSource - Surface_Center);
	//CameraLocation = glm::vec3(7.5f, 7.5f, -5.f);
	
	/*if (_L_.z > 0)
		_L_ = -_L_;*/

	glm::vec3 _I_ = glm::normalize(CameraLocation - Surface_Center);
	/*if (_I_.z > 0)
		_I_ = -_I_;*/
	
	GLfloat _ambient_ = 0.5f;

	GLfloat _diffuse_ =	 glm::max(glm::dot(_L_, _N_), 0.0f );

	glm::vec3 _R_ = 2.0f * _diffuse_ * _N_ - _L_;

	GLfloat _specular_ = glm::pow(glm::dot(_R_, _I_), 4.0f);
	
	//return  _specular_*2;

	//glm::vec3 center_vertex((SurfaceVertex[0] + SurfaceVertex[1] + SurfaceVertex[2]) / 3.0f);
	
	if (glm::pow(ObjectCenter.x - LightSource.x,2) + glm::pow(ObjectCenter.y - LightSource.y,2) < (flash_radius))
		return _ambient_+_diffuse_*0.3+0.3*_specular_;
	else
		return 0.0f;

}


