#include "main.h"


void Refresh_Primitives() {

	while (to_remove_list != NULL) { // Collect scores
		to_remove* tmp = pop_up_to_remove(&to_remove_list);
		Nullify_Prim_table(tmp->x, tmp->y);
		delete tmp;
		if (GAME_STARTED)
			score += 1; // increase score
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
			if (MultipleMatch6(dummy_table[x - 2][y], dummy_table[x - 1][y], dummy_table[x][y], dummy_table[x + 1][y], dummy_table[x + 2][y], 3))
				matched = true;
			else if (MultipleMatch6(dummy_table[x][y - 2], dummy_table[x][y - 1], dummy_table[x][y], dummy_table[x][y + 1], dummy_table[x][y + 2], 3))
				matched = true;
			else if (MultipleMatch6(dummy_table[x - 2][y - 2], dummy_table[x - 1][y - 1], dummy_table[x][y], dummy_table[x + 1][y + 1], dummy_table[x + 2][y + 2], 2))
				matched = true;
			else if (MultipleMatch6(dummy_table[x - 2][y + 2], dummy_table[x - 1][y + 1], dummy_table[x][y], dummy_table[x + 1][y - 1], dummy_table[x + 2][y - 2], 2))
				matched = true;
		}
	}
	return matched;
}


bool append_if_matched(int x, int y, bool store) {
	//if (table[x - 1][y] == table[x][y] == table[x + 1][y]) {
	bool matched = false;
	if (2 <= x && x <= 12) {
		if (MultipleMatch6(get_Prim_table(x - 2, y)->getMyType(), get_Prim_table(x + 2, y)->getMyType(),
			get_Prim_table(x - 1, y)->getMyType(), get_Prim_table(x, y)->getMyType(), get_Prim_table(x + 1, y)->getMyType(), 3)) {
			printf("Horizontal match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x + i, y);
			}
			matched = true;
		}
	}
	//if (table[x][y - 1] == table[x][y] == table[x][y + 1]) {
	if (2 <= y && y <= 12) {
		if (MultipleMatch6(get_Prim_table(x, y - 2)->getMyType(), get_Prim_table(x, y + 2)->getMyType(),
			get_Prim_table(x, y - 1)->getMyType(), get_Prim_table(x, y)->getMyType(), get_Prim_table(x, y + 1)->getMyType(), 3)) {
			printf("Vertical match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x, y + i);
			}
			matched = true;
		}
	}
	//if (table[x - 1][y - 1] == table[x][y] == table[x + 1][y + 1]) {
	if (2 <= x && x <= 12 && 2 <= y && y <= 12) {
		if (MultipleMatch6(get_Prim_table(x - 2, y - 2)->getMyType(), get_Prim_table(x + 2, y + 2)->getMyType(),
			get_Prim_table(x - 1, y - 1)->getMyType(), get_Prim_table(x, y)->getMyType(), get_Prim_table(x + 1, y + 1)->getMyType(), 2)) {
			printf("Right cross match at center: [%d][%d]\n", x, y);
			if (store) {
				for (int i = -2; i < 3; i++)
					append_to_remove_call(&to_remove_list, x + i, y + i);
			}
			matched = true;
		}
		//if (table[x + 1][y - 1] == table[x][y] == table[x - 1][y + 1]) {
		if (MultipleMatch6(get_Prim_table(x + 2, y - 2)->getMyType(), get_Prim_table(x - 2, y + 2)->getMyType(),
			get_Prim_table(x + 1, y - 1)->getMyType(), get_Prim_table(x, y)->getMyType(), get_Prim_table(x - 1, y + 1)->getMyType(), 2)) {
			printf("Left cross match at center: [%d][%d]\n", x, y);
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
			if (append_if_matched(x_center, y_center, store))
				match = true;
		}
	}
	return match;
}