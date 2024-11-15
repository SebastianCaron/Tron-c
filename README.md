# Tron-c
An amazing game called Tron using C and libs SDL and ncurses



## 2024 - Développement d'un Jeu Tron avec Interfaces Ncurses et SDL en utilisant le Modèle MVC
Introduction : Le jeu Tron est un jeu adapté du film du même nom et sorti en 1982. Dans ce projet, vous êtes chargé de développer une version de Tron qui peut être jouée à la fois en utilisant l'interface ncurses et l'interface SDL. En plus de cela, le projet doit respecter le modèle de conception MVC (Modèle-Vue-Contrôleur) pour garantir une séparation claire entre la logique du jeu, l'interface utilisateur et le contrôle des événements.

> Objectif : L'objectif principal de ce projet est de concevoir et de développer un jeu Tron fonctionnel avec deux interfaces utilisateur (ncurses et SDL) en utilisant le modèle MVC.

### Fonctionnalités Requises :

1. Modèle : Implémentez la logique du jeu Tron. Cela comprend la gestion des "motos", leur déplacement, la détection de collision et le calcul du score.
2. Vue (Ncurses) : Créez une interface utilisateur en utilisant la bibliothèque ncurses. La vue doit afficher le plateau de jeu, les motos, leur "mur de lumière" (positions précédentes), le score et toute autre information pertinente.
3. Vue (SDL) : Créez une interface utilisateur graphique en utilisant la bibliothèque SDL. La vue SDL doit avoir les mêmes fonctionnalités que la vue ncurses.
4. Contrôleur : Gérez les entrées utilisateur (clavier) pour faire changer les motos de direction. Assurez-vous que le contrôleur est capable de gérer les événements provenant des deux interfaces.


### Exigences Techniques :

Le jeu doit pouvoir basculer en douceur entre les interfaces ncurses et SDL en fonction de la préférence de l'utilisateur (au lancement du jeu).
Le modèle doit être indépendant des vues et du contrôleur, conformément au modèle MVC.
Utilisez des structures de données appropriées pour représenter le plateau de jeu, le score, etc.
Assurez-vous que le code est modulaire, bien commenté et respecte les bonnes pratiques de programmation en C.


### Bonus (Facultatif) :

Implémentez des fonctionnalités supplémentaires telles que la gestion des niveaux, des effets sonores, des animations, ou toute autre amélioration du gameplay. Vous pouvez par exemple créer un mode ou les directions sont relatives au déplacement de la moto, et pas à l'écran (si la moto va vers le bas, le joueur doit sélectionner la direction gauche pour aller vers la droite de l'écran)
Livrables :

Code source complet avec des commentaires explicatifs.
Instructions détaillées pour compiler et exécuter le jeu avec les deux interfaces.
Une vidéo de 10 minutes expliquant l'architecture du projet, les choix de conception, les difficultés rencontrées et les solutions apportées. Et vous terminerez par une petite démo montrant votre comment fonctionne votre jeu avec les deux interfaces.
Évaluation : Vous serez évalués sur la qualité du code, la conception MVC, la fonctionnalité des interfaces ncurses et SDL, la gestion des événements, ainsi que sur toute fonctionnalité bonus implémentée. Votre code doit pouvoir être exécuté sur Linux.

### Conseils :

Commencez par concevoir l'architecture MVC de votre application avant de plonger dans l'implémentation.
Testez votre jeu rigoureusement pour vous assurer qu'il est jouable et exempt de bugs. Vérifier si vous n'avez pas de fuites de mémoire.
Bonne chance dans votre projet ! N'hésitez pas à poser des questions si vous en avez.
```
