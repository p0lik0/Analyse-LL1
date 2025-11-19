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
    fprintf(stderr,"!!! ERREUR !!!\n");
    //fprintf(stderr,"%s\n", m);
    exit(1);
}

void parseS(){
    if(currentToken()!=NULL && (currentToken()->type==PLUS || currentToken()->type==MINUS 
    || currentToken()->type==OPAR || currentToken()->type==VAR || currentToken()->type==INT)){
        parseA();
        if (currentToken() == NULL || currentToken()->type != SEMICOLON)
            error("Token inattendu pour S");
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
int parseA(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || currentToken()->type==MINUS 
        || currentToken()->type==OPAR  || currentToken()->type==INT){
            return parseE();
        }
        else if(currentToken()->type==VAR && lookup(2)->type==ASSIGN){
            char *t = strdup(currentToken()->text);
            consume(VAR);
            consume(ASSIGN);
            int v = parseA();
            set_value(t,v);
            free(t);
            return v;
        }
        else if(currentToken()->type==VAR){
            return parseE();
        }
        else {
            error("Token inattendu pour A");
        }
    }
    else{
        error("Erreur en partion de A"); 
    }

}
int parseE(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || currentToken()->type==MINUS 
        || currentToken()->type==OPAR || currentToken()->type==VAR || currentToken()->type==INT){
            int v = parseT();
            return parseEp(v);
        }
        else {
            error("Token inattendu pour E");
        }
    }
    else{
        error("Erreur en partion de E"); 
    }
}
int parseEp(int res){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS){
            consume(PLUS);
            res += parseT();
            return parseEp(res);
        }
        else if(currentToken()->type==MINUS){
            consume(MINUS);
            res -= parseT();
            return parseEp(res);
        }
        else if(currentToken()->type==FPAR || currentToken()->type==SEMICOLON){
            return res;
        }
        else {
            error("Token inattendu pour E'");
        }
    }
    else{
        error("Erreur en partion de E' (token NULL)"); 
    }
}
int parseT(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==OPAR || 
        currentToken()->type==VAR ||currentToken()->type==INT){
            int v = parseP();
            return parseTp(v);
        }
        else {
            error("Token inattendu pour T");
        }
    }
    else{
            error("Erreur en partion de T "); 
    }
}
int parseTp(int res){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==FPAR || 
        currentToken()->type==SEMICOLON){
            return res;
        }
        else if(currentToken()->type==MULT){
            consume(MULT);
            res *= parseP();
            return parseTp(res);
        }
        else if(currentToken()->type==DIV){
            consume(DIV);
            int d = parseP();
            if (d == 0) error("Division par zero");        
            res /= d;
            return parseTp(res);
        }
        else {
            error("Token inattendu pour T'");
        }
    }
    else{
        error("Erreur en partion de T' "); 
    }
}
int parseP(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==OPAR || 
        currentToken()->type==VAR ||currentToken()->type==INT){
            int v = parseU();
            return parsePp(v);
        }
        else {
            error("Token inattendu pour P");
        }
    }
    else{
        error("Erreur en partion de P "); 
    }
}
int parsePp(int res){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS || 
        currentToken()->type==MINUS || currentToken()->type==FPAR || 
        currentToken()->type==SEMICOLON || currentToken()->type==MULT
        || currentToken()->type==DIV){
            return res;
        }
        else if(currentToken()->type==EXPON){
            consume(EXPON);
            int u = parseU();
            int r = res;
            if(u>0){
                for(int i=1; i<u; i++){
                    res *= r;
                }
            }
            else res=0;
            return parsePp(res);
        }
        else {
            error("Token inattendu pour P'");
        }
    }
    else{
        error("Erreur en partion de P' "); 
    }
}
int parseU(){
    if(currentToken()!=NULL){
        if(currentToken()->type==PLUS){
            consume(PLUS);
            return parseU();
        }
        else if(currentToken()->type==MINUS){
            consume(MINUS);
            return -parseU();
        }
        else if(currentToken()->type==OPAR || currentToken()->type==VAR
        || currentToken()->type==INT){
            return parseF();
        }
        else {
            error("Token inattendu pour U");
        }
    }
    else{
        error("Erreur en partion de U "); 
    }
}
int parseF(){
    if(currentToken()!=NULL){
        if(currentToken()->type==OPAR){
            consume(OPAR);
            int v = parseA();
            consume(FPAR);
            return v;
        }
        else if(currentToken()->type==VAR){
            char * t = get_text(currentToken());
            int v = get_value(t);
            consume(VAR);
            return v;
        }
        else if(currentToken()->type==INT){
            int v = currentToken()->intValue;
            consume(INT);
            return v;
        }
        else {
            error("Token inattendu pour F");
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

    //fprintf(stderr, "Starting parse...\n");
    parseS();

    // ON VERIFIE QUE LA GRAMMAIRE A BIEN TERMINE SON TRAVAIL A LA FIN DU MOT A ANALYSER
    if (currentToken() != NULL) {
        fprintf(stderr, "Unexpected input after assignation.\n");
        fprintf(stderr, "!!! ERREUR !!!\n");
        return 1;
    }
    // ET ON AFFICHE LA TABLE DES SYMBOLES
    print_symbols();
    return 0;
}

