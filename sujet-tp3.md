# Travail pratique 3: Maintenance de l'application `kover`

Ce document décrit le travail pratique 3 du cours [INF3135 Construction et
maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135) au
trimestre d'hiver 2025. Le cours est enseigné par Alexandre Blondin Massé,
chargé de cours au [département d'informatique](http://info.uqam.ca/),
à l'[Université du Québec à Montréal](https://uqam.ca/).

Le travail doit être remis au plus tard le **dimanche 27 avril à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.

## Objectifs pédagogiques

Les principaux objectifs pédagogiques visés sont les suivants:

* Poursuivre l'apprentissage du langage de **programmation C**
* Organiser adéquatement votre développement à l'aide des **branches** d'un
  logiciel de contrôle de versions (Git)
* Étendre le développement d'une application de façon **modulaire**
* Utiliser adéquatement une **bibliothèque tierce**
* Continuer à respecter un **style de programmation imposé**
* Rédiger des **tests fonctionnels** couvrant les modifications que vous avez
  apportées à l'application
* Vous assurer de ne pas introduire de **régression** lorsque vous apportez des
  modifications à un programme
* Mettre à jour la **documentation** pour refléter les modifications apportées
  à l'application

## Description du travail

Lors du [travail pratique 1](sujet-tp1.md), vous avez eu à mettre en place les
bases d'une application nommée `kover`. Dans le [travail pratique
2](sujet-tp2.md), on vous a demandé d'améliorer cette application en proposant
trois groupes de modifications, organisés autour de branches Git.

Dans ce troisième travail, vous devez encore une fois apporter différentes
modifications à cette application, que ce soit pour des raisons de maintenance,
d'ajout de nouvelles fonctionnalités ou de réorganisation de la base de code.
Afin de s'assurer que tout le monde travaille à partir de la même base et pour
faciliter la correction, nous vous fournissons la solution du second travail
pratique. Vous devrez donc dupliquer (*fork*) le dépôt courant dans votre
espace personnel, le rendre privé (afin d'éviter le plagiat) et donner accès
aux utilisateurs `blondin_al` et `gosset.guillaume` en mode *Maintainer*. Noter
que le correcteur est différent de celui qui a corrigé les deux premiers
travaux pratiques.

Vous devez accomplir les 4 tâches suivantes:

1. `refactor-scene`: Séparer le module `scene` (fichiers `scene.h` et
   `scene.c`), qui commence à devenir gros, en trois modules: `scene`,
   `antenna` et `construction`.
2. `dynamic-allocation`: Rendre l'application plus flexible, en utilisant
   l'allocation dynamique pour représenter une scène
3. `fix-overlap`: Corriger un bogue actuel de l'application, qui ne détecte pas
   tous les types de chevauchements possibles.
4. `draw-scene`: Produire une image au format SVG d'une scène à l'aide de la
   bibliothèque Cairo

Ces tâches sont détaillées dans les sections suivantes. Noter qu'elles doivent
être réparties sur des branches nommées exactement `refactor-scene`,
`fix-overlap`, `dynamic-allocation` et `draw-scene`, en suivant la syntaxe
`kebab-case` pour la nomenclature.

## Tâches préliminaires à accomplir

Dans un premier temps, vous devez accomplir les actions suivantes:

1. Lisez le sujet du travail jusqu'à la fin
2. Créez un clone (*fork*) du [dépôt du
   projet](https://gitlab.info.uqam.ca/inf31351/20251/inf3135-251-tp3)
3. Assurez-vous que le dépôt soit privé, pour éviter le plagiat
4. Donnez accès à votre dépôt aux utilisateurs `blondin_al` et
   `gosset.guillaume` en mode *Maintainer* (pour la correction)
5. Familiarisez-vous avec le contenu du dépôt, en étudiant chacun des fichiers
   présents (`README.md`, répertoire `bats`, `.gitlab-ci.yml`, etc.).
6. Testez les différentes cibles des fichiers Makefile, notamment `make build`
   et `make test`

Vous êtes maintenant prêt ou prête à commencer les tâches demandées!

## Tâche 1: `refactor-scene`

La première tâche consiste à réorganiser (réusiner ou refactoriser) le module
`scene`, qui commence à être volumineux. Il s'agit essentiellement de créer
deux nouveaux modules, `antenna` et `construction`, qui regroupent les
déclarations de types et les fonctions spécifique aux antennes et aux
constructions (*building* ou *house*).

En particulier, le comportement de votre programme devrait rester exactement le
même qu'avant les modifications. Ainsi, quand on lance la suite de tests, on
s'attend à ce qu'il n'y ait aucune différence.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `refactor-scene`:

1. Créer des fichiers `antenna.h`, `antenna.c`, `construction.h` et
   `construction.c`
2. Déplacer toutes les déclarations et définitions pertinentes qui se trouvent
   dans `scene.h` et `scene.c` vers les 4 fichiers créés
3. Au besoin, renommer les types et/ou les fonctions pour uniformiser la
   nomenclature des nouveaux modules
4. Mettre à jour les fichiers Makefiles pour adapter la compilation de ces
   nouveaux modules
5. S'assurer que le comportement du programme reste le même qu'avant les
   modifications
6. Mettre à jour les *docstrings* s'il y a lieu.
7. Vous assurer de respecter le style de programmation existant
8. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 2: `dynamic-allocation`

La deuxième tâche consiste à rendre plus flexible la structure de données
`struct Scene` à l'aide de l'allocation dynamique. Pour cela, vous devez
utiliser des tableaux dynamiques (similaires à la structure `struct Array` vue
en classe), qui sont redimensionnés chaque fois que la capacité maximale du
tableau est atteinte.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `dynamic-allocation`:

1. Ajuster la déclaration du type `struct Scene` pour permettre l'allocation
   dynamique
2. Supprimer les constantes `NUM_MAX_CONSTRUCTIONS` et `NUM_MAX_ANTENNAS` qui
   ne seront plus nécessaires
3. Rendre l'ajout de constructions et d'antennes fonctionnel avec l'allocation
   dynamique
4. Ajouter des tests fonctionnels qui démontrent qu'il n'y a pas de mauvaise
   gestion de la mémoire, peu importe la sous-commande invoquée
5. Au besoin, mettre à jour la documentation pour refléter les changements
   apportés
6. Vous assurer de respecter le style de programmation existant
7. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 3: `fix-overlap`

La troisième tâche consiste à régler un bogue qui existe dans l'application
depuis un certain temps. En effet, si vous prenez la scène suivante, le
programme ne détectera pas qu'il y a un chevauchement entre les deux
*buildings*:

```sh
begin scene
  building b1 0 0 1 1
  building b2 0 1 3 2
end scene
```

On vous demande donc de régler ce bogue. Plus spécifiquement, vous devez
minimalement apporter les modifications suivantes, en plaçant tous vos
*commits* sur la branche `fix-overlap`:

1. Créer une *issue* sur votre dépôt privé avec GitLab qui documente le bogue,
   avec un titre significatif, une description précise et un exemple qui met en
   évidence le bogue, le tout en respectant le format Markdown
2. Corriger l'erreur pour que les chevauchements soient correctement détectés
3. Mettre à jour les tests fonctionnels (tests Bats) qui démontrent que le
   bogue a été réglé
4. Au besoin, mettre à jour toute documentation pertinente
5. Vous assurer de respecter le style de programmation existant
6. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 4: `draw-scene`

La quatrième et dernière tâche consiste à ajouter une sous-commande `draw` qui
dessine une scène en produisant un fichier au format SVG, à l'aide de la
bibliothèque Cairo. Ainsi, on s'attend à ce que la commande suivante produise
un fichier `scene.svg` dans le répertoire courant, qui contient une
représentation de la scène similaire à celle disponible dans le fichier
`doc/scene.svg`:

```sh
$ bin/kover draw scene.svg < examples/2b1h2a.scene
```

L'image produite doit contenir les éléments suivants:

* Un cadre rectangulaire qui englobe tous les éléments de la scène
* Des graduations (des nombres) sur chacun des 4 côtés du cadre qui facilitent
  l'identification des coordonnées des points de la scène
* Pour chaque construction, un rectangle de couleur bleue (pour les
  *buildings*) ou mauve (pour les maisons) avec les bonnes dimensions, ainsi
  qu'une étiquette portant l'identifiant de la construction placé au centre
* Pour chaque antenne, un disque de couleur verte avec la bonne dimension,
  ainsi qu'une étiquette au centre qui identifie l'antenne (vous pouvez
  optionnellement dessiner un point au centre, mais ce n'est pas obligatoire)

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `draw-scene`:

1. Introduire la sous-commande `draw` à l'application `kover`
2. Implémenter la sous-commande `draw` pour qu'elle produise un fichier au
   format SVG représentant une scène quelconque, ayant le contenu décrit
   ci-haut
3. Ajuster le Makefile pour qu'il soit en mesure de compiler et de faire
   l'édition des liens avec la bibliothèque Cairo
4. Ajuster le fichier `.gitlab-ci.yml` pour que la bibliothèque Cairo soit
   installée avant les étapes `build` et `test` du pipeline existant
5. Ajouter des tests fonctionnels (Bats) reflétant les changements apportés
   (noter que vous n'avez pas à tester la validité de l'image produite,
   seulement vous assurer que la commande fonctionne)
6. Au besoin, mettre à jour toute documentation pertinente
7. Vous assurer de respecter le style de programmation existant
8. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Clone et création du dépôt

Comme pour le travail pratique 1, vous devez cloner le dépôt fourni et
l'héberger sur la plateforme [Gitlab du
département](https://gitlab.info.uqam.ca/). Votre dépôt devra se nommer
**exactement** `inf3135-251-tp3` et l'URL devra être **exactement**
`https://gitlab.info.uqam.ca/<utilisateur>/inf3135-251-tp3`, où `<utilisateur>`
doit être remplacé par votre nom d'utilisateur. Il devra être **privé** et
accessible seulement par vous et par les utilisateurs `blondin_al` et
`gosset.guillaume`.

## Git

Il est important de continuer à utiliser correctement Git pour suivre
l'évolution de votre projet. N'oubliez pas de bien configurer correctement
votre fichier `.gitconfig` qui permet de vous identifier comme auteur de
*commits*, en y indiquant vos **véritables** prénom, nom et courriel. Comme
pour le travail pratique 1 et le travail pratique 2, les messages de *commit*
doivent suivre [la convention de Chris
Beams](https://chris.beams.io/posts/git-commit/), adaptée au français:

1. Séparer le sujet (la première ligne) du corps (le reste du message) par une
   ligne vide
2. Limiter le sujet à 50 caractères
3. Commencer le message par une lettre majuscule
4. Ne pas terminer le sujet avec un point
5. Utiliser un verbe à l'indicatif présent comme premier mot du sujet
6. Limiter les lignes du corps du message à 72 caractères
7. Utiliser le corps pour expliquer *ce que* (*what*) la modification apporte
   et *pourquoi* (*why*) elle apporte cette modification, plutôt que *comment*
   (*how*) elle apporte une modification

Vous pouvez vous inspirer des messages de *commit* déjà existants dans
l'historique. Aussi, n'oubliez pas de mettre les fichiers `.gitignore` du dépôt
si nécessaire en fonction de votre environnement de développement. Assurez-vous
de ne pas versionner de fichiers inutiles (les fichiers binaires, entre autres,
mais pas seulement).

**Attention!**. Comme pour le deuxième travail pratique, vous ne devez pas
modifier la branche `master`. Plutôt, vous devez remettre quatre branches
nommées `refactor-scene`, `fix-overlap`, `dynamic-allocation` et `draw-scene`,
sans les fusionner sur la branche `master`.

## Langue

Comme pour les deux autres travaux pratiques, le projet est développé dans deux
langues, soit le français et l'anglais. On rappelle les règles suivantes:

* Tous les messages de *commit* doivent être rédigés en français
* Le fichier `README.md` doit être rédigé en français
* Les commentaires et la documentation du code source (fichier `.c`, fichier
  `.bats`, etc.) doivent être rédigés en anglais

Cette convention n'est pas universelle, mais elle est souvent employée dans les
milieux bilingues.

## Barème

Les points sont répartis selon la grille suivante:

| Élément             |  Points |
|:--------------------|--------:|
| Tâche 1             |     /30 |
| Tâche 2             |     /30 |
| Tâche 3             |     /30 |
| Tâche 4             |     /30 |
| Utilisation de Git  |     /20 |
| Total               |    /140 |

Plus précisément, les éléments suivants seront pris en compte:

- **Tâche 1 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Le comportement de
  l'application est le même qu'avant les modifications. Les modules `antenna`
  et `construction` sont introduits et contiennent les déclarations et les
  définitions pertinentes. Les fichiers Makefile sont ajustés en conséquence
  pour que la compilation des nouveaux modules soit adéquate. Les *docstrings*
  du code source ont été mises à jour s'il y a lieu. Le style de programmation
  est respecté.

- **Tâche 2 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Le comportement de
  l'application est le même qu'avant les modifications. Le module `scene`
  repose maintenant sur l'allocation dynamique et redimensionne une scène au
  besoin. Les constantes non utilisées ont été retirées. Des tests fonctionnels
  ont été ajoutés pour montrer que la mémoire est gérée correctement. Les
  *docstrings* du code source ont été mises à jour. Le style de programmation
  est respecté.

- **Tâche 3 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Le bogue a été résolu. Une
  *issue* a été créée sur le dépôt privé afin de documenter le bogue. Cette
  *issue* porte un titre significatif, une description complète, ainsi qu'un
  exemple illustrant le bogue. Des tests fonctionnels ont été ajoutés pour
  montrer que le bogue a été réglé. Les *docstrings* du code source ont été
  mises à jour au besoin. Le style de programmation est respecté.

- **Tâche 4 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. La sous-commande `draw`
  a été introduite. Elle permet de produire une image au format SVG qui
  contient les différents éléments de la scène. Les fichiers Makefile ont été
  mis à jour pour faire l'édition des liens avec la bibliothèque Cairo. Le
  fichier `.gitlab-ci.yml` a été mis à jour pour supporter la bibliothèque
  Cairo. Des tests fonctionnels ont été ajoutés pour montrer que la nouvelle
  sous-commande est fonctionnelle. L'aide (`kover help`) a été mis à jour pour
  refléter la nouvelle commande. Les *docstrings* du code source ont été mises
  à jour. Le fichier `README.md` a été mis à jour pour refléter la nouvelle
  commande. Le style de programmation est respecté.

- **Utilisation de Git (20 points)**: Les modifications sont réparties en
  *commits* atomiques. Le fichier `.gitignore` est complet. Les messages de
  *commit* sont significatifs, uniformes et suivent la convention suggérée.
  Les *commits* sont répartis sur des branches correctement nommées. Chaque
  *commit* est pertinent pour la branche sur laquelle il se trouve. Les
  branches sont organisées de façon logique. Noter que si vous n'avez pas
  beaucoup avancé dans certaines des tâches demandées, vous ne pourrez pas
  obtenir beaucoup de points sur ce critère.

## Pénalités

Si votre programme ne compile pas pour une tâche donnée, une pénalité pouvant
aller jusqu'à **100%** pourra être appliquée pour la tâche en question.

**Note**: vous n'avez pas besoin d'insérer votre code permanent dans le fichier
`README.md`: nous avons établi l'association entre votre nom d'utilisateur et
votre code permanent dans le premier travail pratique.

En outre, si vous ne respectez pas les critères suivants, une pénalité pouvant
aller jusqu'à **50%** pourra être appliquée:

- Votre dépôt doit se nommer **exactement** `inf3135-251-tp3`
- L'URL de votre dépôt doit être **exactement**
  `https://gitlab.info.uqam.ca/<utilisateur>/inf3135-251-tp3` où
  `<utilisateur>` doit être remplacé par votre identifiant
- Les utilisateurs `blondin_al` et `gosset.guillaume` doivent avoir accès
  à votre projet en mode *Maintainer*.
- Votre dépôt doit être un *fork* du [gabarit
  fourni](https://gitlab.info.uqam.ca/inf31351/20251/inf3135-251-tp3).
- Votre dépôt doit être **privé**.

## Remise

La remise se fait automatiquement en ajoutant les utilisateurs `blondin_al` et
`gosset.guillaume` en mode *Maintainer*, vous n'avez rien de plus à faire.
À moins d'indication contraire de votre part, ce sera le dernier (*commit*)
disponible sur chacune de vos branches qui sera considéré pour la correction.

Le travail doit être remis au plus tard le **dimanche 27 avril à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.
