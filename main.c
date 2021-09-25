#include <stdio.h>
#include "definitions.h"
#include "reading.h"


int main() {
    //freopen("Input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);

    command_node_tree *root = command_node_create(NULL);
    register_print *r = register_create();
    complete_reading(root, r);
    execute(root);
    complete_print(r);

    /*comando letto = leggi_comando();
    stato * s = stato_crea();
    while(letto.tipo!='q')
    {
        comando *c1 = malloc(sizeof(comando));
        *c1 = letto;
        stato_modifica(s, c1);
        letto = leggi_comando();
    }
    */




    return 0;
}
