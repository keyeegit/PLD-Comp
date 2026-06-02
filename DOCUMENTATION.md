# Fonctionnalités implémentées

| Fonctionnalité | Description |
|---|---|
| Opérations arithmétiques de base | `+`, `-`, `*` |
| Division et modulo | `/`, `%` |
| Négation unaire | `-expr` |
| Précédence des opérateurs | `*`, `/`, `%` avant `+`, `-` ; associativité gauche |
| Parenthèses | Forçage de la priorité d'évaluation |
| Déclaration de variable | `int x;` ou `int x = expr;` |
| Affectation de variable | `x = expr;` |
| Instruction `return` | Retour d'une expression entière |
| Table des symboles | Détection des variables non déclarées et des doublons |
| Avertissement variable inutilisée | Warning si une variable est déclarée mais jamais lue |
| Génération x86-64 | Code assembleur pour Linux / Intel |
| Génération ARM64 | Code assembleur pour macOS Apple Silicon |
