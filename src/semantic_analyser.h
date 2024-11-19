/*******************************************************************************
 *                                                                             *
 * Název projektu:   Implementace překladače imperativního jazyka IFJ24        *
 *                                                                             *
 * Soubor:           semantic_analyser.h                                       *
 * Autor:            Krejčí David   <xkrejcd00>                                *
 *                                                                             *
 * Datum:            11.11.2024                                                *
 * Poslední změna:   19.11.2024                                                *
 *                                                                             *
 * Tým:      Tým xkalinj00                                                     *
 * Členové:  Farkašovský Lukáš    <xfarkal00>                                  *
 *           Hýža Pavel           <xhyzapa00>                                  *
 *           Kalina Jan           <xkalinj00>                                  *
 *           Krejčí David         <xkrejcd00>                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file semantic_analyser.h
 * @author Krejčí David \<xkrejcd00>
 *
 * @brief Hlavičkový soubor pro sémantický analyzátor.
 * @details Obsahuje definice funkcí pro sémantickou analýzu
 */

#ifndef SEMANTIC_ANALYSER_H_
/** @cond  */
#define SEMANTIC_ANALYSER_H_
/** @endcond  */

#include <stdlib.h>
#include <stdbool.h>
#include "ast_nodes.h"
#include "ast_interface.h"
#include "frame_stack.h"
#include "symtable.h"
#include "error.h"


typedef enum {
    SEM_DATA_UNKNOWN = 0,
    SEM_DATA_INT = 1,
    SEM_DATA_INT_OR_NULL = 2,
    SEM_DATA_FLOAT = 3,
    SEM_DATA_FLOAT_OR_NULL = 4,
    SEM_DATA_RAW_STRING = 5,
    SEM_DATA_STRING = 6,
    SEM_DATA_STRING_OR_NULL = 7,
    SEM_DATA_VOID = 8,
    SEM_DATA_BOOL = 9,
    SEM_DATA_NULL = 10
} Semantic_Data;


/**
 * @brief Provede sémantickou analýzu celého programu.
 *
 * @details Volá funkce pro dílčí sémantické analýzy programu:
 *          - semantic_checkProgramStructure()
 *          - semantic_checkFunctionDefinitions()
 *          - semantic_checkVariables()
 */
void semantic_analyseProgram();


/**
 * @brief Provede sémantickou analýzu základní struktury programu.
 *
 * @details Ověří, že existuje funkce main s návratovým typem void a bez parametrů
 *          Dále ověří sprvánost literálů v prologu programu
 *
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseProgramStructure();


/**
 * @brief Provede sémantickou analýzu definic funkcí
 *
 * @details Projde strom a pro každou definici funkce zavolá sémantickou sondu
 *          ()
 *
 * @return '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseFunctionDefinitions();


/**
 * @brief Provede sémantickou analýzu všech proměnných v programu
 *
 * @details Projde všechny rámce v globálním poli a zkontroluje,
 *          že každá proměnná je využita,
 *          že každá nekonstantní proměnná je změněna,
 *          že žádná konstantní proměnná není změněna
 *
 * @return '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseVariables();


/**
 * @brief Provede sémantickou analýzu bloku funkce
 *
 * @details Prochází blok příkaz po příkazu a podle typu volá další
 *          pomocné funkce podle typu příkazu. Kontroluje, zda funkce
 *          vrací hodnotu na všech cestách, pokud není void.
 * 
 * @param [in] node Ukazatel na uzel funkce
 *
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 *
 */
ErrorType semantic_probeFunction(AST_FunDefNode *node);

/**
 * @brief Provede sémantickou analýzu bloku příkazů
 * 
 * @details Prochází blok příkaz po příkazu a podle typu volá další
 *          pomocné funkce podle typu příkazu. Je volána pro bloky funkcí,
 *          cyklů a podmínek. 
 * 
 * @param [in] fun_return Návratový typ funkce, ve které se blok nachází
 * @param [in] statement Ukazatel na uzel bloku (první příkaz)
 * @param [out] returned Ukazatel na proměnnou, do které se uloží, zda byla
 *                      nalezena návratová hodnota
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_probeBlock(Semantic_Data fun_return,
                              AST_StatementNode *statement, bool* returned);


/**
 * @brief Provede sémantickou analýzu definice proměnné
 * 
 * @details Vyhodnotí typ a případně hodnotu výrazu pravé strany a porovná jej
 *          s typem vlevo nebo jej odvodí z výrazu vpravo. 
 * 
 * @param [in] statement Ukazatel na uzel definice proměnné
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 * 
 */
ErrorType semantic_analyseVarDef(AST_StatementNode *statement);

/**
 * @brief Provede sémantickou analýzu výrazu
 * 
 * @details Vyhodnotí typ a případně hodnotu výrazu. Pro literály a proměnné
 *          nastaví typ a hodnotu. Pro binární operátory a volání funkcí volá
 *          další funkce (semantic_analyseFunCall, semantic_analyseBinOp).
 * 
 * @param [in] expr_node Ukazatel na uzel výrazu
 * @param [out] type Ukazatel na proměnnou, do které se uloží typ výrazu
 * @param [out] value Ukazatel na proměnnou, do které se uloží hodnota výrazu
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseExpr(AST_ExprNode *expr_node, Semantic_Data *type, void **value);

/**
 * @brief Provede sémantickou analýzu binárního operátoru
 * 
 * @details Vyhodnotí typ a případně hodnotu výrazu. Provede sémantickou analýzu
 *          levého a pravého operandu a porovná je podle typu operátoru.
 *          Pro operátory s přiřazením zkontroluje, zda je vlevo proměnná.
 *          Jinak kontroluje kompatibilitu typů operandů a případně provádí konverze.
 * 
 * @param [in] node Ukazatel na uzel binárního operátoru
 * @param [out] type Ukazatel na proměnnou, do které se uloží typ výrazu
 * @param [out] value Ukazatel na proměnnou, do které se uloží hodnota výrazu
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseBinOp(AST_ExprNode *node, Semantic_Data *type, void** value);

/**
 * @brief Provede sémantickou analýzu volání funkce
 * 
 * @details Kontroluje typ a počet parametrů funkce. Pro vestavěné funkce
 *          ifj.write a ifj.string volá konktrétní funkce pro kontrolu,
 *          protože argumenty mohou mít více typů.
 * 
 * @param [in] fun_node Ukazatel na uzel volání funkce
 * @param [out] return_type Ukazatel na proměnnou, do které se uloží návratový typ
 * 
 */
ErrorType semantic_analyseFunCall(AST_FunCallNode *fun_node, Semantic_Data *return_type);

/**
 * @brief Provede sémantickou analýzu podmíněného příkazu if
 * 
 * @details Provede sémantickou analýzu podmínky, která má buď pravdivostní typ
 *          nebo typ s null. Provede sémantickou analýzu bloků then a else.
 * 
 * @param [in] fun_return Návratový typ funkce, ve které se blok nachází
 * @param [in] if_node Ukazatel na uzel podmíněného příkazu if
 * @param [out] returned Ukazatel na proměnnou, do které se uloží, zda byla
 *                     v then i else nalezena návratová hodnota
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseIf(Semantic_Data fun_return,       \
                             AST_IfNode *if_node, bool *returned);

/**
 * @brief Provede sémantickou analýzu cyklu while
 * 
 * @details Provede sémantickou analýzu podmínky, která musí mít pravdivostní typ.
 *          Provede sémantickou analýzu těla cyklu.
 * 
 * @param [in] fun_return Návratový typ funkce, ve které se blok nachází
 * @param [in] while_node Ukazatel na uzel cyklu while
 * @param [out] returned Ukazatel na proměnnou, do které se uloží, zda byla
 *                    v těle cyklu nalezena návratová hodnota
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseWhile(Semantic_Data fun_return,       \
                                AST_WhileNode *while_node, bool *returned);

/**
 * @brief Provede sémantickou analýzu návratového příkazu return
 * 
 * @details Provede sémantickou analýzu výrazu, který musí být kompatibilní
 *         s návratovým typem funkce.
 * 
 * @param [in] fun_return Návratový typ funkce
 * @param [in] node Ukazatel na uzel návratového příkazu return
 * 
 * @return  '0', pokud nebyla nalezena sémantická chyba, jinak kód chyby
 */
ErrorType semantic_analyseReturn(Semantic_Data fun_return, AST_ExprNode *node);

/**
 * @brief Vyhodnotí, zda lze provést přiřazení z typu from do typu to
 * 
 * @details Nepočítá s případnými konverzemi, to znamená, že vrací 0 pouze
 *          pokud jsou typy kompatibilní bez jakékoli konverze.
 * 
 * @param [in] to Typ, do kterého se má přiřadit
 * @param [in] from Typ, ze kterého se má přiřadit
 * 
 * @return  '0', pokud je přiřazení možné, jinak kód chyby
 */
ErrorType semantic_compatibleAssign(Semantic_Data to, Semantic_Data from);

/**
 * @brief Vyhodnotí, zda lze provést relační operaci mezi typy
 * 
 * @details Typy musí být číselné bez NULL. Počítá s možností konverze,
 *          to znamená, že vrací 0 pokud typy nejsou rovnou kompatibilní,
 *          ale je šance, že se dají převést.
 * 
 * @param [in] type_1 Typ 1
 * @param [in] type_2 Typ 2
 * 
 * @return  '0', pokud je operace možná, jinak kód chyby
 */
ErrorType semantic_compatibleRelation(Semantic_Data type_1, Semantic_Data type_2);

/**
 * @brief Vyhodnotí, zda lze provést rovnostní operaci mezi typy
 * 
 * @details Typy musí být číselné, číselné s NULL nebo NULL.
 *          Počítá s možností konverze, to znamená, že vrací 0 pokud typy
 *          nejsou rovnou kompatibilní, ale je šance, že se dají převést.
 * 
 * @param [in] type_1 Typ 1
 * @param [in] type_2 Typ 2
 * 
 * @return  '0', pokud je operace možná, jinak kód chyby
 */
ErrorType semantic_compatibleEqual(Semantic_Data type_1, Semantic_Data type_2);

/**
 * @brief Vyhodnotí, zda lze provést aritmetickou operaci mezi typy
 * 
 * @details Typy musí být číselné. Počítá s možností konverze, to znamená,
 *          že typy mohou být různé, ale musí být oba číselné.
 * 
 * @param [in] type_1 Typ 1
 * @param [in] type_2 Typ 2
 * 
 * @return  '0', pokud je operace možná, jinak kód chyby
 */
ErrorType semantic_compatibleArithmetic(Semantic_Data type_1, Semantic_Data type_2);

/**
 * @brief Převede hodnotu z typu float na typ int
 * 
 * @details Pokusí se převést hodnotu z typu float na int.
 *          Zkontroluje, že float nemá desetinnou část.
 *          Předpokládá typ výrazu AST_VarNode s literálem nebo proměnnou.
 *          Uvolní předchozí hodnotu a nastaví novou.
 *
 * @param [in] node Ukazatel na uzel s výrazem
 * 
 * @return  '0', pokud byla konverze úspěšná, jinak kód chyby
 */
ErrorType semantic_toInt(AST_ExprNode *node);

/**
 * @brief Převede hodnotu z typu int na typ float
 * 
 * @details Pokusí se převést hodnotu z typu int na float.
 *          Předpokládá typ výrazu AST_VarNode s literálem nebo proměnnou.
 *          Uvolní předchozí hodnotu a nastaví novou.
 * 
 * @param [in] node Ukazatel na uzel s výrazem
 * 
 * @return  '0', pokud byla konverze úspěšná, jinak kód chyby
 */
ErrorType semantic_toFloat(AST_ExprNode *node);

/**
 * @brief Převede hodnotu enum AST_DataType na typ Semantic_Data
 */
Semantic_Data semantic_ASTToSemType(AST_DataType type);

/**
 * @brief Převede hodnotu enum AST_LiteralType na typ Semantic_Data
 */
Semantic_Data semantic_literalToSemType(AST_LiteralType type);

/**
 * @brief Převede hodnotu enum symtable_symbolState na typ Semantic_Data
 */
Semantic_Data semantic_stateToSemType(symtable_symbolState state);

/**
 * @brief Převede hodnotu enum symtable_functionReturnType na typ Semantic_Data
 */
Semantic_Data semantic_returnToSemType(symtable_functionReturnType type);

/**
 * @brief Převede hodnotu typu Semantic_Data na enum symtable_symbolState
 */
symtable_symbolState semantic_semTypeToState(Semantic_Data type);

/**
 * @brief Převede hodnotu typu Semantic_Data na enum AST_LiteralType
 */
AST_LiteralType semantic_semToLiteral(Semantic_Data type);

/**
 * @brief Kontroluje počet parametrů volání funkce ifj.write
 */
ErrorType semantic_checkIFJWrite(AST_FunCallNode *fun_node);

/**
 * @brief Kontroluje typ a počet parametrů volání funkce ifj.string
 */
ErrorType semantic_checkIFJString(AST_FunCallNode *fun_node);

#endif // SEMANTIC_ANALYSER_H_

/*** Konec souboru semantic_analyser.h ***/
