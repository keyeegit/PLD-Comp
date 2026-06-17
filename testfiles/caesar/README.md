# Chiffrement de César

Lit des caractères depuis stdin, applique un décalage de 3 dans l'alphabet, et écrit le résultat sur stdout. Les caractères non-alphabétiques sont transmis sans modification.

## Utilisation

```bash
echo "Hello World" | ./ifcc caesar/caesar.c > caesar.s
gcc caesar.s -o caesar
echo "Hello World" | ./caesar
# Sortie : Khoor Zruog
```

## Tests automatisés

Ce programme utilise `getchar()` et nécessite un stdin pour ne pas boucler indéfiniment. Le fichier `input.txt` est automatiquement pipé en stdin par `ifcc-test.py` si il est présent dans le répertoire du test :

```
testfiles/caesar/
├── caesar.c
├── input.txt   ← pipé en stdin lors du test
└── README.md
```

Pour tous les tests sans `input.txt`, `/dev/null` est utilisé (EOF immédiat).

## Fonctionnalités du compilateur utilisées

| Fonctionnalité | Usage |
|---|---|
| Fonctions + paramètres | `is_upper`, `is_lower`, `shift` |
| `return` partout | 3 `return` dans `shift` |
| `&` bitwise comme AND logique | `c > 64 & c < 91` |
| `%` modulo | wrap autour de l'alphabet |
| `if` sans `else` | deux gardes dans `shift` |
| `while` + `getchar` | lecture jusqu'à EOF (`-1`) |
| `putchar` | affichage du résultat |
| Arithmétique | `+`, `-`, `%` |
| Comparaisons | `>`, `<`, `!=` |
