//
// Created by Caterina Buccoliero on 18/08/2020.
//

#include "reading.h"
#include "definitions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//this function reads the input and creates a command according to the input
command read_command ()
{
    command current_command;
    char buffer[1025];
    size_t row_length;

    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    row_length = strlen(buffer);
    current_command.type = buffer[row_length - 1];
    if (current_command.type == 'c')
    {
        char *comma = strchr(buffer, ',');
        current_command.first_number = atoi(buffer);
        current_command.last_number = atoi(comma + 1);
        int rows_amount = current_command.last_number - current_command.first_number + 1;
        current_command.text = malloc(sizeof(char*) * rows_amount);
        for (int i=0; i < rows_amount; i++)
        {
            fgets(buffer, 1024, stdin);
            buffer[strcspn(buffer, "\r\n")] = '\0';
            row_length = strlen(buffer);
            current_command.text [i] = malloc(sizeof(char) * (row_length + 1));
            strcpy(current_command.text[i], buffer);
        }
        fgets(buffer, 1024, stdin);

    }
    else if (current_command.type == 'd' || current_command.type == 'p')
    {
        char *comma = strchr(buffer, ',');
        current_command.first_number = atoi(buffer);
        current_command.last_number = atoi(comma + 1);
        current_command.text = NULL;
    }
    else if (current_command.type == 'u' || current_command.type == 'r')
    {
        current_command.first_number = atoi(buffer);
        current_command.last_number = -1;
        current_command.text = NULL;
    }
    else if (current_command.type == 'q')
    {
        current_command.first_number = -1;
        current_command.last_number = -1;
        current_command.text = NULL;
    }
    return current_command;

}

//this function reads the whole input and creates the tree accordingly
void complete_reading (command_node_tree * root, register_print *reg)
{
    command_node_tree *current_node, *new_node;
    command last_read_command, *new_command;

    last_read_command = read_command();
    current_node = root;

    while (last_read_command.type != 'q')
    {
        if(last_read_command.type == 'c' || last_read_command.type == 'd')
        {
            new_command = malloc(sizeof(command));
            *new_command = last_read_command;
            new_node = command_node_create(new_command);
            command_node_tree_add(current_node, new_node);
            current_node = new_node;
        }
        else if(last_read_command.type == 'p')
        {
            print *current_print = malloc(sizeof(print));
            current_print->first_number = last_read_command.first_number;
            current_print->last_number = last_read_command.last_number;
            current_print->text = NULL;
            register_add(reg, current_print);
            add_print(current_node, current_print);
        }
        else if(last_read_command.type == 'u')
        {
            current_node = command_node_scan(current_node, -last_read_command.first_number);
        }
        else if(last_read_command.type == 'r')
        {
            current_node = command_node_scan(current_node, last_read_command.first_number);
        }
        last_read_command = read_command();
    }
}
