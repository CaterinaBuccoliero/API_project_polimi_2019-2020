//
// Created by Caterina Buccoliero on 18/08/2020.
//

#ifndef PROJECTAPI_READING_H
#define PROJECTAPI_READING_H
#include "commands.h"
#include "definitions.h"

command read_command ();
void complete_reading (command_node_tree *root, register_print *reg);

#endif //PROJECTAPI_READING_H

