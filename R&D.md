# Développement du Projet

## Fonctionnalités Susceptibles d'être ajoutées

- Multijoueur en Réseau
- IA (Q-Learning)

- Effets Néon sur les Murs de Lumières

## Modele MVC en quelques mots

Le Modèle contient les données de jeux et la logique de du jeux (vérifie les collisions etc.).

La Vue (ou les vues SDL, ncurses) représente l'interface d'interactions pour l'utilisateur.

Le contrôleur fait le lien entre la vue et le modéle. Le controleur traite les actions utilisateurs et modifie la vue et le modèle.

## Pseudo-code du déroulement d'une partie


```
fonction Jeu ( Modele logique, Vue ui ):

    ui->afficher()
    
    TANT QUE    logique->est_fini() == FALSE    {

        ui->afficher()

        E_direction d_joueur1 = ui->new_direction?();
        E_direction d_joueur2; # BOT / IA ou MULTIJOUEUR

        logique->bouger_j1(d_joueur1);
        logique->bouger_j2(d_joueur2);

        logique->detection_collision();

    }

    ui->affiche_fin();

```



## Recherche Serveur/Client TCP

### Liens :

- [rsInfo socket/serv/client](https://ressourcesinformatiques.com/article.php?article=824)
- [fork](https://www.geeksforgeeks.org/fork-system-call/)
- [socket](https://www.geeksforgeeks.org/socket-programming-cc/)
- [socket-clients](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)
- [serveur-client implementation](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
- [sockets détails](https://broux.developpez.com/articles/c/sockets/#LIV)


\+ Man Pages
