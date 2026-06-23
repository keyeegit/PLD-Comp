# PLD-Comp — Compilateur C vers assembleur

Compilateur d'un sous-ensemble du langage C, générant du code assembleur x86-64 (Linux) ou ARM64 (macOS Apple Silicon).

**Auteurs :** GARNIER Axel, HUDE Dimitri, KUSIAK Guillaume, NADEAU Paul, REYDET Baptiste, TRAN Bao Anh

---

## Prérequis

- `g++` (C++17)
- `make`
- ANTLR 4.13 (jar + runtime C++)

Configurez les chemins ANTLR dans [compiler/config.mk](compiler/config.mk) selon votre environnement (des exemples sont déjà présents dans le fichier pour chaque système).

---

## Compilation du compilateur

```bash
cd compiler
make
```

L'exécutable `ifcc` est généré dans le dossier `compiler/`.

---

## Utilisation

```bash
./ifcc source.c
```

Le code assembleur est écrit sur la **sortie standard**. Pour produire un fichier puis l'exécuter :

```bash
./ifcc source.c > source.s
gcc source.s -o mon_programme -no-pie
./mon_programme
```

> Sur macOS (ARM64), l'assembleur généré cible Apple Silicon automatiquement.

---

## Langage supporté

Le compilateur accepte un sous-ensemble du C :

- Types : `int`, constantes caractères (`'a'`)
- Opérations : `+`, `-`, `*`, `/`, `%`, `&`, `|`, `^`
- Comparaisons : `==`, `!=`, `<`, `>`
- Opérateurs logiques paresseux : `&&`, `||`
- Opérateurs unaires : `-expr`, `!expr`
- Déclaration de variables n'importe où dans un bloc
- Tableaux à une dimension
- Fonctions avec paramètres `int` ou `void`, et `return`
- Structures de contrôle : `if / else`, `while`
- Entrées/sorties : `putchar`, `getchar`
- Portées imbriquées (*shadowing*)

### Vérifications à la compilation

Le compilateur signale les erreurs suivantes :

- Variable utilisée sans être déclarée
- Double déclaration dans la même portée
- Appel de fonction avec un mauvais nombre d'arguments
- Avertissement si une variable est déclarée mais jamais utilisée

---

## État des fonctionnalités

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

### Non prioritaires

- [ ] Variables globales
- [ ] Autres types (`inttypes.h`, `float`)
- [ ] Opérateurs `<=`, `>=`, `<<`, `>>`
- [ ] Structures de contrôle `for`, `do...while`

### Déconseillées

- [ ] Séparation déclarations / définitions dans plusieurs fichiers
- [ ] Support du préprocesseur (`#define`, `#include`, `#if`)
- [ ] Structures et unions
- [ ] Type `char` 8 bits complet

---

## Tests

Un script de test est fourni pour valider le compilateur :

```bash
cd ..
python3 ifcc-test.py testfiles/
```

Les cas de test se trouvent dans le dossier [testfiles/](testfiles/).
