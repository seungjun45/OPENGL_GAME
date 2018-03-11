#include "main.h"


// Handling Individual Primitive

static void SelectAndScale(Primitives* TargetPrimitive, GLfloat scale_factor) {
	GLfloat center_x, center_y;
	Point centerLoc = TargetPrimitive->getMyLoc_center();
	center_x = centerLoc.x;
	center_y = centerLoc.y;
	glm::mat4 _SelectAndScale_ = glm::scale(glm::vec3(scale_factor));
	glm::mat4 Translate_to_Origin = glm::translate(glm::mat4(1.f), glm::vec3(-center_x, -center_y, 0.f));
	glm::mat4 Translate_back = glm::translate(glm::mat4(1.f), glm::vec3(center_x, center_y, 0.f));
	glm::mat4 _Operations_ = Translate_back * _SelectAndScale_*Translate_to_Origin;

	switch (TargetPrimitive->getMyType()) {
	case 1: // sphere, 288 vertices
		for (int i = 0; i < 288; i++) {
			glm::vec3 tmp = ((Sphere*)TargetPrimitive)->Sphere_vertices[i];
			((Sphere*)TargetPrimitive)->Sphere_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	case 2: // Cube, 16 vertices
		for (int i = 0; i < 16; i++) {
			glm::vec3 tmp = ((Cube*)TargetPrimitive)->Cube_vertices[i];
			((Cube*)TargetPrimitive)->Cube_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	case 3: // Diamond, 6 vertices
		for (int i = 0; i < 6; i++) {
			glm::vec3 tmp = ((Diamond*)TargetPrimitive)->Diamond_vertices[i];
			((Diamond*)TargetPrimitive)->Diamond_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	default:
		assert("Existing primitive error at Select and Scale");
	}
}

static void SelectAndTranslate(Primitives* TargetPrimitive, Point new_centerLoc) {
	Point Old_centerLoc = TargetPrimitive->getMyLoc_center();
	glm::mat4 _Operations_ = glm::translate(glm::mat4(1.f), glm::vec3(new_centerLoc.x - Old_centerLoc.x, new_centerLoc.y - Old_centerLoc.y, 0.f));

	switch (TargetPrimitive->getMyType()) {
	case 1: // sphere, 288 vertices
		for (int i = 0; i < 288; i++) {
			glm::vec3 tmp = ((Sphere*)TargetPrimitive)->Sphere_vertices[i];
			((Sphere*)TargetPrimitive)->Sphere_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	case 2: // Cube, 16 vertices
		for (int i = 0; i < 16; i++) {
			glm::vec3 tmp = ((Cube*)TargetPrimitive)->Cube_vertices[i];
			((Cube*)TargetPrimitive)->Cube_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	case 3: // Diamond, 6 vertices
		for (int i = 0; i < 6; i++) {
			glm::vec3 tmp = ((Diamond*)TargetPrimitive)->Diamond_vertices[i];
			((Diamond*)TargetPrimitive)->Diamond_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	default:
		assert("Existing primitive error at Select and Scale");
	}
	TargetPrimitive->setMyLoc_center(new_centerLoc);
}