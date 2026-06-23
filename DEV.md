# Documentation développeur — PLD-Comp

**Dépôt :** [https://github.com/keyeegit/PLD-Comp](https://github.com/keyeegit/PLD-Comp)

**Auteurs :** GARNIER Axel, HUDE Dimitri, KUSIAK Guillaume, NADEAU Paul, REYDET Baptiste, TRAN Bao Anh

---

## Vue d'ensemble

PLD-Comp est un compilateur d'un sous-ensemble du C vers de l'assembleur x86-64 ou ARM64. Il est structuré en trois passes classiques :

```
Source C
  └─▶ [Lexer / Parser ANTLR4]  → Arbre syntaxique (AST)
        └─▶ [SymbolTableVisitor]  → Tables de symboles + vérifications sémantiques
              └─▶ [IRGenVisitor]    → Représentation intermédiaire (IR)
                    └─▶ [x86Backend | ARMBackend]  → Assembleur
```

---

## Structure du projet

```
compiler/
├── ifcc.g4                  # Grammaire ANTLR4
├── main.cpp                 # Point d'entrée du compilateur
├── config.mk                # Chemins ANTLR (à configurer par machine)
├── Makefile
├── frontend/
│   ├── SymbolTableVisitor.h
│   └── SymbolTableVisitor.cpp   # Passe 1 : analyse sémantique
├── ir/
│   ├── IRInstr.h                # Définition des instructions IR
│   ├── IRGenVisitor.h
│   ├── IRGenVisitor.cpp         # Passe 2 : génération de l'IR
│   └── IRBackend.h              # Interface abstraite des backends
├── backend/
│   ├── x86Backend.h / .cpp      # Passe 3 : génération x86-64
│   └── ARMBackend.h / .cpp      # Passe 3 : génération ARM64
└── generated/                   # Fichiers auto-générés par ANTLR (ne pas éditer)
```

---

## Passe 1 — Analyse sémantique (`SymbolTableVisitor`)

Cette passe parcourt l'AST et construit les tables de symboles. Elle détecte les erreurs sémantiques avant toute génération de code.

### Structures de données principales

| Champ | Type | Rôle |
|---|---|---|
| `funcTable` | `map<string, FuncInfo>` | Signatures des fonctions (type de retour + types des paramètres) |
| `funcSymbolTables` | `map<string, map<string, int>>` | Tables de symboles locaux par fonction (nom → offset sur la pile) |
| `scopeStack` | pile de maps | Portées imbriquées pour gérer le *shadowing* |
| `usedVars` | `set<string>` | Variables effectivement utilisées (pour les avertissements) |

### Gestion du shadowing

Les variables sont renommées en `varname_b<profondeur>` (ex. `a_b1`, `a_b2`) pour différencier les déclarations de même nom dans des portées imbriquées.

### Vérifications effectuées

- Variable utilisée sans être déclarée
- Double déclaration dans la même portée
- Appel de fonction avec un mauvais nombre d'arguments
- Avertissement si une variable est déclarée mais jamais utilisée

---

## Passe 2 — Génération IR (`IRGenVisitor`)

Cette passe traduit l'AST en une liste d'instructions IR indépendantes de l'architecture cible.

### Structure d'un programme IR (`IRProgram`)

```cpp
struct IRProgram {
    string funcName;
    string returnType;          // "int" ou "void"
    map<string, int> symbols;   // nom → offset pile
    vector<IRInstr> instrs;
};
```

### Jeu d'instructions IR

| Instruction | Effet |
|---|---|
| `LDCONST` | Charger une constante dans une variable |
| `COPY` | Copier une variable dans une autre |
| `ADD`, `SUB`, `MUL`, `DIV`, `MOD` | Arithmétique binaire |
| `NEG` | Négation unaire |
| `CMP_EQ`, `CMP_NEQ`, `CMP_LT`, `CMP_GT` | Comparaisons (résultat 0 ou 1) |
| `CMP_AND`, `CMP_OR`, `CMP_XOR` | Opérations bit-à-bit |
| `NOT` | NON logique |
| `CMP_CBR` | Saut conditionnel (si zéro) |
| `JMP` | Saut inconditionnel |
| `LABEL` | Étiquette de code |
| `RET` | Retour de fonction |
| `PARAM` | Déclaration d'un paramètre |
| `CALL` | Appel de fonction |
| `PUTCHAR`, `GETCHAR` | Entrées/sorties |
| `STORE_ARRAY`, `LOAD_ARRAY` | Accès aux tableaux |

### Conventions de nommage

- Variables temporaires : `_t0`, `_t1`, `_t2`, …
- Étiquettes de saut : `.L0`, `.L1`, `.L2`, …

### Opérateurs court-circuit (`&&`, `||`)

Les opérateurs logiques paresseux sont traduits avec des labels et des `CMP_CBR`/`JMP` pour éviter d'évaluer le second opérande si inutile.

---

## Passe 3 — Génération de code

Les deux backends héritent de l'interface abstraite `IRBackend` et implémentent `emitInstr()`.

### Backend x86-64 (`x86Backend`)

- Convention d'appel : System V AMD64 ABI
- Registres de travail : `%eax`, `%ecx`, `%edx`
- Arguments de fonctions : `%edi`, `%esi`, `%edx`, `%ecx`, `%r8d`, `%r9d`
- Frame : `%rbp` (base), `%rsp` (sommet de pile)
- La pile est alignée sur 16 octets (`stackSize()`)
- Adressage mémoire : `offset(%rbp)`

### Backend ARM64 (`ARMBackend`)

- Convention d'appel : AArch64 (macOS)
- Registres de travail : `w8`, `w9`, `w10`
- Arguments / retour : `w0`–`w7`
- Frame : `x29` (frame pointer), `x30` (link register)
- Sauvegarde du frame : `stp x29, x30, [sp, #-N]!`
- Adressage mémoire : `[x29, #offset]`
- Les symboles externes sont préfixés par `_` (convention macOS)

---

## Grammaire (`ifcc.g4`)

Le fichier ANTLR4 définit le langage accepté. Les règles principales :

- `prog` : séquence de définitions de fonctions
- `func_def` : `int|void ident(params) { block }`
- `stmt` : déclaration, affectation, `if`, `while`, `return`, appel, affectation de tableau
- `expr` : expressions avec précédence et associativité gérées par la hiérarchie de règles ANTLR

ANTLR génère automatiquement dans `generated/` : le lexer, le parser, et les classes visiteurs de base.

---

## Système de build

```bash
cd compiler
make          # Compile ifcc
make clean    # Supprime build/ et generated/
make gui      # Affiche l'arbre syntaxique dans l'interface graphique ANTLR (Java)
```

La cible `generated/ifccParser.cpp` déclenche la génération ANTLR si la grammaire est modifiée.

La configuration des chemins ANTLR se fait dans [compiler/config.mk](compiler/config.mk). Des exemples pour chaque environnement sont commentés dans ce fichier.

---

## Tests

```bash
python3 ifcc-test.py testfiles/
```

Chaque sous-dossier de [testfiles/](testfiles/) contient un ou plusieurs `.c` à compiler. Le script compare la sortie du compilateur à la référence gcc.

---

## Ajouter une fonctionnalité

1. **Grammaire** — modifier `ifcc.g4` si une nouvelle syntaxe est nécessaire, puis relancer `make`.
2. **Analyse sémantique** — ajouter les vérifications dans `SymbolTableVisitor`.
3. **IR** — ajouter une ou plusieurs entrées dans `IRInstr::Op` et implémenter la génération dans `IRGenVisitor`.
4. **Backends** — implémenter le cas correspondant dans `x86Backend::emitInstr()` et `ARMBackend::emitInstr()`.
5. **Tests** — ajouter des cas de test dans `testfiles/`.
