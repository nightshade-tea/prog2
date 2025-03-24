#include <stdlib.h>
#include <stdio.h>

#define MACHINE_DEFAULT_PROBABILITY 5

/* Structs */

typedef struct teddy_machine {
    unsigned int id;
    unsigned int probability;

    struct teddy_machine *next;
    struct teddy_machine *previous;
} teddy_machine;

/* List functions */

teddy_machine* create_machine(unsigned int id, teddy_machine* previous, teddy_machine* next)
{
    teddy_machine *mach;

    if ( !(mach = malloc(sizeof(teddy_machine))) )
        return NULL;

    mach->id = id;
    mach->probability = MACHINE_DEFAULT_PROBABILITY;
    mach->next = next;
    mach->previous = previous;

    return mach;
}

void destroy_machine(teddy_machine** pmach)
{
    teddy_machine *mach_next, *mach_previous;

    if ( !pmach || !(*pmach) )
        return;

    mach_next = (*pmach)->next;
    mach_previous = (*pmach)->previous;

    if (mach_next)
        mach_next->previous = mach_previous;

    if (mach_previous)
        mach_previous->next = mach_next;

    free(*pmach);
    *pmach = NULL;

    return;
}

void destroy_list(teddy_machine *list)
{
    teddy_machine *current, *next;

    if (!list)
        return;

    current = list;
    next = list->next;

    while (current != next) {
        destroy_machine(&current);
        current = next;

        if (next)
            next = next->next;
    }

    destroy_machine(&current);

    return;
}

teddy_machine* create_list(unsigned int machines)
{
    teddy_machine *list, *mach;

    if (machines < 1)
        return NULL;

    if ( !(list = create_machine(1, NULL, NULL)) )
        return NULL;

    list->previous = list->next = list;

    for (int id = 2; id <= machines; id++) {
        if ( !(mach = create_machine(id, list->previous, list)) ) {
            destroy_list(list);
            return NULL;
        }

        list->previous = mach->previous->next = mach;
    }

    return list;
}

teddy_machine* select_machine(teddy_machine *list, unsigned int place)
{
    int meio;
    teddy_machine *selected;

    if (!list)
        return NULL;

    meio = 0;

    if (list->previous)
        meio = (list->previous->id) / 2;

    selected = list;

    // escolha o caminho mais curto
    if (place < meio) {

        while (selected->id != place) {

            selected = selected->next;

            if (!selected)
                return NULL;
        }

    } else {

        while (selected->id != place) {

            selected = selected->previous;

            if (!selected)
                return NULL;
        }
    }

    return selected;
}

// remover sem destruir?
teddy_machine* remove_machine(teddy_machine *list, teddy_machine *remove)
{
    destroy_machine(&remove);
    return NULL;
}

/* Randomization functions */

unsigned int get_place(unsigned int machines){
    return rand() % machines;
}

unsigned int get_attempt(){
    return rand() % 100 + 1;
}

/* Printing functions */

void print_attempt(teddy_machine *machine, unsigned int attempt){

    if (attempt <= machine->probability) printf("O URSO DA MAQUINA %u [FOI] OBTIDO!\n", machine->id);
    else printf("O URSO DA MAQUINA %u [NAO FOI] OBTIDO!\n", machine->id);
}

void print_available_machines(teddy_machine *list){

    if (!list) printf("NAO HA MAQUINAS DISPONIVEIS!\n");
    else{
        teddy_machine *i = list;
        do {
            printf("PROBABILIDADE DA MAQUINA %u: %u\n", i->id, i->probability);
            i = (teddy_machine*) i->next;
        } while((i) && (i != list));
    }

}

/* Main function */

int main(int argc, char *argv[]){
    unsigned int machines, rounds, seed;
    int rv = 0;

    if (argc != 4) rv = -1;
    else {
        machines = atoi(argv[1]);
        if (!machines) rv = -2;
        else {
            rounds = atoi(argv[2]);
            if (!rounds) rv = -3;
            else {
                seed = atoi(argv[3]);
                if (!seed) rv = -4;
            }
        }
    }

    if (rv){
        printf("USO: main [NR. DE MAQUINAS] [NR. DE RODADAS] [SEMENTE DE RAND.]\n");
        return rv;
    }

    teddy_machine *list = create_list(machines);
    srand(seed);

    unsigned int machine_place, machine_attempt;
    for (unsigned int r = 0; r < rounds; r++){
        printf("\n============================ ROUND %u ============================\n", r+1);
        machine_place = get_place(machines); /* Define a localização da máquina da rodada, não considera máquinas sem urso */
        machine_attempt = get_attempt(); /* Define a tentativa da rodada; se for menor ou igual à probabilidade da máquina selecionada, o urso foi pego */

        /* INSTRUÇÃO: Implemente a lógica do seu programa aqui */
        /* INSTRUÇÃO: Utilize a função "print_attempt" antes do "print_available_machines"! */

        print_available_machines(list);
        printf("==================================================================\n");
    }

    return 0;
}
