# Container in C

Ce projet illustrant la création d'un conteneur minimaliste en utilisant le langage C et les appels système Linux.

## Contenu

Le code fourni dans ce projet comprend :

- `container.c` : Le code source principal qui crée un conteneur basique en utilisant des appels système comme `clone()`, `mount()` et d'autres fonctionnalités du système Linux.
- `README.md` : Ce fichier, fournissant des informations sur le projet.

## Fonctionnalités

Ce conteneur basique met en œuvre les fonctionnalités suivantes :

- Création d'un processus fils isolé dans un nouveau namespace.
- Montage d'un système de fichiers minimaliste.
- Partage d'un dossier spécifique de l'hôte avec le conteneur via un "bind mount".
- Restreindre les ressources du conteneur.
- Isoler l'interface réseau du conteneur.
- Restreindre les capacités du conteneur.

## Utilisation

Compilez le code en utilisant un compilateur C 

```bash
gcc -o container container.c
```

Exécutez le programme :

```bash
./container
```