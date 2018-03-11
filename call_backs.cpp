#include "main.h"



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
		accum_deg_y = 0.f;
		accum_deg_x = 0.f;
		del_x_cursor = 0.f;
		del_y_cursor = 0.f;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		accum_deg_y = 0.f;
		accum_deg_x = 0.f;
		del_x_cursor = 0.f;
		del_y_cursor = 0.f;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		trans_del_x += 0.1f;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		trans_del_x -= 0.1f;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		trans_del_y += 0.1f;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		trans_del_y -= 0.1f;
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		Fill_In_Empty_Primitives();
	}
	else if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) {
			ctrlbutton_down = true;
		}
		else if (action == GLFW_RELEASE) {
			ctrlbutton_down = false;
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
			int center_x = round(Win2Table_TransX(original_cursor_x, width));
			int center_y = round(Win2Table_TransY(original_cursor_y, height));

			if (GLFW_PRESS == action) {
				GAME_STARTED = true;

				lbutton_down = true;
				if (0 <= center_x && center_x <= 14 && 0 <= center_y && center_y <= 14) {
					Primitive_On_Handling = get_Prim_table(center_x, center_y); // Target is on handling
					Prim_OnHandl_Loc_X = center_x;
					Prim_OnHandl_Loc_Y = center_y;
					SelectAndScale(Primitive_On_Handling, 1.25f);
					printf("Selected at x:[%d] y:[%d], type: <%d>\n", center_x, center_y, get_Prim_table(center_x, center_y)->getMyType());
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
	if (ctrlbutton_down) { // Control button pressed (Rotation task)
		if (lbutton_down) {
			glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);
			del_x_cursor = new_cursor_x - original_cursor_x;
			del_y_cursor = new_cursor_y - original_cursor_y;
		}
	}
	else { // Control button not pressed
		if (lbutton_down && (Primitive_On_Handling != NULL)) // Mouse clicked!!! Drag and (If possible) swapping operation
		{
			glfwGetCursorPos(window, &new_cursor_x, &new_cursor_y);
			GLfloat new_center_x = Win2Table_TransX(new_cursor_x, width);
			GLfloat new_center_y = Win2Table_TransY(new_cursor_y, height);
			//printf("new_center_x : %f, new_center_y : %f\n", new_center_x, new_center_y);
			SelectAndTranslate(Primitive_On_Handling, { new_center_x, new_center_y });

			// Swapping test
			int new_Cx_int = round(new_center_x);
			int new_Cy_int = round(new_center_y);
			if (0 <= new_Cx_int && new_Cx_int <= 14 && 0 <= new_Cy_int && new_Cy_int <= 14) // Event call if-only mouse moving in valid domain
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