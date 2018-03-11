#pragma once
#include "main.h"
#include "shaders.h"
#include <FreeImage.h>

#define _CRT_SECURE_NO_WARNINGS

static void set_angle_zero() {
	accum_deg_y = 0.f;
	accum_deg_x = 0.f;
	del_x_cursor = 0.f;
	del_y_cursor = 0.f;
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		Initialize_all_Primitives();
		set_angle_zero();
		glfwSetTime(0.0f);
		score = 0;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		set_angle_zero();
	}
	
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (ctrlbutton_down)
		trans_del_x += 0.1f;
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (ctrlbutton_down)
		trans_del_x -= 0.1f;
	}
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (ctrlbutton_down)
		trans_del_y += 0.1f;
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if (ctrlbutton_down)
		trans_del_y -= 0.1f;
	}

	else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		Fill_In_Empty_Primitives();
	}
	else if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) {
			ctrlbutton_down = true;
		}
		else if(action == GLFW_RELEASE){
			ctrlbutton_down = false;
			set_angle_zero();
			scroll_offset = 0.f;
			lbutton_down = false;
			trans_del_x = 0.f;
			trans_del_y = 0.f;
		}
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLbyte color[4];
	GLfloat depth;
	GLuint index;

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (ctrlbutton_down) {
			if (GLFW_PRESS == action) {
				lbutton_down = true;
				glfwGetCursorPos(window, &original_cursor_x, &original_cursor_y);
			}
			else if (GLFW_RELEASE == action) {
				lbutton_down = false;
				accum_deg_x += del_x_cursor * 0.1f;
				del_x_cursor = 0.f;
				accum_deg_y += del_y_cursor * 0.1f;
				del_y_cursor = 0.f;
			}
		}
		else {
			glfwGetCursorPos(window, &original_cursor_x, &original_cursor_y); // Trying swapping operation
			int center_x = round(Win2Table_TransX(original_cursor_x,width));
			int center_y = round(Win2Table_TransY(original_cursor_y,height));

			if (GLFW_PRESS == action) {
				
				if (0 <= center_x && center_x <= 14 && 0 <= center_y  && center_y <= 14) {
					GAME_STARTED = true;
					lbutton_down = true;

					Primitive_On_Handling = get_Prim_table(center_x, center_y); // Target is on handling
					Prim_OnHandl_Loc_X = center_x;
					Prim_OnHandl_Loc_Y = center_y;
					SelectAndScale(Primitive_On_Handling, 1.25f);
					//printf("Selected at x:[%d] y:[%d], type: <%d>\n", center_x, center_y, get_Prim_table(center_x,center_y)->getMyType());
				}
			}
			else if (GLFW_RELEASE == action) {
				// swap operation here
				if (lbutton_down) {
					lbutton_down = false;
					if (0 <= Prim_OnHandl_Loc_X && Prim_OnHandl_Loc_X <= 14 && 0 <= Prim_OnHandl_Loc_Y && Prim_OnHandl_Loc_Y <= 14)
					{
						SelectAndScale(Primitive_On_Handling, 0.8f); // Shrink the selected primitive back to its original size
						SelectAndTranslate(Primitive_On_Handling, { (GLfloat)Prim_OnHandl_Loc_X, (GLfloat)Prim_OnHandl_Loc_Y }); // If swapping not occured
						Primitive_On_Handling = NULL;
					}
				}
			}
		}
	}

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	double new_cursor_x, new_cursor_y;
	glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);
	mouse_X_converted = Win2Table_TransX(new_cursor_x, width);
	mouse_Y_converted = Win2Table_TransY(new_cursor_y, height);
	

	if (ctrlbutton_down) { // Control button pressed (Rotation task)
		if (lbutton_down) {
			del_x_cursor = new_cursor_x - original_cursor_x;
			del_y_cursor = new_cursor_y - original_cursor_y;
		}
	}
	else { // Control button not pressed
		if (lbutton_down && (Primitive_On_Handling != NULL) ) // Mouse clicked!!! Drag and (If possible) swapping operation
		{
			
			//printf("mouse_X_converted : %f, mouse_Y_converted : %f\n", mouse_X_converted, mouse_Y_converted);
			SelectAndTranslate(Primitive_On_Handling, { mouse_X_converted, mouse_Y_converted });
			
			// Swapping test
			int new_Cx_int = round(mouse_X_converted);
			int new_Cy_int = round(mouse_Y_converted);
			if (0 <= new_Cx_int && new_Cx_int <= 14 && 0 <= new_Cy_int && new_Cy_int  <= 14) // Event call if-only mouse moving in valid domain
			{
				if (Prim_OnHandl_Loc_X != new_Cx_int || Prim_OnHandl_Loc_Y != new_Cy_int) { // location changed
					// Try Virtual Swap(in Prim table) and see if there is any change
					Primitives* target = get_Prim_table(new_Cx_int, new_Cy_int);

					for (int i = 0; i < 15; i++) {
						for (int j = 0; j < 15; j++) {
							dummy_table[i][j] = get_Prim_table(i, j)->getMyType();
						}
					}

					int tmp = dummy_table[new_Cx_int][new_Cy_int];
					dummy_table[new_Cx_int][new_Cy_int] = dummy_table[Prim_OnHandl_Loc_X][Prim_OnHandl_Loc_Y];
					dummy_table[Prim_OnHandl_Loc_X][Prim_OnHandl_Loc_Y] = tmp;
					
					// check
					if (find_match_dummy(dummy_table)) // If Swapping actually has a match
					{
						// Make actual change in real View
						SelectAndTranslate(target, { (GLfloat)Prim_OnHandl_Loc_X,(GLfloat)Prim_OnHandl_Loc_Y });
						SelectAndTranslate(Primitive_On_Handling, { (GLfloat)new_Cx_int,(GLfloat)new_Cy_int });
						Swap_in_Prim_Table_Target(new_Cx_int, new_Cy_int, Prim_OnHandl_Loc_X, Prim_OnHandl_Loc_Y);
					} 
					else { // Swapping not occured
						//Swap_in_Prim_Table_Target(Prim_OnHandl_Loc_X, Prim_OnHandl_Loc_Y, new_Cx_int, new_Cy_int);
						SelectAndTranslate(Primitive_On_Handling, { (GLfloat)Prim_OnHandl_Loc_X, (GLfloat)Prim_OnHandl_Loc_Y }); // If swapping not occured
					}
					lbutton_down = false;
					SelectAndScale(Primitive_On_Handling, 0.8f); // Shrink the selected primitive back to its original size
					
					Primitive_On_Handling = NULL;


				}
			}
			

			

		}
	}


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (ctrlbutton_down) {
		//if ( (-4 < scroll_offset && yoffset < 0)  || (yoffset > 0 && scroll_offset < 4) ) {
			scroll_offset += -yoffset * 0.1;
			printf("scroll offset : %f\n", scroll_offset);
		//}
	}

}



static void _glVertex3f_(glm::vec3 input_) {
	glVertex3f(input_.x, input_.y, input_.z);
}

static void Draw_glVertex3f_Array_(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3, Point centerLoc, glm::vec3 camera_location) {
	glBegin(GL_TRIANGLES);
	GLfloat _l_factor_;
	glm::vec3 _tmp_vec3_[3];
	for (int i = 0; i < list_length; i++) {
		for (int k = 0; k < 3; k++) {
			_tmp_vec3_[k] = vec3_list[(i + k) % list_length];
		}
		_l_factor_ = SpotlightEffect(camera_location, camera_location, _tmp_vec3_, centerLoc, flash_radius);
		for (int j = 0; j < 3; j++) {
			glColor3f(_l_factor_*ColorVec3[j].x, _l_factor_*ColorVec3[j].y, _l_factor_*ColorVec3[j].z);
			_glVertex3f_(_tmp_vec3_[j]);
		}
	}
	glEnd();
}

static void Draw_glVertex3f_Array_M(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3, Point centerLoc, glm::vec3 camera_location) {
	glBegin(GL_TRIANGLES);
	GLfloat center_x, center_y;
	center_x = centerLoc.x;
	center_y = centerLoc.y;
	glm::mat4 Translate_to_Origin = glm::translate(glm::mat4(1.f), glm::vec3(-center_x, -center_y, 0.f));
	glm::mat4 Rot_Primitives = glm::rotate((float)glfwGetTime() * 1.f, glm::vec3(0.f, 1.0f, 0.f));
	glm::mat4 Translate_back = glm::translate(glm::mat4(1.f), glm::vec3(center_x, center_y, 0.f));

	glm::mat4 _Operations_ = Translate_back * Rot_Primitives*Translate_to_Origin;

	GLfloat _l_factor_;
	glm::vec3 _tmp_vec3_[3];

	for (int i = 0; i < list_length; i++) {
		for (int k = 0; k < 3; k++) {
			
			glm::vec4 homo_vec = glm::vec4(vec3_list[(i + k) % list_length], 1.0f);
			homo_vec = _Operations_ * homo_vec;
			_tmp_vec3_[k] = glm::vec3(homo_vec);
		}
		_l_factor_ = SpotlightEffect(camera_location, camera_location, _tmp_vec3_,centerLoc, flash_radius);

		for (int j = 0; j < 3; j++) {
			glColor3f(_l_factor_*ColorVec3[j].x, _l_factor_*ColorVec3[j].y, _l_factor_*ColorVec3[j].z);
			_glVertex3f_(_tmp_vec3_[j]);
		}
	}
	glEnd();
}


static void Draw_Prim_Table(glm::vec3 camera_location) { //Draw elements in Prim_Table. Actual center locations of each primitive are stored in each instace.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 15; y++) {
			if (get_Prim_table(x, y) != NULL) {
				switch (get_Prim_table(x, y)->getMyType())
				{
				case 1: // Sphere, 288 indices
					Draw_glVertex3f_Array_(((Sphere*)get_Prim_table(x, y))->Sphere_vertices, 288, ((Sphere*)get_Prim_table(x, y))->color, ((Sphere*)get_Prim_table(x, y))->getMyLoc_center(),camera_location);
					//Draw_glVertex3f_Array_M(((Sphere*)get_Prim_table(x, y))->Sphere_vertices, 288, ((Sphere*)get_Prim_table(x, y))->color, x,y);
					break;
				case 2: // Cube, 16 indices
					//Draw_glVertex3f_Array_(((Cube*)get_Prim_table(x, y))->Cube_vertices, 16, ((Cube*)get_Prim_table(x, y))->color);
					Draw_glVertex3f_Array_M(((Cube*)get_Prim_table(x, y))->Cube_vertices, 16, ((Cube*)get_Prim_table(x, y))->color, ((Cube*)get_Prim_table(x, y))->getMyLoc_center(), camera_location);
					break;
				case 3: // Diamond, 6 indices
					//Draw_glVertex3f_Array_(((Diamond*)get_Prim_table(x, y))->Diamond_vertices, 12, ((Diamond*)get_Prim_table(x, y))->color);
					Draw_glVertex3f_Array_M(((Diamond*)get_Prim_table(x, y))->Diamond_vertices, 12, ((Diamond*)get_Prim_table(x, y))->color, ((Diamond*)get_Prim_table(x, y))->getMyLoc_center(), camera_location);
					break;
				default:
					assert("Not correct primitive appeared! <Draw_Prim_Table>");
				}
			}


		}
	}
}


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
			((Sphere*)TargetPrimitive)->Sphere_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp,1.0f));
		}
		break;
	case 2: // Cube, 16 vertices
		for (int i = 0; i < 16; i++) {
			glm::vec3 tmp = ((Cube*)TargetPrimitive)->Cube_vertices[i];
			((Cube*)TargetPrimitive)->Cube_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	case 3: // Diamond, 12 vertices
		for (int i = 0; i < 12; i++) {
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
		for (int i = 0; i < 12; i++) {
			glm::vec3 tmp = ((Diamond*)TargetPrimitive)->Diamond_vertices[i];
			((Diamond*)TargetPrimitive)->Diamond_vertices[i] = glm::vec3(_Operations_* glm::vec4(tmp, 1.0f));
		}
		break;
	default:
		assert("Existing primitive error at Select and Scale");
	}
	TargetPrimitive->setMyLoc_center(new_centerLoc);
}

void Refresh_Primitives() {

	while (to_remove_list != NULL) { // Collect scores
		to_remove* tmp = pop_up_to_remove(&to_remove_list);
		if (get_Prim_table(tmp->x, tmp->y) != NULL) {
			if (GAME_STARTED) {
				switch (get_Prim_table(tmp->x, tmp->y)->getMyType()) {
				case 1:
					score += 1; // increase score
					break;
				case 2:
					score += 2;
					break;
				case 3:
					score += 3;
					break;
				}
				
			}
			Nullify_Prim_table(tmp->x, tmp->y);
		}
		delete tmp;

		
	}

	bool move_down_occur = true;
	// Move down remaining Primitives

	while (move_down_occur) {
		move_down_occur = false;
		//	for (int time = 0; time < 3; time++) { // give animation effect (gravity fall down)
		for (int j = 14; j > 0; j--) {
			for (int i = 0; i < 15; i++) {
				if (get_Prim_table(i, j) != NULL && get_Prim_table(i, j - 1) == NULL) { // move down condition
					move_down_occur = true;
					SelectAndTranslate(get_Prim_table(i, j), { (GLfloat)i, (GLfloat)j - 1 });
					Swap_in_Prim_Table_Target(i, j - 1, i, j);
				}
			}
		}
	}
}

void append_to_remove_call(to_remove** list, int x, int y) {
	new_node = create_node(x, y);

	append_to_remove(list, new_node);
}

to_remove* create_node(int x, int y) {
	to_remove* new_node = new to_remove;
	new_node->x = x;
	new_node->y = y;
	new_node->next = NULL;
	return new_node;
}

void append_to_remove(to_remove** list, to_remove* node) {
	
	if (*list == NULL) {
		
		*list = node;
	}
	else {
		to_remove *Tail = *list;
		while (Tail->next != NULL) {
			Tail = Tail->next;
		}
		Tail->next = node;
	}
}

to_remove* pop_up_to_remove(to_remove** list) {
	to_remove* node = *list;
	*list = (*list)->next;
	return node;
}

bool find_match_dummy(int table[15][15]) {
	bool matched = false;
	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 15; y++) {
			if (2 <= x && x <= 12) {
				if (MultipleMatch6(dummy_table[x - 2][y], dummy_table[x - 1][y], dummy_table[x][y], dummy_table[x + 1][y], dummy_table[x + 2][y], 3)) {
					//printf("Dummy Call::Horizontal match at center: [%d][%d]\n", x, y);
					matched = true;
				}
			}
			if (2 <= y && y <= 12) {
				if (MultipleMatch6(dummy_table[x][y - 2], dummy_table[x][y - 1], dummy_table[x][y], dummy_table[x][y + 1], dummy_table[x][y + 2], 3)) {
					//printf("Dummy Call::Vertical match at center: [%d][%d]\n", x, y);
					matched = true;
				}
			}
			if (2 <= x && x <= 12 && 2 <= y && y <= 12) {
				if (MultipleMatch6(dummy_table[x - 2][y - 2], dummy_table[x - 1][y - 1], dummy_table[x][y], dummy_table[x + 1][y + 1], dummy_table[x + 2][y + 2], 2)) {
					//printf("Dummy Call::Right cross match at center: [%d][%d]\n", x, y);
					matched = true;
				}

				if (MultipleMatch6(dummy_table[x - 2][y + 2], dummy_table[x - 1][y + 1], dummy_table[x][y], dummy_table[x + 1][y - 1], dummy_table[x + 2][y - 2], 2)) {
					//printf("Dummy Call::Left cross match at center: [%d][%d]\n", x, y);
					matched = true;
				}
			}
		}
	}
	return matched;
}


bool append_if_matched(int x, int y, bool store) {
	//if (table[x - 1][y] == table[x][y] == table[x + 1][y]) {
	bool matched = false;
	if (2 <= x && x <= 12) {
		if (MultipleMatch6(get_Prim_table(x - 2, y)->getMyType(),get_Prim_table(x + 2, y)->getMyType(),
			get_Prim_table(x - 1, y)->getMyType(), get_Prim_table(x, y)->getMyType(), get_Prim_table(x + 1, y)->getMyType(), 3)) {
			//printf("Horizontal match at center: [%d][%d]\n",x,y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x + i, y);
			}
			matched = true;
		}
	}
	//if (table[x][y - 1] == table[x][y] == table[x][y + 1]) {
	if (2 <= y && y <= 12) {
		if (MultipleMatch6(get_Prim_table(x, y - 2)->getMyType() , get_Prim_table(x, y + 2)->getMyType() ,
			get_Prim_table(x, y - 1)->getMyType() , get_Prim_table(x, y)->getMyType() , get_Prim_table(x, y + 1)->getMyType() , 3)) {
			//printf("Vertical match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x, y + i);
			}
			matched = true;
		}
	}
	//if (table[x - 1][y - 1] == table[x][y] == table[x + 1][y + 1]) {
	if (2 <= x && x <= 12 && 2 <= y && y <= 12) {
		if (MultipleMatch6(get_Prim_table(x - 2, y - 2)->getMyType() , get_Prim_table(x + 2, y + 2)->getMyType() ,
			get_Prim_table(x - 1, y - 1)->getMyType() , get_Prim_table(x, y)->getMyType() , get_Prim_table(x + 1, y + 1)->getMyType() , 2 )) {
			//printf("Right cross match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x + i, y + i);
			}
			matched = true;
		}
		//if (table[x + 1][y - 1] == table[x][y] == table[x - 1][y + 1]) {
		if (MultipleMatch6(get_Prim_table(x + 2, y - 2)->getMyType() , get_Prim_table(x - 2, y + 2)->getMyType() ,
			get_Prim_table(x + 1, y - 1)->getMyType() , get_Prim_table(x, y)->getMyType() , get_Prim_table(x - 1, y + 1)->getMyType() , 2)) {
			//printf("Left cross match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x - i, y + i);
			}
			matched = true;
		}
	}
	return matched;
}


bool find_match(bool store) { //  This function must be called to refresh whenever there sth to be updated in table
	Fill_In_Empty_Primitives();
	/*printf("Prim_table type :\n");
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			printf("[%d]", get_Prim_table(i, j)->getMyType());
		}
		printf("\n");
	}*/
	bool match = false;
	for (int x_center = 0; x_center < 15; x_center++) {
		for (int y_center = 0; y_center < 15; y_center++) {
			if (append_if_matched(x_center, y_center,store))
				match = true;
		}
	}
	return match;
}

static void Draw_Score(int _score_) {
	char score_text[15];
	for (int j = 0; j < 15; j++) {
		score_text[j] = '-';
;	}
	GLfloat _tmp_;
	sprintf(score_text, "score : %d", _score_);
	glBegin(GL_QUADS);
	for (int i = 0; i < 15; i++) {
		_tmp_ = ((GLfloat) i);

		glMultiTexCoord2f(GL_TEXTURE0, UV_UP_LEFT_X(score_text[i]), UV_UP_LEFT_Y(score_text[i]));
		glVertex3f(7.5f - 5.0f + _tmp_, 16.0f, 0.0f);

		glMultiTexCoord2f(GL_TEXTURE0, UV_DOWN_LEFT_X(score_text[i]), UV_DOWN_LEFT_Y(score_text[i]));
		glVertex3f(7.5f - 5.0f + _tmp_, 15.0f, 0.0f);

		glMultiTexCoord2f(GL_TEXTURE0, UV_DOWN_RIGHT_X(score_text[i]), UV_DOWN_RIGHT_Y(score_text[i]));
		glVertex3f(7.5f - 4.0f + _tmp_, 15.0f, 0.0f);

		glMultiTexCoord2f(GL_TEXTURE0, UV_UP_RIGHT_X(score_text[i]), UV_UP_RIGHT_Y(score_text[i]));
		glVertex3f(7.5f - 4.0f + _tmp_, 16.0f, 0.0f);
	}
	glEnd();
}


static void Draw_Texture(int _score_) {
	glBegin(GL_QUADS);

	// Front face
	glMultiTexCoord2f(GL_TEXTURE1, 0.f, 1.0f);
	glVertex3f(0.f-2.5f, 20.0f - 2.5f, 3.0f);

	glMultiTexCoord2f(GL_TEXTURE1, 0.f, 0.f);
	glVertex3f(0.f - 2.5f, 0.f - 2.5f, 3.0f);

	glMultiTexCoord2f(GL_TEXTURE1, 1.f,0.f);
	glVertex3f(20.f - 2.5f,0.f - 2.5f, 3.0f);

	glMultiTexCoord2f(GL_TEXTURE1, 1.f, 1.f);
	glVertex3f(20.f - 2.5f, 20.0f - 2.5f, 3.0f);
	glEnd();

	char score_text[15];
	for (int j = 0; j < 15; j++) {
		score_text[j] = '-';
		;
	}
	GLfloat _tmp_;
	sprintf(score_text, "score : %d", _score_);
	glBegin(GL_QUADS);
	for (int i = 0; i < 15; i++) {
		_tmp_ = ((GLfloat)i);


		glMultiTexCoord2f(GL_TEXTURE1, UV_UP_LEFT_X(score_text[i]), UV_UP_LEFT_Y(score_text[i]));
		glVertex3f(7.5f - 6.0f + _tmp_ - 2.5f, 2.0f - 2.5f, 3.0f);

		glMultiTexCoord2f(GL_TEXTURE1, UV_DOWN_LEFT_X(score_text[i]), UV_DOWN_LEFT_Y(score_text[i]));
		glVertex3f(7.5f - 6.0f + _tmp_ - 2.5f, 1.0f - 2.5f, 3.0f);

		glMultiTexCoord2f(GL_TEXTURE1, UV_DOWN_RIGHT_X(score_text[i]), UV_DOWN_RIGHT_Y(score_text[i]));
		glVertex3f(7.5f - 5.0f + _tmp_ - 2.5f, 1.0f - 2.5f,-3.0f);

		glMultiTexCoord2f(GL_TEXTURE1, UV_UP_RIGHT_X(score_text[i]), UV_UP_RIGHT_Y(score_text[i]));
		glVertex3f(7.5f - 5.0f + _tmp_ - 2.5f, 2.0f - 2.5f, 3.0f);
	}
	glEnd();

}


int main(void)
{
	GLuint texture;
	glm::mat4 rot_camera1, rot_camera2, trans_camera;
	glm::vec4 camera_location_homo(0.f, 0.f, -5.f, 1.f);
	GLfloat scale_factor;
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(1080, 640, "Personal Project!!_ Seungjun Jung", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window); // pop up on current window
	if (glewInit()) { // Now it's a good time to initialize Xtension wranglers
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return 1;
	}
	glfwSwapInterval(1); // the number of screen updates to wait from the time glfwSwapBuffers was called before swapping the buffers and returning
	glfwSetKeyCallback(window, key_callback); // key_callback setup
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	FreeImage_Initialise();

	Initialize_all_Primitives(); // INITIAL GAME STARTER



	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glClearDepthf(1.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/vert.glsl");
	fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
	shaderprogram = initprogram(vertexshader, fragmentshader);
	
	inittexture_PNG("./Holstein.png", shaderprogram, 0);
	inittexture_PNG("./GAMEOVER.jpg", shaderprogram, 1);
	
	
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		
		

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height); // 뷰포트 변환은 클리핑 및 투영된 평면 이미지를 윈도우의 어디쯤에다 출력할 것인지를 지정한다.

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Background setup. Can load texture here



		//===============from world-view to camera-view==================================

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -10.f, 10.f); // Non-projective mapping based view. defining view frustrum
		//glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 10.f); // Projective mapping based view. Frustrum defined as below

		// 3D geometries are projected on this rectangle plane
		//
		//	[-ratio, 1.f]-------------------------[ratio, 1.f]
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//		|										|
		//	[-ratio, -1.f]-------------------------[ratio, -1.f]
		//
		//			     	< at z(depth) = 1 >
		//
		//===============from model-view to world-view==================================

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
		glRotatef(+(accum_deg_x+del_x_cursor * 0.1f), 0.f, 1.f, 0.f);
		glRotatef(+(accum_deg_y+del_y_cursor * 0.1f), 1.f, 0.f, 0.f);
		glTranslatef(-7.5f + trans_del_x, -7.5f + trans_del_y, 5.f+ scroll_offset);
		scale_factor = 5.f/(5.f + scroll_offset);
		//glScalef(scale_factor, scale_factor, scale_factor);

		rot_camera1 = glm::rotate((float) +(accum_deg_x + del_x_cursor * 0.1f), glm::vec3({ 0.f, 1.f, 0.f }));
		rot_camera2 = glm::rotate((float)+(accum_deg_y + del_y_cursor * 0.1f), glm::vec3({ 1.f, 0.f, 0.f }));
		camera_location_homo.x = mouse_X_converted;
		camera_location_homo.y = mouse_Y_converted;
		//printf("mouse x:[%f], y: [%f]\n", camera_location_homo.x, camera_location_homo.y, camera_location_homo.z);
		
		camera_location_homo.z = 5.f + scroll_offset;


		// *************************************"PRESS N TO START A NEW GAME"*******************************************
		find_match(true);
		Refresh_Primitives();
		flash_radius = 200.f / (1 + glm::pow( glm::max((GLfloat) (glfwGetTime()- score/4.f),0.0f), 2)*0.02f);
		

		if (flash_radius < 1.6f) {
			
			Draw_Texture(score);
			printf("****************************************************************\n");
			printf("****************************************************************\n");
			printf("\t\t\tFinal Score : %d\n", score);
			printf("****************************************************************\n");
			printf("****************************************************************\n");
			printf("G A M E\t\t O V E R\n");
		}
		else {
			printf("glTime : %f\n", glfwGetTime());
			printf("score : %d\n", score);
			Draw_Prim_Table(glm::vec3(rot_camera2*rot_camera1*camera_location_homo));

			//SCORE TEXTURE MAPPING
			Draw_Score(score);
			//Draw_Texture();
		}
		



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	FreeImage_DeInitialise();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}