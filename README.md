# Instructions

Pour pouvoir compiler le projet sur votre ordinateur, il vous faut installer les librairies flex et bison.

- Sur linux (ubuntu) cela se fait par la commande :
  
  sudo apt-get install flex bison
  
  (ou équivalent sur d'autres distributions)

- Sur mac, ces librairies sont installées en standard. Si ce n'est pas le cas pour vous, cela peut s'installer via homebrew (cf https://brew.sh/index_fr)

  brew install flex bison

- sur windows, je ne sais pas, à vous de chercher.

# Compte rendu

Je vous demande de rédiger votre compte rendu dans ce fichier que vous rendrez avec vos programmes sur caseine. Je vous suggère les éléments suivants:

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
    =   +   -   *   /   ^   (   )   x   a   ;   ε   $
S'  .   7   7   .   .   .   9   .   3   8   .   .   2
S   .   6   6   .   .   .   7   .   2   7   .   1   .
A   .   5   5   .   .   .   6   .   1   6   .   .   .
E   .   4   4   .   .   .   5   .   5   5   .   .   .
E'  .   1   1   .   .   .   .   .   .   .   .   1   .
T   .   3   3   .   .   .   4   .   4   4   .   .   .
T'  .   .   .   1   1   .   .   .   .   .   .   1   .
P   .   2   2   .   .   .   3   .   3   3   .   .   .
P'  .   .   .   .   .   1   .   .   .   .   .   1   .
U   .   1   1   .   .   .   2   .   2   2   .   .   .
F   .   .   .   .   .   .   1   .   1   1   .   .   .

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
car Premiers(x=A) ∩ Premiers(E) = {x}

## Difficultés rencontrées

Si vous en avez eu...

## Remarques éventuelles

Si nécessaires...
