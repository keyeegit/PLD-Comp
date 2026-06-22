# ASCII Art — Losange

Dessine un losange de taille 5 avec `putchar`, sans aucune entrée stdin.

## Sortie attendue

```
    *
   ***
  *****
 *******
*********
 *******
  *****
   ***
    *
```

## Utilisation

```bash
./ifcc ascii_art/ascii_art.c > ascii_art.s
gcc ascii_art.s -o ascii_art
./ascii_art
```

## Fonctionnalités du compilateur utilisées

| Fonctionnalité | Usage |
|---|---|
| Fonctions `void` | `print_spaces`, `print_stars`, `draw_diamond` |
| Fonctions + paramètres | chaque fonction prend `int n` |
| `while` imbriqués | boucle externe (lignes) + boucles internes (caractères) |
| Arithmétique | `2 * i + 1`, `n - 1 - i`, `2 * (n - 1 - i) + 1` |
| Constantes char | `' '` (espace), `'*'` (étoile) |
| `putchar` | affichage caractère par caractère |
| `putchar(10)` | saut de ligne (`'\n'` = ASCII 10) |
| Deux boucles séquentielles | moitié haute puis moitié basse du losange |
| `return` sans valeur | fonctions `void` |
