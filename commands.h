#include <ntsid.h>
//
// Created by Caterina Buccoliero on 18/08/2020.
//

#ifndef PROJECTAPI_COMMANDS_H
#define PROJECTAPI_COMMANDS_H
#include "definitions.h"
#include "data_structure.h"

// struct command_s defines a command with its type, an interval of lines on the document and the text
struct command_s {
    char type;
    int first_number;
    int last_number;
    char **text;
};

// struct print_s defines a print with an interval of lines and a text
struct print_s {
    int first_number;
    int last_number;
    char **text;
};

// struct print_node_list_s defines a list of elements (nodes) to print, with their value
struct print_node_list_s {
    struct print_node_list_s *next;
    print *val;
};

// struct print_node_list_dc_s defines a doubly linked list of nodes to print with their value
struct print_node_list_dc_s{
    struct print_node_list_dc_s *next;
    struct print_node_list_dc_s *prev;
    print *val;
};

// struct command_node_tree_s defines the nodes of the tree that constitutes the data structure
// each node has a command, a list of children, one parent, the child whose branch is currently being extended,
// a state and a list of elements to print
struct command_node_tree_s{
    command *current_command;
    children_node_list *children;
    struct command_node_tree_s *parent;
    children_node_list *current_child;
    struct state_s *current_state;
    print_node_list *print_head;
};

// struct children_node_list_s defines a list of children of the same parent
struct children_node_list_s{
    struct command_node_tree_s *val;
    struct children_node_list_s *next;
};

// struct register_print_s defines all prints as it contains a reference to the first and last one of the doubly linked
//list of prints
struct register_print_s{
    print_node_list_dc *head;
    print_node_list_dc *last;
};

// struct stack_s defines a stack of nodes (it will be used to store the intersection nodes)
struct stack_s{
    command_node_tree **vector;
    int top, allocated_amount;
};

///////////////////////////////////////////////////////////////////////////////////

print_node_list * print_push(print_node_list *head, print *val);
print_node_list_dc * print_dc_push(print_node_list_dc *head, print *val);

///////////////////////////////////////////////////////////////////////////////////

command_node_tree * command_node_create(command *current_command);

void command_node_tree_add(command_node_tree *parent, command_node_tree *child);

children_node_list * children_push(children_node_list *head, command_node_tree *val);

void add_print (command_node_tree *current_node, print *current_print);

void execute_print (state *current_state, print_node_list *print_head);

command_node_tree * command_node_scan (command_node_tree *starting_node, int moves);

///////////////////////////////////////////////////////////////////////////////////

register_print * register_create ();

void register_add(register_print *register_p, print *val);


///////////////////////////////////////////////////////////////////////////////////

stack * stack_allocate();

void stack_push(stack *p, command_node_tree *n);

command_node_tree * stack_pop(stack *p);

command_node_tree * stack_top(stack *p);

int stack_empty(stack *p);

///////////////////////////////////////////////////////////////////////////////////

void execute(command_node_tree *root);
void complete_print(register_print *reg);

#endif //PROJECTAPI_COMMANDS_H