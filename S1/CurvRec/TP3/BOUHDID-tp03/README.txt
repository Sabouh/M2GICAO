Les fonctions estimatePlane et estimateQuadric ont été modifiées

On choisit celle qu'on veut utiliser dans la méthode apply
La méthode estimateColorPlane est plus rapide par rapport à la méthode estimateCOlorQuadric
Cependant, celle-ci est moins efficace que la seconde lorsqu'on a beaucoup de bruit.
Et notamment, lorsqu'on essaye d'enlever le bruit procédé grâce à la 2nde fonction dans le main.


EstimateSigma est une méthode qui permet de calculer un sigma adéquat pour chaque pixel de l'image.
On prend en compte le nombre de voisins présents et on renvoie le sigma pour lequel le voisinage associé
est composé d'au moins (3*sigma)/2 pixels.
 On borne aussi cette recherche par un entier allant de 10 à 20 (loop < int) qui doit être adapté à la méthode
de reconstruction appelée. 
Lorsqu'on utilise la méthode estimateSigma, le calcul de l'image reconstruite est bien plus rapide que lorsqu'on a
un sigma fixé.

Remarque: lorsque j'execute le programme sur les ordinateurs de l'université, avec la fonction estimateColorQuadric, 
je retrouve une image sans bruit. Mais quand je l'execute chez moi, j'ai toujours du bruit. Cela avec le même code.




