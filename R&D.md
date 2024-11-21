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

### Notes

Création du serveur :  Création du Socket -> SetSockOptions -> Bind -> Listen -> Accept -> Send and Receive

Création du client :  Création du Socket -> Connect -> Send and Receive

Listen(fd, ln_queue);

Accept() -> retourne un fd de connexion

Connect() -> Retourne le status de connexion

send(fd, char *, len, flag) 

read(fd, buffer, size);

write(fd, buffern size);

close(fd);

Bind(fd, sockaddr, socklen) : nomme le socket

Socket(AF_INET, SOCK_STREAM, 0) pour TCP ou Socket(AF_INET, SOCK_DGRM, 0) pour UDP. Retourne un fd

> NB : Send and receive fonctions bloquantes.

Utiliser Select pour plusieurs FileDescriptors, readfds, writefds non bloquants.


fork => Retourne le pid_enfant dans le parent et 0 dans le child.

