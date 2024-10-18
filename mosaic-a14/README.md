# mosaic-a14

Règles

Selon la collection "Portable Puzzle" de Simon Tatham :

On a une grille de cases qu'il faut colorer en noir ou en blanc. Certainnes cases contiennent des indices (dites aussi contraintes). Chaque indice nous indique le nombre de cases noirs dans la région 3X3 entourant la case de l'indice - y compris la case elle-même.

Donc, pour jouer à ce jeu, nous devons choisir une couleur (blanc ou noir) pour chaque case afin de remplir toute la grille. Par exemple, si une case vide contient le nombre 0, le voisinage 3x3 doit être blanc. De même, si un carré contient le nombre 9, le voisinage 3X3 sera noir.

De plus, nous limiterons notre jeu à une grille carrée de taille 5x5.


Par convention, la case à la ligne i et la colonne j de la grille a pour coordonnée (i, j). Par exemple la coordonnée (0, 0) correspond au coin supérieur gauche.

Interface

L'interface est organisée afin d'avoir la grille entourée de boutons.

Afin de changer la couleur d'une case de la grille, il faut appuyer sur la case qu'on désire changer jusqu'à obtenir la couleur voulu (blanc, noir ou vide). Nous avons à disposition sur le côté gauche de la grille les boutons "HELP" qui affiche une aide sur le terminal, "RESET" qui remet la grille au point de départ en la vidant et "QUIT" qui permet de quitter le jeu. En dessous de la grille nous avons aussi les bouttons "UNDO" pour annuler un coup et "REDO" pour rejouer un coup annulé.

Nous pouvons aussi quitter le jeu en appuyant sur la touche échap du clavier. Il est aussi possible d'enregistrer la partie qu'on est entrain de jouer en appuyant sur la touche "s", la grille sera sauvegarder dans un fichier "sdl_save.txt". Si nécessaire nous pouvons aussi avoir la solution du jeu avec la touche "o".

Auteurs : INDRANO Loren, ROBERT Wendy