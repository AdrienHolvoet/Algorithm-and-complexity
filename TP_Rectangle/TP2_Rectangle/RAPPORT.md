# TP2_Rectangle

* n = la taille de l'entrée
* l h : Deux entiers strictement positifs : Dimension de l'espace des points
* x_i y_i : n points de coordonnées (x_i, y_i), triés dans l'ordre des abscisses croissantes où l'on rajoute deux points (0,0) et (l,0)
* La base du rectangle se trouve sur l'axe des x

## Question 1

- Algorithme en Θ(n^3) , c'est l'algo naîf, très lent sur des grosses entrées .Il faut se concentrer sur le calcul du plus grand rectangle entre deux points A,B, puis A,C, puis A,D, ... où la surface est égale à la longueur entre ces deux points(leurs abscisses) fois la hauteur qui est égale à l'ordonnée minimale des points compris entre les deux points. Par la suite comparer toutes ses surfaces trouvées et retenir la meilleure.

Pour cette algo on retrouve 3 boucles "for" imbriquées l'une dans l'autre qui parcourent pour chacune plus ou moins n fois l'entrée ce qui équivaut à un algo en O(n^3).


- Algorithme en Θ(n^2) , beaucoup plus rapide que l'algo O(n^3) mais toujours très lent pour des tailles d'entrées très grandes .Pour le O(n^2) l'idée de base est la même que l'ago précédent mais pour celui-ci, j'arrête de recalculer à chaque fois l'ordonnée minimale des points compris entre deux points mais je garde l'ordonnée minimal e déjà calculée  entre deux points et je la compare avec la suivante. Comme ceci : si Ordmin(i,j) est l'ordonnée minimale des points compris entre les points d'index i et j alors Ordmin(i,j+1) est égale au minimum entre Ordmin(i,j) et l'ordonnée du point j+1.

Pour cette algo on retrouve 2 boucles "for" imbriquées l'une dans l'autre qui parcourt pour chacunes plus ou moins n fois l'entrée ce qui équivaut à un algo en O(n^2).


- Ces deux algos ci-dessus ne sont pas assez efficaces pour des tailles d'entrées très grandes. En effet, ils ne s’exécuteront pas en moins de 1s pour n=100.000.


## Question 2

- Approche : Diviser de manière récursive chaque rectangle en 2 sous-rectangles ou l'ordonnée minimale du point se trouvant à l'intérieur de celui-ci est la plus petite, jusqu'à ce que l'ordonnée minimale soit égale à h. On calcule l'air de chaque sous-rectangle et on les compare entre elles pour garder la plus grande.
    * L'aire où l'ordonnée minimale  est égale à h on a : surface = distance entre le point le plus à gauche et le point le plus à droite * h
    * Les autres aires on a : surface = distance entre le point le plus à gauche du 1er sous-rectangle et le point le plus à droite du 2ème sous-rectangle*ymin
    <br/>
  
- Cet algorithme "Diviser pour régner" est plus efficace que l'algo O(n^3) et prend assez vite le dessus sur l'algo O(n^2) en terme d'efficacité. En effet, sur des petites tailles de données l'algo O(n^2)est plus rapide que le "Diviser pour régner" mais dans la majorité des cas lorsque la taille des données devient grande, l'algo "Diviser pour régner" devient plus efficace.

- Dans le pire des cas on a une compléxité en O(n^2) et dans le meilleur des cas en O(n*log(n))

## Question 3 

/
