//
// Created by Caterina Buccoliero on 18/08/2020.
//
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "definitions.h"

//this function allocates the space for a new state
state* create_state()
{
    state *new_state = malloc(sizeof(state));
    new_state->rows_amount = 0;
    new_state->allocated_amount = TEXT_STARTING_DIMENSION;
    new_state->text = malloc(sizeof(char*) * TEXT_STARTING_DIMENSION);
    return new_state;
}

//this function saves a state into a node
void save_state(state **destination, state *source)
{
    *destination = malloc(sizeof(state));
    if(source->rows_amount == 0)
        (*destination)->text = NULL;
    else
    {
        (*destination)->text = malloc(sizeof(char*) * source->rows_amount);
        memmove((*destination)->text, source->text, source->rows_amount * sizeof(char *));
    }
    (*destination)->rows_amount = source->rows_amount;
    (*destination)->allocated_amount = source->rows_amount;
}

//this function loads the source state to the destination state
void load_state(state *destination, state *source)
{
    if(destination->allocated_amount < source->rows_amount)
    {
        destination->text = realloc(destination->text, (source->rows_amount * 2) * sizeof(char *));
        destination->allocated_amount = source->rows_amount * 2;
    }
    if(source->rows_amount > 0)
        memmove(destination->text, source->text, source->rows_amount * sizeof(char*));
    destination->rows_amount = source->rows_amount;
}

//this method changes the text of the state according to the command c
void change_state(state *s, command *c)
{
    int text_end; //la prima riga presente ha indice 1
    int command_beginning, command_end;
    int intersection_beginning, intersection_end;
    int added_rows=0;

    text_end = s->rows_amount;
    command_beginning = c->first_number;
    command_end = c->last_number;

    if(text_end < command_beginning)
    {
        intersection_beginning = -1;
        intersection_end = -1;
    }
    else
    {
        intersection_beginning = command_beginning;
        intersection_end = (text_end < command_end) ? text_end : command_end;
    }
    //aggiornare le righe già esistenti
    for(int i=intersection_beginning, j=0; i <= intersection_end && i > 0; i++, j++)
    {
        s->text[i - 1] = c->text[j];
        added_rows++;
    }
    //controllo spazio disponibile
    int rows_to_add;
    rows_to_add = (intersection_end > 0) ? (command_end - command_beginning + 1) - (intersection_end - intersection_beginning + 1) :
                  (command_end - command_beginning + 1);
    if(s->rows_amount + rows_to_add > s->allocated_amount)
    {
        s->text = realloc(s->text, sizeof(char*) * (s->allocated_amount + rows_to_add) * 2);
        s->allocated_amount = (s->allocated_amount + rows_to_add) * 2;
    }
    for(int i= text_end + 1, j=0; j < rows_to_add; i++, j++)
        s->text[i - 1] = c->text[added_rows + j];
    s->rows_amount += rows_to_add;
}

//this method deletes a number of rows from the text of the state, according to the command c
void delete_state(state *s, command *c)
{
    int beginning_delete, end_delete;
    int beginning_text, end_text;
    int beginning_intersection, end_intersection;

    beginning_delete = c->first_number;
    end_delete = c->last_number;
    beginning_text = (s->rows_amount == 0) ? 0 : 1;
    end_text = s->rows_amount;

    if(beginning_delete > end_text || end_delete < beginning_text || beginning_text == 0)
        return;
    if(beginning_delete < beginning_text && end_delete <= end_text)
    {    //cancella da inizio testo a fine cancella
        beginning_intersection = beginning_text;
        end_intersection = end_delete;
    }
    else if(beginning_delete >= beginning_text && end_delete > end_text)
    {
        beginning_intersection = beginning_delete;
        end_intersection = end_text;
    }
    else if(beginning_delete >= beginning_text && end_delete <= end_text)
    {
        beginning_intersection = beginning_delete;
        end_intersection = end_delete;
    }
    else if(beginning_delete < beginning_text && end_delete > end_text)
    {
        beginning_intersection = beginning_text;
        end_intersection = end_text;
    }

    int rows_to_delete = end_intersection - beginning_intersection + 1;
    size_t to_move = s->rows_amount - (beginning_intersection - 1) - rows_to_delete;
    char **beginning_receiver, **beginning_source;
    beginning_receiver = &s->text[beginning_intersection - 1];
    beginning_source = &s->text[end_intersection];
    memmove(beginning_receiver, beginning_source, to_move * sizeof(char*));
    s->rows_amount = s->rows_amount - rows_to_delete;
}

// this function modifies the state by calling "change_state" or "delete_state", depending on the command c
void modify_state(state *s, command *c)
{
    if(c->type == 'c')
        change_state(s, c);
    else if(c->type == 'd')
        delete_state(s, c);
}

// this function prints a number of rows of the text of the state, according to the print stp
void print_state(state *s, print *stp)
{
    int beginning_print, end_print;
    int beginning_text, end_text;
    int beginning_intersection, end_intersection;

    beginning_print = stp->first_number;
    end_print = stp->last_number;
    beginning_text = (s->rows_amount == 0) ? 0 : 1;
    end_text = s->rows_amount;

    if(beginning_print > end_text || end_print < beginning_text || beginning_text == 0)
    {
        beginning_intersection = -1;
        end_intersection = -1;
    }
    else if(beginning_print < beginning_text && end_print <= end_text)
    {    //cancella da iniziotesto a fine cancella
        beginning_intersection = beginning_text;
        end_intersection = end_print;
    }
    else if(beginning_print >= beginning_text && end_print > end_text)
    {
        beginning_intersection = beginning_print;
        end_intersection = end_text;
    }
    else if(beginning_print >= beginning_text && end_print <= end_text)
    {
        beginning_intersection = beginning_print;
        end_intersection = end_print;
    }
    else if(beginning_print < beginning_text && end_print > end_text)
    {
        beginning_intersection = beginning_text;
        end_intersection = end_text;
    }

    stp->text = malloc(sizeof(char*) * (end_print - beginning_print + 1));
    if(beginning_intersection == -1)
    {
        for(int k = 0; k < end_print - beginning_print + 1; k++)
            stp->text[k] = NULL;
    }
    else
    {
        int j = 0;
        for (int i = stp->first_number; i < beginning_intersection; i++, j++)
            stp->text[j] = NULL;
        for (int i = beginning_intersection; i <= end_intersection; i++, j++)
            stp->text[j] = s->text[i - 1];
        for (int i = end_intersection + 1; i <= end_print; i++, j++)
            stp->text[j] = NULL;
    }

}

