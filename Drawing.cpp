#include "main.h"


static void _glVertex3f_(glm::vec3 input_) {
	glVertex3f(input_.x, input_.y, input_.z);
}

static void Draw_glVertex3f_Array_(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3) {
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < list_length; i++) {
		for (int j = 0; j < 3; j++) {
			glColor3f(ColorVec3[j].x, ColorVec3[j].y, ColorVec3[j].z);
			_glVertex3f_(vec3_list[(i + j) % list_length]);
		}
	}
	glEnd();
}

static void Draw_glVertex3f_Array_M(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3, Point centerLoc) {
	glBegin(GL_TRIANGLES);
	GLfloat center_x, center_y;
	center_x = centerLoc.x;
	center_y = centerLoc.y;
	glm::mat4 Translate_to_Origin = glm::translate(glm::mat4(1.f), glm::vec3(-center_x, -center_y, 0.f));
	glm::mat4 Rot_Primitives = glm::rotate((float)glfwGetTime() * 1.f, glm::vec3(0.f, 1.0f, 0.f));
	glm::mat4 Translate_back = glm::translate(glm::mat4(1.f), glm::vec3(center_x, center_y, 0.f));

	glm::mat4 _Operations_ = Translate_back * Rot_Primitives*Translate_to_Origin;


	for (int i = 0; i < list_length; i++) {
		for (int j = 0; j < 3; j++) {
			glColor3f(ColorVec3[j].x, ColorVec3[j].y, ColorVec3[j].z);
			glm::vec4 homo_vec = glm::vec4(vec3_list[(i + j) % list_length], 1.0f);
			homo_vec = _Operations_ * homo_vec;
			_glVertex3f_(glm::vec3(homo_vec));
		}
	}
	glEnd();
}


static void Draw_Prim_Table() { //Draw elements in Prim_Table. Actual center locations of each primitive are stored in each instace.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 15; y++) {
			if (get_Prim_table(x, y) != NULL) {
				switch (get_Prim_table(x, y)->getMyType())
				{
				case 1: // Sphere, 288 indices
					Draw_glVertex3f_Array_(((Sphere*)get_Prim_table(x, y))->Sphere_vertices, 288, ((Sphere*)get_Prim_table(x, y))->color);
					//Draw_glVertex3f_Array_M(((Sphere*)get_Prim_table(x, y))->Sphere_vertices, 288, ((Sphere*)get_Prim_table(x, y))->color, x,y);
					break;
				case 2: // Cube, 16 indices
						//Draw_glVertex3f_Array_(((Cube*)get_Prim_table(x, y))->Cube_vertices, 16, ((Cube*)get_Prim_table(x, y))->color);
					Draw_glVertex3f_Array_M(((Cube*)get_Prim_table(x, y))->Cube_vertices, 16, ((Cube*)get_Prim_table(x, y))->color, ((Cube*)get_Prim_table(x, y))->getMyLoc_center());
					break;
				case 3: // Diamond, 6 indices
						//Draw_glVertex3f_Array_(((Diamond*)get_Prim_table(x, y))->Diamond_vertices, 6, ((Diamond*)get_Prim_table(x, y))->color);
					Draw_glVertex3f_Array_M(((Diamond*)get_Prim_table(x, y))->Diamond_vertices, 6, ((Diamond*)get_Prim_table(x, y))->color, ((Diamond*)get_Prim_table(x, y))->getMyLoc_center());
					break;
				default:
					assert("Not correct primitive appeared! <Draw_Prim_Table>");
				}
			}


		}
	}
}