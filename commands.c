#include "commands.h"
#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>

//this function pushes a new print node in a list of print nodes
print_node_list * print_push(print_node_list *head, print *val)
{
    print_node_list *new_node = malloc(sizeof(print_node_list));
    new_node -> val = val;
    new_node -> next = head;
    return new_node;
}

//this function pushes a new print node in a doubly linked list of print nodes
print_node_list_dc * print_dc_push(print_node_list_dc *head, print *val)
{
    print_node_list_dc *new_node = malloc(sizeof(print_node_list_dc));
    new_node ->val = val;
    new_node ->prev = NULL;
    new_node ->next = head;
    if(head)
    {
        head ->prev = new_node;
    }
    return new_node;
}

//this function pushes a new children in a list of children nodes
children_node_list * children_push(children_node_list *head, command_node_tree *val)
{
    children_node_list *new_node = malloc(sizeof(children_node_list));
    new_node -> val = val;
    new_node ->next = head;
    return new_node;
}

//this function creates a new node from a command
command_node_tree * command_node_create(command *current_command)
{
    command_node_tree *new_node = malloc(sizeof(command_node_tree));
    new_node ->current_command = current_command;
    new_node ->current_child = NULL;
    new_node ->current_state = NULL;
    new_node ->children = NULL;
    new_node ->parent = NULL;
    new_node ->print_head = NULL;
    return new_node;
}

//this function adds a new node to the tree, linking it to its parent and vice versa
void command_node_tree_add(command_node_tree *parent, command_node_tree *child)
{
    parent ->children = children_push(parent->children, child);
    parent ->current_child = parent->children;
    child->parent = parent;
}

//this function adds a print to the current node
void add_print (command_node_tree *current_node, print *current_print)
{
    current_node ->print_head = print_push(current_node->print_head, current_print);
}

//this function executes a single print on a state
void execute_print (state *current_state, print_node_list *print_head)
{
    while(print_head)
    {
        print_state(current_state, print_head->val);
        print_head = print_head->next;
    }
}

//this function scans the tree based on the number of moves inserted.
//if this amount is positive it moves down selecting the current children among the children
//if this amount is negative it moves upwards selecting the parent
command_node_tree * command_node_scan (command_node_tree *starting_node, int moves)
{
    command_node_tree *current_node = starting_node;
    if (moves >= 0)
    {
        while(moves != 0 && current_node && current_node->current_child)
        {
            current_node = current_node->current_child->val;
            moves--;
        }
    }
    else
    {
        while(moves != 0 && current_node && current_node->parent)
        {
            current_node = current_node->parent;
            moves++;
        }
    }
    return current_node;
}

//this function creates an empty register of prints
register_print * register_create ()
{
    register_print *r= malloc(sizeof(register_print));
    r ->head = NULL;
    r ->last = NULL;
    return r;
}

//this function adds a print to the beginning of a print register
void register_add(register_print *register_p, print *val)
{
    if(register_p->head == NULL)
    {
        register_p->head = print_dc_push(register_p->head, val);
        register_p->last = register_p->head;
    }
    else
    {
        register_p->head = print_dc_push(register_p->head, val);
    }
}

//this function allocates the space for a future stack
stack * stack_allocate()
{
    stack *new_stack = malloc(sizeof(stack));
    new_stack->top = -1;
    new_stack->vector = malloc(sizeof(command_node_tree*) * STACK_STARTING_DIMENSION);
    new_stack->allocated_amount = STACK_STARTING_DIMENSION;
    return new_stack;
}

//this function pushes a node to a stack of nodes
void stack_push(stack *p, command_node_tree *n)
{
    if(p->top + 1 <= p->allocated_amount - 1)
    {
        p->top = p->top + 1;
        p->vector[p->top] = n;
    }
    else
    {
        p->allocated_amount = p->allocated_amount + (p->allocated_amount / 2 + 1);
        p->vector = realloc(p->vector, sizeof(command_node_tree*) * p->allocated_amount);
        stack_push(p, n);
    }
}

//this function pops the top element of the stack p
command_node_tree * stack_pop(stack *p)
{
    if(p->top == -1)
        return NULL;
    command_node_tree *temp = p->vector[p->top];
    p->top = p->top - 1;
    return temp;
}

//this function returns the top element of the stack p
command_node_tree * stack_top(stack *p)
{
    if(p->top == -1)
        return NULL;
    command_node_tree *temp = p->vector[p->top];
    return temp;
}

//this function checks if the stack is empty
int stack_empty(stack *p)
{
    return p->top == -1;
}

//this function scans the tree and sets the text of the prints (previously stored in the print register),
// based on the state of the node referencing them
void execute(command_node_tree *root)
{
    //creating an empty stack, a new empty current state and a current node
    stack *p = stack_allocate();
    state *current_state = create_state();
    command_node_tree *current_node;

    current_node = root;
    execute_print(current_state, current_node->print_head);
    save_state(&current_node->current_state, current_state);
    stack_push(p, current_node);
    current_node = current_node->current_child->val;
    while(!stack_empty(p))
    {
        //current node does not have any children, or it has just one child who has yet to be visited
        if(current_node && (current_node->children == NULL || current_node->children->next == NULL))
        {
            modify_state(current_state, current_node->current_command);
            execute_print(current_state, current_node->print_head);
            current_node = (current_node->children) ? current_node->current_child->val : NULL;
        }
        //end of a branch, the next current node is set to the first intersection that still has an explorable path
        else if(current_node == NULL)
        {
            while(!stack_empty(p) && stack_top(p)->current_child->next == NULL)
            {
                stack_pop(p);
            }
            if(stack_empty(p))
                break;
            stack_top(p)->current_child = stack_top(p)->current_child->next;
            load_state(current_state, stack_top(p)->current_state);
            current_node = stack_top(p)->current_child->val;
        }
        //visiting a node that has at least a child to visit and has not been already visited
        else if(current_node && current_node->children->next)
        {
            modify_state(current_state, current_node->current_command);
            execute_print(current_state, current_node->print_head);
            save_state(&current_node->current_state, current_state);
            stack_push(p, current_node);
            current_node = current_node->current_child->val;
        }
    }
    free(p->vector);
    free(p);
}

// this function prints the final text, according to the print register, from last print to first
void complete_print(register_print *reg)
{
    if(!reg || !reg->last)
        return;
    print_node_list_dc *p = reg->last;
    while(p)
    {
        for(int i = 0; i < p->val->last_number - p->val->first_number + 1; i++)
        {
            if(p->val->text[i] == NULL)
                puts(".");
            else
                puts(p->val->text[i]);
        }
        p = p->prev;
    }

}



