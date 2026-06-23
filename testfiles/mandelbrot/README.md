# Fractal de Mandelbrot (ASCII Art)

Ce programme calcule et dessine la célèbre fractale de Mandelbrot en ASCII art dans le terminal. Comme notre compilateur ne supporte pas encore les nombres à virgule flottante (`float` / `double`), le programme utilise de l'arithmétique en virgule fixe (avec un facteur d'échelle de 4096) pour effectuer les calculs complexes nécessaires.

## Utilisation

Pour compiler et exécuter le programme avec notre compilateur `ifcc` :

```bash
# Générer l'assembleur x86-64
./compiler/ifcc testfiles/mandelbrot/mandelbrot.c > mandelbrot.s

# Assembler avec gcc
gcc mandelbrot.s -o mandelbrot

# Exécuter
./mandelbrot
```

## Fonctionnalités du compilateur démontrées

Ce programme est une excellente démo pour le jury car il sollicite intensément toutes les fonctionnalités majeures de notre compilateur :

| Fonctionnalité | Usage dans la démo |
|---|---|
| **Appels de fonctions** | Définition de `renderMandelbrot()` et appel depuis `main()`. |
| **Boucles imbriquées** | 3 niveaux de boucles `while` (lignes $y$ $\rightarrow$ colonnes $x$ $\rightarrow$ itérations complexes $z_{n+1} = z_n^2 + c$). |
| **Arithmétique complexe** | Utilisation intensive de multiplications, divisions, additions et soustractions d'entiers. |
| **Opérations relationnelles** | Conditions d'échappement et de boucles (`<`, `>`). |
| **Structures conditionnelles** | Cascade de blocs `if...else` complexes pour faire correspondre le nombre d'itérations à des caractères de dégradé ASCII (ombrage). |
| **Interaction système** | Utilisation de `putchar()` pour l'affichage fluide caractère par caractère. |

## Rendu attendu dans le terminal

```
                     ..................................-+--...............
                  .....................................--*+----..............
                ......................................----*+**--..............
             ........................................----++#+----.............
           ........................................---**+*##%+++--............
          .....................................-------+########+---...........
        ....................................----------+%#######+---------.....
       ..................................--+++%+--%*+++*#####%*+#+++------%-..
      .................................----**%##***#################+++%++++-.
     .................................------+#############################+--.
    ...............................------+#+*%###########################+---.
   ...................-------------------+%##############################%+---
  ..................---++-----++--------+%##################################*-
  ..................----+**++++#**++--++##################################%#--
 ..................-----+*##%######%++++####################################+-
 .................----+**#############**###################################+--
 ..........-------+++++%###############%###################################--.
 ....-------------++%##%#################################################*---.
#######################################################################*+----.
 ....-------------++%##%#################################################*---.
 ..........-------+++++%###############%###################################--.
 .................----+**#############**###################################+--
 ..................-----+*##%######%++++####################################+-
  ..................----+**++++#**++--++##################################%#--
  ..................---++-----++--------+%##################################*-
   ...................-------------------+%##############################%+---
    ...............................------+#+*%###########################+---.
     .................................------+#############################+--.
      .................................----**%##***#################+++%++++-.
       ..................................--+++%+--%*+++*#####%*+#+++------%-..
        ....................................----------+%#######+---------.....
          .....................................-------+########+---...........
           ........................................---**+*##%+++--............
             ........................................----++#+----.............
                ......................................----*+**--..............
                  .....................................--*+----..............
```
