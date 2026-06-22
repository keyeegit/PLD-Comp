# PLD-Comp — Compilateur C → x86/ARM64

GARNIER Axel, HUDE Dimitri, KUSIAK Guillaume, NADEAU Paul, REYDET Baptiste, TRAN Bao Anh

---

## Fonctionnalités implémentées

### Obligatoires

- [x] Type de données de base `int` (32 bits)
- [x] Variables et constantes entières / caractère (`'a'`, `'A'`)
- [x] Opérations arithmétiques : `+`, `-`, `*`
- [x] Division et modulo : `/`, `%`
- [x] Opérations logiques bit-à-bit : `&`, `|`, `^`
- [x] Opérations de comparaison : `==`, `!=`, `<`, `>`
- [x] Opérations unaires : `!expr`, `-expr`
- [x] Précédence des opérateurs et associativité gauche
- [x] Parenthèses
- [x] Déclaration de variables n'importe où (`int x;` ou `int x = expr;`)
- [x] Affectation de variable (`x = expr;`)
- [x] `putchar` et `getchar`
- [x] Définition de fonctions avec paramètres (`int` ou `void`)
- [x] Vérification de la cohérence des appels de fonctions
- [x] Structure de blocs `{ }`
- [x] Portées de variables et *shadowing*
- [x] Structures de contrôle : `if`, `else`, `while`
- [x] `return expr;` n'importe où dans une fonction
- [x] Vérification variable non déclarée
- [x] Vérification double déclaration dans la même portée
- [x] Avertissement variable déclarée mais inutilisée
- [x] Génération x86-64 (Linux / Intel)

### Facultatives

- [x] Génération ARM64 (macOS Apple Silicon) — reciblage multi-architecture
- [x] Tableaux à une dimension (déclaration, accès, affectation)
- [ ] Support des `double` avec conversions implicites
- [ ] Propagation de constantes simple
- [ ] Propagation de variables constantes (analyse *data-flow*)
- [ ] Pointeurs
- [ ] `break` et `continue`
- [ ] Chaînes de caractères (tableaux de `char`)
- [ ] `switch...case`
- [x] Opérateurs logiques paresseux `||`, `&&`
- [ ] Opérateurs d'affectation `+=`, `-=`, `++`, `--`

### Non prioritaires (NP)

- [ ] Variables globales
- [ ] Autres types (`inttypes.h`, `float`)
- [ ] Opérateurs `<=`, `>=`, `<<`, `>>`
- [ ] Structures de contrôle `for`, `do...while`

###  Déconseillées (D)

- [ ] Séparation déclarations / définitions dans plusieurs fichiers
- [ ] Support du préprocesseur (`#define`, `#include`, `#if`)
- [ ] Structures et unions
- [ ] Type `char` 8 bits complet
