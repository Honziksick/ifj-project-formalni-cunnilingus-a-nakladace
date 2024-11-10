/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           frame_stack.c                                             *
 * Autor:            David Krejčí <xkrejcd00>                                  *
 *                                                                             *
 * Datum:            30.10.2024                                                *
 * Poslední změna:   9.11.2024                                                 *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file frame_stack.c
 * @author David Krejčí <xkrejcd00>    
 *
 * @brief Implementační funkcí pro práci se zásobníkem rámců.
 * @details Tento implementační soubor obsahuje definice funkcí a datových struktur
 *          potřebných pro správu zásobníku rámců v překladači. Zásobník rámců
 *          je používán pro správu tabulek symbolů a jejich rámců během analýzy
 *          a generování kódu. Funkce v tomto souboru umožňují inicializaci,
 *          manipulaci a vyhledávání v zásobníku rámců.
 */

#include "frame_stack.h"
#include "symtable.h"
#include "dynamic_string.h"
#include "error.h"


/**
 * @brief Počáteční velikost pole rámců.
 */
#define FRAME_ARRAY_INIT_SIZE 10

/**
 * @brief Při rozšíření pole rámců se velikost pole násobí tímto faktorem.
 */
#define FRAME_ARRAY_EXPAND_FACTOR 2

// Globální zásobník rámců používaný v celém překladači
FrameStack stack = {NULL, NULL, 0};

// Globální pole všech vytvořených rámců
FrameArray frameArray = {0, NULL};


/**
 * @brief Inicializuje zásobník rámců.
 */
void frameStack_init(){
    // Alokujeme místo pro globální rámec
    FramePtr frame = malloc(sizeof(Frame));
    if(frame == NULL){
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme tabulku symbolů pro globální rámec
    frame->frame = symtable_init();
    if(frame->frame == NULL){
        free(frame);
        error_handle(ERROR_INTERNAL);
    }

    // Alokujeme pole rámců
    frameArray.array = malloc(FRAME_ARRAY_INIT_SIZE * sizeof(FramePtr));
    if(frameArray.array == NULL){
        free(frame->frame);
        free(frame);
        error_handle(ERROR_INTERNAL);
    }

    // Nastavíme vrchol a spodek zásobníku na globální rámec
    stack.bottom = frame;
    stack.top = frame;

    // Inicializujeme globální rámec
    frame->frameID = 0;
    frame->next = NULL;
    frame->searchStop = true;

    // Inicializujeme pole rámců
    frameArray.allocated = FRAME_ARRAY_INIT_SIZE;
    frameArray.array[frame->frameID] = frame;

}

/**
 * @brief Vloží nový rámec na zásobník.
 */
void frameStack_push(bool isFunction){
    // Pokud není zásobník inicializován, skončíme s chybou
    if(stack.top == NULL){
        error_handle(ERROR_INTERNAL);
    }

    // Alokujeme místo pro rámec
    FramePtr frame = malloc(sizeof(Frame));
    if(frame == NULL){
        error_handle(ERROR_INTERNAL);
    }

    // Vytvoříme tabulku symbolů rámce
    frame->frame = symtable_init();
    if(frame->frame == NULL){
        free(frame);
        error_handle(ERROR_INTERNAL);
    }

    // Aktualizujeme nejvyšší ID
    stack.currentID++;
    
    // Nastavíme hodnoty rámce
    frame->frameID = stack.currentID;
    frame->next = stack.top;
    frame->searchStop = isFunction;

    // Nastavíme vrchol zásobníku
    stack.top = frame;
    
    // Pokud došla kapacita pole rámců, tak jej rozšíříme
    if(stack.currentID +1 > frameArray.allocated){
        // Vytvoříme nové pole s dvojnásobnou kapacitou
        FramePtr *new_array = malloc(frameArray.allocated * FRAME_ARRAY_EXPAND_FACTOR * sizeof(FramePtr));
        if(new_array == NULL){
            free(frame->frame);
            free(frame);
            error_handle(ERROR_INTERNAL);
        }

        // Překopírujeme položky
        memcpy(new_array, frameArray.array, frameArray.allocated * sizeof(FramePtr));
        // Uvolníme staré pole
        free(frameArray.array);
        // Nastavíme novou velikost a ukazatel na pole
        frameArray.allocated *= FRAME_ARRAY_EXPAND_FACTOR;
        frameArray.array = new_array;
    }

    // Přidáme rámec do globálního pole
    frameArray.array[frame->frameID] = frame;

}

/**
 * @brief Odstraní vrchní rámec ze zásobníku.
 */
frame_stack_result frameStack_pop(){
    if(stack.top == NULL){
        return FRAME_STACK_NOT_INITIALIZED;
    }
    if(stack.top->next == NULL){
        return FRAME_STACK_POP_GLOBAL;
    }
    stack.top = stack.top->next;
    return FRAME_STACK_SUCCESS;
}

/**
 * @brief Vyhledá položku v zásobníku rámců podle klíče.
 */
frame_stack_result frameStack_findItem(DString *key, SymtableItem **out_item){
    // Pokud je klíč NULL, vrátíme chybu
    if(key == NULL){
        return FRAME_STACK_KEY_NULL;
    }

    // Pokud není zásobník inicializován, vrátíme chybu
    if(stack.top == NULL){
        return FRAME_STACK_NOT_INITIALIZED;
    }

    // Nastavíme začátek prohledávání na vrchol zásobníku
    FramePtr frame = stack.top;
    // Cyklus prohledávání rámců
    while(true){
        // Prohledáme rámec
        symtable_result result = symtable_findItem(frame->frame, key, out_item);
        if(result == SYMTABLE_SUCCESS){
            // Pokud byla položka nalezena, vrátíme úspěch
            return FRAME_STACK_SUCCESS;
        }else{
            // Pokud položka nebyla nalezena a vynořili bychom se z funkce,
            // položka neexistuje
            if(frame->searchStop){
                // Pokud jsme na globálním rámci, položka neexistuje
                if(frame->frameID == 0){
                    return FRAME_STACK_ITEM_DOESNT_EXIST;
                }
                break;
            }
            // Jinak pokračujeme v prohledávání dalšího rámce
            frame = frame->next;
        }
    }
    // Projdeme ještě globální rámec
    symtable_result result = symtable_findItem(frameArray.array[0]->frame, key, out_item);
    if(result == SYMTABLE_SUCCESS){
        return FRAME_STACK_SUCCESS;
    }else{
        return FRAME_STACK_ITEM_DOESNT_EXIST;
    }

}



/**
 * @brief Přidá novou položku do vrchního rámce zásobníku.
 */
frame_stack_result frameStack_addItem(DString *key, SymtableItem **out_item){
    // Voláme findItem, abychom zjistili, zda položka již existuje
    // Zároveň získáme ukazatel na položku, pokud existuje
    // Funkce se zároveň postará o chybové stavy
    frame_stack_result result = frameStack_findItem(key, out_item);
    // Pokud položka již existuje, vracíme tuto informaci
    if(result == FRAME_STACK_SUCCESS){
        return FRAME_STACK_ITEM_ALREADY_EXISTS;
    }
    // Pokud položka neexistuje, přidáme ji
    if(result == FRAME_STACK_ITEM_DOESNT_EXIST){
        symtable_result sym_result = symtable_addItem(stack.top->frame, key, out_item);
        // Pokud se položka podařilo přidat, vrátíme úspěch
        if(sym_result == SYMTABLE_SUCCESS){
            return FRAME_STACK_SUCCESS;
        }else{
            // Pokud selhalo přidání položky, vrátíme chybu
            return FRAME_STACK_ALLOCATION_FAIL;
        }
    }
    // Pokud došlo k chybě při hledání, vrátíme chybu
    return result;
}

void frameStack_destroyAll(){
    // Pokud není zásobník inicializován, vrátíme se
    if(stack.top == NULL){
        return;
    }
    // Uvolníme všechny rámce
    for(size_t i = 0; i <= stack.currentID; i++){
        symtable_destroyTable(frameArray.array[i]->frame);
        free(frameArray.array[i]);
    }
    free(frameArray.array);

    // Nastavíme zásobník na původní stav
    stack.bottom = NULL;
    stack.top = NULL;
    stack.currentID = 0;

    // Nastavíme pole rámců na původní stav
    frameArray.allocated = 0;
    frameArray.array = NULL;
}


/*** Konec souboru frame_stack.h ***/
