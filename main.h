#pragma once
#include "geo_primitives.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// ** ========================= BASIC MACRO FUNCTIONS ======================================= ** //

#define Win2Table_TransX(winX,winWidth) winX / winWidth * 20 - 10.0f + 7.5f
#define Win2Table_TransY(winY,winHeight) (winHeight - winY) / winHeight * 20 - 10.0f + 7.5f


#define MultipleMatch(a,b,c,d,e) (a == b) && (b == c) && (c == d) && (d == e)
#define MultipleMatch6(a,b,c,d,e,f) (a == b) && (b == c) && (c == d) && (d == e) && (e != f)

#define UV_X(character) (character%16)/16.0f
#define UV_Y(character) (character/16)/16.0f
#define UV_UP_LEFT_X(character) UV_X(character)
#define UV_UP_LEFT_Y(character) 1.0f-UV_Y(character)
#define UV_UP_RIGHT_X(character) UV_X(character)+1.0f/16.0f
#define UV_UP_RIGHT_Y(character) 1.0f-UV_Y(character)
#define UV_DOWN_RIGHT_X(character) UV_X(character)+1.0f/16.0f
#define UV_DOWN_RIGHT_Y(character) 1.0f-(UV_Y(character)+1.0f/16.0f)
#define UV_DOWN_LEFT_X(character) UV_X(character)
#define UV_DOWN_LEFT_Y(character) 1.0f-(UV_Y(character)+1.0f/16.0f)

#define FAILED(hr) (((HRESULT)(hr)) < 0)


// ** ========================= MISCELLANEOUS VARIABLES ======================================= ** //

static bool GAME_STARTED = false;

static GLfloat trans_del_x = 0.f;
static GLfloat trans_del_y = 0.f;

static GLfloat flash_radius = 0.0f;

static to_remove *to_remove_list = NULL;
static to_remove *new_node = NULL;

static double scroll_offset =0.0;

int dummy_table[15][15] = { 0 };

static int width, height; // window width and height

static bool lbutton_down = false, ctrlbutton_down = false;

static double original_cursor_x, original_cursor_y, del_x_cursor = 0.f, del_y_cursor = 0.f;
static double accum_deg_x = 0.f, accum_deg_y = 0.f;

static Primitives* Primitive_On_Handling;

static int Prim_OnHandl_Loc_X=NULL;
static int Prim_OnHandl_Loc_Y=NULL;



static GLfloat mouse_X_converted = 0.f;
static GLfloat mouse_Y_converted = 0.f;


GLuint vertexshader, fragmentshader, shaderprogram; // shaders


GLubyte woodtexture[256][256][3]; // ** NEW ** texture (from grsites.com)
GLuint texNames[1]; // ** NEW ** texture buffer
GLuint istex;  // ** NEW ** blend parameter for texturing
			   //GLuint islight; // ** NEW ** for lighting
GLint texturing = 1; // ** NEW ** to turn on/off texturing

#include "geometry3.h"



// ** ========================= CALL BACK FUNCTIONS ======================================= ** //

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);



// ** ========================= VERTEX && FRAGMENT SHADING FUNCTIONS ======================================= ** //

static void _glVertex3f_(glm::vec3 input_);
static void Draw_glVertex3f_Array_(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3, Point centerLoc, glm::vec3 camera_location);
static void Draw_glVertex3f_Array_M(glm::vec3* vec3_list, int list_length, glm::vec3* ColorVec3, Point centerLoc, glm::vec3 camera_location);
static void Draw_Prim_Table(glm::vec3 camera_location);



// ** ========================= INDIVIDUAL PRIMITIVE(INSTANCE) HANDLING FUNCTIONS ======================================= ** //

static void SelectAndTranslate(Primitives* TargetPrimitive, Point new_centerLoc);
static void SelectAndScale(Primitives* TargetPrimitive, GLfloat scale_factor);


// ** ========================= GAME HANDLING FUNCTIONS ======================================= ** //

void Refresh_Primitives();
void append_to_remove(to_remove** list, to_remove* node);
to_remove* pop_up_to_remove(to_remove** list);
bool append_if_matched(int x, int y, bool store);
to_remove* create_node(int x, int y);
bool find_match_dummy(int table[15][15]);


// ** ========================= DISPLAY HANDLING FUNCTIONS ======================================= ** //

static void Draw_Score(int _score_);
