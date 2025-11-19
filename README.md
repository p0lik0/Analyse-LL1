## Grammaires utilisée pour les différentes étapes

Donnez ici les grammaires que vous avez utilisées pour résoudre votre problème.

Grammaire initiale :
S → A ; S | A ;
A → x = A | E
E → E + T | E - T | T
T → T ∗ P | T / P | P
P → P ^ U | U
U → + U | - U | F
F → x | a | (A)

Grammaire sans recursivité à gauche :
R0:   S' → S $
R1:   S → A ; S
R2:   S → ε
R3:   A → x = A
R4:   A → E
R5:   E → T E'
R6:   E' → + T E'
R7:   E' → - T E'
R8:   E' → ε
R9:   T → P T'
R10:  T' → * P T'
R11:  T' → / P T'
R12:  T' → ε
R13:  P → U P'
R14:  P' → ^ U P'
R15:  P' → ε
R16:  U → + U
R17:  U → - U
R18:  U → F
R19:  F → x
R20:  F → a
R21:  F → (A)

## Table LL1 calculée
Donnez la table LL1 de votre grammaire.

Premiers
    =   +   -   *   /   ^   (   )   x   a   ;   ε
S'  .   7   7   .   .   .   9   .   3   8   .   .
S   .   6   6   .   .   .   7   .   2   7   .   1
A   .   5   5   .   .   .   6   .   1   6   .   .
E   .   4   4   .   .   .   5   .   5   5   .   .
E'  .   1   1   .   .   .   .   .   .   .   .   1
T   .   3   3   .   .   .   4   .   4   4   .   .
T'  .   .   .   1   1   .   .   .   .   .   .   1
P   .   2   2   .   .   .   3   .   3   3   .   .
P'  .   .   .   .   .   1   .   .   .   .   .   1
U   .   1   1   .   .   .   2   .   2   2   .   .
F   .   .   .   .   .   .   1   .   1   1   .   .

Suivants
    =   +   -   *   /   ^   (   )   x   a   ;   $
S'  .   .   .   .   .   .   .   .   .   .   .   .
S   .   .   .   .   .   .   .   .   .   .   .   x
A   .   .   .   .   .   .   .   x   .   .   x   .
E   .   .   .   .   .   .   .   x   .   .   x   .
E'  .   .   .   .   .   .   .   x   .   .   x   .
T   .   x   x   .   .   .   .   x   .   .   x   .
T'  .   x   x   .   .   .   .   x   .   .   x   .
P   .   x   x   x   x   .   .   x   .   .   x   .
P'  .   x   x   x   x   .   .   x   .   .   x   .
U   .   x   x   x   x   x   .   x   .   .   x   .
F   .   x   x   x   x   x   .   x   .   .   x   .

LL1
    =   +   -   *   /   ^   (   )   x   a   ;   $
S'  .   R0  R0  .   .   .   R0  .   R0  R0  .   .
S   .   R1  R1  .   .   .   R1  .   R1  R1  .   R2
A   .   R4  R4  .   .   .   R4  . R3,R4 R4  .   .
E   .   R5  R5  .   .   .   R5  .   R5  R5  .   .
E'  .   R6  R7  .   .   .   .   R8  .   .   R8  .
T   .   R9  R9  .   .   .   R9  .   R9  R9  .   .
T'  .  R12 R12 R10 R11  .   .  R12  .   .  R12  .
P   .  R13 R13  .   .   .  R13  .  R13 R13  .   .
P'  .  R15 R15 R15 R15 R14  .  R15  .   .  R15  .
U   .  R16 R17  .   .   .  R18  .  R18 R18  .   .
F   .   .   .   .   .   .  R21  .  R19 R20  .   .

On a un conflit dans LL1(A,x)={R3,R4},
car Premiers(A->x=A) ∩ Premiers(A->E) = {x}

## Difficultés rencontrées

Si vous en avez eu...

## Remarques éventuelles

chaque assignation de sequence à traiter doit avoir ';' à la fin.
Par exemple :
x=10 
sera pas accepté par la grammaire, par contre x=10; sera bien accepté.

### Explication de l'état d'avancement de projet:

I. Grammaire
1. J’ai complété la grammaire du TD12 avec les règles demandées.
2. J’ai enlevé la récursivité à gauche.
3. J’ai calculé les ensembles Premiers, Suivants et finalement construit la table LL(1).

II. Analyse syntaxique
4. 
Pour implémenter la grammaire, j’ai créé des parseurs:
   void parseS(); void parseA(); ... void parseF();
pour chaque non-terminal de ma grammaire, sauf S’, car le rôle du symbole $ (fin de l’entrée) est assuré par le lexer et par le test final if(currentToken() != NULL);

Chaque parseur de non-terminal fait une derivation de ce non-terminal:
La table LL(1) définit quelle règle appliquer en fonction du token courant,
et, selon cette règle, on consomme les terminaux attendus et on appelle les parseurs nécessaires.

!!! Pour l’implémentation d’un cas LL(1)[A, x] dans parseA(), où il y a une contradiction,
il faut regarder non seulement le type du token courant, mais aussi celui du token qui le suit.
Et donc si après token courrant VAR, il y a token ASSIGN, alors on aplique R3, sinon - R4.

5. J’ai ajouté un appel de source (parseS()) dans le main de assignation.c pour lancer les dérivations.
6. J’ai testé le programme sur différents cas afin de m’assurer que toutes les erreurs (le traitement des mots qui ne sont pas acceptés par la grammaire) sont bien gérées.

III. Ajout du calcul des expressions
7. 
On garde les consomation de symboles ajoutés precedement, mais on change les appels de parseurs en ajoutant un traitement de calcul suivant:

parseS()
  Reste inchangé;

parseA()
R3 : A → x = A 
  On calcule récursivement la valeur de l’expression à droite et associe cette valeur à x dans la table des variables;
R4 : A → E 
  On retourne directement la valeur calculée par E;

parseE()
R5 : E → T E'
  On calcule T et on transmet cette valeur à parseEp(val) qui applique les opérations additives (+ ou -). Le résultat final est construit dans parseEp;

parseEp(int val)
R6 : E' → + T E'
  On calcule un nouveau T, ajoute sa valeur, puis continue récursivement;
R7 : E' → - T E'
  Idem mais avec une soustraction;
R8 : E' → ε
  Fin d’expression, on retourne la valeur accumulée;

parseT()
R9 : T → P T'
  On calcule P. On transmet cette valeur à parseTp(val) qui applique les opérations multiplicatives (* ou /); 

parseTp(int val)
R10 : T' → * P T'
  On calcule un nouveau P, multiplie, puis continue récursivement;
R11 : T' → / P T'
  Idem mais avec une division;
R12 : T' → ε
  On retourne la valeur accumulée.

parseP()
R13 : P → U P'
  On calcule U. On transmet cette valeur à parsePp(val) qui applique les puissances (^).

parsePp(int val)
R14 : P' → ^ U P'
  On calcule un nouveau U, applique la puissance de U sur val, puis continue récursivement;
R15 : P' → ε
  Fin de puissance, on retourne la valeur accumulée;

parseU()
R16 : U → + U
  On retourne directement la valeur calculée par U;
R17 : U → - U
  On calcule récursivement U et retourne son opposé;
R18 : U → F
  On retourne directement la valeur calculée par F;

parseF()
R19 : F → x
  On récupère la valeur de x dans la table, et on la retourne. Ainsi on monte un nombre dans les appels reccursifs et finalement calcule l'assignation;
R20 : F → a
  Une constante entière : on lit sa valeur, et on la retourne. Ainsi on monte une nombre dans les appels reccursifs et finalement calcule l'assignation;
R21 : F → (A)
  On retourne A;
