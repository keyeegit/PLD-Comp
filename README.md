GARNIER Axel, HUDE Dimitri, KUSIAK Guillaume, NADEAU Paul, REYDET Baptiste, TRAN Bao Anh

# Fonctionnalités implémentées

| Fonctionnalité                         | Description                                           |
| -------------------------------------- | ----------------------------------------------------- |
| Opérations arithmétiques de base       | `+`, `-`, `*`                                         |
| Division et modulo                     | `/`, `%`                                              |
| Négation unaire                        | `-expr`                                               |
| Not unaire                             | `!expr`                                               |
| Précédence des opérateurs              | `*`, `/`, `%` avant `+`, `-` ; associativité gauche   |
| Parenthèses                            | Forçage de la priorité d'évaluation                   |
| Déclaration de variable (n'importe où) | `int x;` ou `int x = expr;`                           |
| Affectation de variable                | `x = expr;`                                           |
| Instruction `return`                   | Retour d'une expression entière                       |
| Vérification variable non déclarée     | Erreur si une variable est utilisée sans avoir été déclarée |
| Vérification double déclaration        | Erreur si une variable est déclarée deux fois dans la même portée |
| Avertissement variable inutilisée      | Warning si une variable est déclarée mais jamais lue  |
| Génération x86-64                      | Code assembleur pour Linux / Intel                    |
| Génération ARM64                       | Code assembleur pour macOS Apple Silicon              |
| Opérations de comparaison              | `==`, `!=`, `<`, `>`                                  |
| Opérations de comparaison bit à bit    | `&`, `\|`, `^`                                        |
| Constantes caractère                   | `'a'`, `'A'` — converties en valeur ASCII entière     |
| `putchar`                              | Affichage d'un caractère sur stdout                   |
| `getchar`                              | Lecture d'un caractère depuis stdin                   |
| Gestion de contexte avec les blocs de code { } | Gestion de contexte avec les blocs de code { }|
| Support des portées de variables et du shadowing | Support des portées de variables et du shadowing |
| Les structures de controle if, else    | Les structures de controle if, else                   |
