//
// Created by Gilles Sérasset on 09/10/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tokens.h"
#include "lookahead_lexer.h"
#include "uthash.h"

#include "assignations.h"

/*********** Managing Symbol Table with UTHash Hash table ***************/

struct Symbol {
    int value;
    char var[100];     /* we'll use this field as the key */
    UT_hash_handle hh; /* makes this structure hashable */
};

struct Symbol *symbols = NULL;

void set_value(char var[], int val) {
    struct Symbol* symbol;
    HASH_FIND_STR( symbols, var, symbol );
    if (symbol == NULL) {
        // the symbol does not exist yet
        symbol = calloc(1, sizeof(struct Symbol));
        strcpy(symbol->var, var);
        HASH_ADD_STR( symbols, var, symbol );
    }
    symbol->value = val;
}

int get_value(char var[]) {
    struct Symbol* symbol;
    HASH_FIND_STR( symbols, var, symbol );
    if (symbol == NULL) {
        return 0;
    } else {
        return symbol->value;
    }
}

void print_symbols() {
    struct Symbol *s;

    for(s=symbols; s != NULL; s=s->hh.next) {
        fprintf(stdout, "%s = %d\n", s->var, s->value);
    }
}

/************** Grammaire *****************/

void error(char *m){
    fprintf(stderr,"Erreur : %s\n", m);
}

void parseS(){
    if(currentToken()!=NULL && (currentToken()->type==PLUS || currentToken()->type==MINUS 
    || currentToken()->type==OPAR || currentToken()->type==VAR || currentToken()->type==INT)){
        parseA();
        consume(SEMICOLON);
        parseS();
    }
    else if(currentToken()==NULL){
        return;
    }
    else{
        error("Erreur en partion de S"); 
    }
}
void parseA(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || currentToken()->type==MINUS 
        || currentToken()->type==OPAR  || currentToken()->type==INT){
            parseE();
        }
        else if(currentToken()->type==VAR || lookup(2)->type==ASSIGN){
            consume(VAR);
            consume(ASSIGN);
            parseA();
            return;
        }
        else{
            parseE();
        }
    }
    else{
        error("Erreur en partion de S"); 
    }
}
void parseE(){
    if(currentToken()!=NULL && (currentToken()->type==PLUS || currentToken()->type==MINUS 
    || currentToken()->type==OPAR || currentToken()->type==VAR || currentToken()->type==INT)){
        parseT();
        parseEp();
    }
    else{
        error("Erreur en partion de E"); 
    }
}
void parseEp(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS){
            consume(PLUS);
        }
        else if(currentToken()->type==MINUS){
            consume(MINUS);
        }
        else if(currentToken()->type==FPAR || currentToken()->type==SEMICOLON){
            return ;
        }
        parseT();
        parseEp();
    }
    else{
        error("Erreur en partion de E' "); 
    }
}
void parseT(){
    if(currentToken()!=NULL && (currentToken()->type==PLUS || 
    currentToken()->type==MINUS || currentToken()->type==OPAR || 
    currentToken()->type==VAR ||currentToken()->type==INT)){
        parseP();
        parseTp();
    }
    else{
        error("Erreur en partion de T "); 
    }
}
void parseTp(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==FPAR || 
        currentToken()->type==SEMICOLON){
            return;
        }
        else if(currentToken()->type==MULT){
            consume(MULT);
        }
        else if(currentToken()->type==DIV){
            consume(DIV);
        }
        parseP();
        parseTp();
    }
    else{
        error("Erreur en partion de T' "); 
    }
}
void parseP(){
    if(currentToken()!=NULL && (currentToken()->type==PLUS || 
    currentToken()->type==MINUS || currentToken()->type==OPAR || 
    currentToken()->type==VAR ||currentToken()->type==INT)){
        parseU();
        parsePp();
    }
    else{
        error("Erreur en partion de P "); 
    }
}
void parsePp(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==FPAR || 
        currentToken()->type==SEMICOLON || currentToken()->type==MULT
        || currentToken()->type==DIV){
            return;
        }
        else if(currentToken()->type==EXPON){
            consume(EXPON);
        }
        parseU();
        parsePp();
    }
    else{
        error("Erreur en partion de P' "); 
    }
}
void parseU(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS){
            consume(PLUS);
        }
        else if(currentToken()->type==MINUS){
            consume(MINUS);
        }
        else if(currentToken()->type==OPAR || currentToken()->type==VAR
        || currentToken()->type==INT){
            parseF();
            return;
        }
        parseU();
    }
    else{
        error("Erreur en partion de U "); 
    }
}
void parseF(){
    if(currentToken()!=NULL){
        if(currentToken()->type==OPAR){
            consume(OPAR);
            parseA();
            consume(FPAR);
            return;
        }
        else if(currentToken()->type==VAR){
            consume(VAR);
        }
        else if(currentToken()->type==INT){
            consume(INT);
        }
    }
    else{
        error("Erreur en partion de F "); 
    }
}
/********* PROGRAMME PRINCIPAL **************/
int main(int argc, char *argv[]) {
    // fprintf(stderr, "Lexing from %s.\n", argv[1]);
    initLexer(argv[1]);
    // APPEL A LA SOURCE DE LA GRAMMAIRE

    fprintf(stderr, "Starting parse...\n");
    parseS();

    // ON VERIFIE QUE LA GRAMMAIRE A BIEN TERMINE SON TRAVAIL A LA FIN DU MOT A ANALYSER
    if (currentToken() != NULL) {
        fprintf(stderr, "Unexpected input after assignation.\n");
        fprintf(stdout, "!!! ERREUR !!!\n");
        return 1;
    }
    // ET ON AFFICHE LA TABLE DES SYMBOLES
    print_symbols();
    return 0;
}

