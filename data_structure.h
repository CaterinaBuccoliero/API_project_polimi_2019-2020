//
// Created by Caterina Buccoliero on 18/08/2020.
//

#ifndef PROJECTAPI_DATA_STRUCTURE_H
#define PROJECTAPI_DATA_STRUCTURE_H

#include "commands.h"
#include <stddef.h>

// struct states defines a state of the text editor
struct state_s {
    char **text;
    size_t allocated_amount;
    int rows_amount;
};

state* create_state();
void save_state(state **destination, state *source);
void load_state(state *destination, state *source);
void modify_state(state *s, command *c);
void print_state(state *s, print *stp);

#endif //PROJECTAPI_DATA_STRUCTURE_H
