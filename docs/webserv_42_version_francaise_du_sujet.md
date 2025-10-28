# Webserv
_C’est quand vous comprenez enfin pourquoi les URL commencent par HTTP_

**Résumé :**
Ce projet consiste à écrire votre propre serveur HTTP.
Vous pourrez le tester avec un vrai navigateur. HTTP est l’un des protocoles les plus utilisés sur Internet. Comprendre ses subtilités vous sera utile, même si vous ne travaillez pas sur un site web.

**Version :** 23.1

---

## Sommaire
I Introduction  
II Règles générales  
III Instructions IA  
IV Partie obligatoire  
&nbsp;&nbsp;IV.1 Exigences  
&nbsp;&nbsp;IV.2 Uniquement pour macOS  
&nbsp;&nbsp;IV.3 Fichier de configuration  
V Partie bonus  
VI Rendu et évaluation par les pairs

---

# Chapitre I — Introduction
Le **Hypertext Transfer Protocol (HTTP)** est un protocole d’application pour des systèmes d’information hypermédia distribués et collaboratifs.

HTTP est la base des communications de données du **World Wide Web**, où les documents hypertexte incluent des hyperliens vers d’autres ressources auxquelles l’utilisateur peut facilement accéder (par exemple en cliquant dans un navigateur).

HTTP a été développé pour supporter l’hypertexte et accompagner la croissance du Web.

La fonction principale d’un **serveur web** est de **stocker, traiter et livrer** des pages web aux clients. La communication client‑serveur a lieu via HTTP. Les pages servies sont le plus souvent des documents HTML, qui peuvent inclure des images, feuilles de style et scripts en plus du contenu texte.

Pour un site à fort trafic, plusieurs serveurs web peuvent être utilisés, répartissant la charge entre plusieurs machines physiques.

Un **agent utilisateur** (généralement un navigateur ou un robot d’indexation) initie la communication en demandant une ressource précise via HTTP, et le serveur répond avec le contenu de cette ressource ou un message d’erreur s’il ne peut pas le faire. La ressource est typiquement un fichier réel sur le stockage du serveur, ou le résultat d’un programme. Mais ce n’est pas toujours le cas : cela peut être bien d’autres choses.

Bien que sa fonction principale soit de servir du contenu, HTTP permet aussi aux clients d’**envoyer des données**. Cette fonctionnalité est utilisée pour la soumission de formulaires web, y compris **l’upload de fichiers**.

---

# Chapitre II — Règles générales
- Votre programme **ne doit jamais crasher** (même en cas d’épuisement mémoire) ni se terminer de manière inattendue. Si cela arrive, votre projet sera considéré **non fonctionnel** et votre note sera **0**.
- Vous devez fournir un **Makefile** qui compile vos sources. Il ne doit pas relinker inutilement.
- Votre Makefile doit au minimum contenir les règles : `$(NAME)`, `all`, `clean`, `fclean` et `re`.
- Compilez votre code avec **c++** et les flags `-Wall -Wextra -Werror`.
- Votre code doit respecter le standard **C++98** et doit toujours compiler avec l’option `-std=c++98`.
- Exploitez autant que possible les fonctionnalités C++ (par ex. préférez `<cstring>` à `<string.h>`). Les fonctions C sont autorisées, mais **préférez** leurs versions C++ si possible.
- **Toute bibliothèque externe** et **Boost** sont **interdites**.

---

# Chapitre III — Instructions IA
**Contexte**  
Pendant votre apprentissage, l’IA peut vous aider pour de nombreuses tâches. Prenez le temps d’explorer les capacités des outils d’IA et comment ils peuvent soutenir votre travail. Cependant, utilisez‑les toujours avec prudence et évaluez de manière critique les résultats. Qu’il s’agisse de code, documentation, idées ou explications techniques, vous ne pouvez jamais être totalement sûr que votre question était bien formulée ni que le contenu généré est exact. Vos pairs sont une ressource précieuse pour éviter erreurs et angles morts.

**Message principal**
- ☛ Utilisez l’IA pour **réduire les tâches répétitives** ou fastidieuses.
- ☛ Développez des **compétences de prompting** (code et hors code) utiles à votre carrière.
- ☛ Comprenez **comment fonctionnent** les systèmes d’IA pour mieux anticiper et éviter risques, biais et enjeux éthiques.
- ☛ Continuez à développer vos compétences techniques et transversales **avec vos pairs**.
- ☛ N’utilisez que du contenu généré par l’IA que vous **comprenez pleinement** et dont vous pouvez **assumer la responsabilité**.

**Règles pour l’apprenant**
- Prenez le temps d’explorer les outils d’IA et de comprendre leur fonctionnement afin de les utiliser **de manière éthique** et **réduire les biais** potentiels.
- **Réfléchissez** à votre problème **avant** de rédiger une requête : cela aide à écrire des prompts plus clairs, détaillés et pertinents avec un vocabulaire précis.
- Prenez l’habitude de **vérifier systématiquement** toute production d’IA : relisez, questionnez, testez.
- Cherchez **toujours une relecture par un pair** — ne comptez pas uniquement sur votre propre validation.

**Résultats attendus de la phase**
- Développer des compétences de prompting **générales** et **spécifiques au domaine**.
- Augmenter votre **productivité** grâce à une utilisation efficace des outils d’IA.
- Poursuivre le renforcement de la **pensée computationnelle**, de la **résolution de problèmes**, de l’**adaptabilité** et de la **collaboration**.

**Commentaires et exemples**
- Vous rencontrerez régulièrement des situations (examens, évaluations, etc.) où vous devrez démontrer une **compréhension réelle**. Préparez‑vous et continuez à développer compétences techniques et interpersonnelles.
- **Expliquer votre raisonnement** et **débattre** avec des pairs révèle souvent des lacunes. Faites de l’apprentissage par les pairs une priorité.
- Les outils d’IA manquent souvent de **votre contexte spécifique** et fournissent des réponses génériques. Vos pairs, qui partagent votre environnement, peuvent offrir des **insights plus pertinents et précis**.
- Là où l’IA tend à générer la réponse la plus probable, vos pairs peuvent fournir **d’autres perspectives** et une **nuance** précieuse. Appuyez‑vous sur eux comme **point de contrôle qualité**.

**Bonnes pratiques**
- ✓ Je demande à l’IA : « Comment tester une fonction de tri ? ». Elle me donne quelques idées. Je les essaie et je **revois les résultats avec un pair**. Nous raffinons l’approche ensemble.
- ✓ J’utilise l’IA pour m’aider à **concevoir un parseur**. Ensuite, je passe la logique en revue avec un pair. Nous trouvons deux bugs et **réécrivons** ensemble — plus **propre** et **compris**.

**Mauvaises pratiques**
- ✗ Je demande à l’IA d’écrire une fonction entière et je la **copie-colle**. En évaluation, je ne peux pas expliquer ce qu’elle fait ni pourquoi. Je **perds en crédibilité** — et je **rate** mon projet.
- ✗ Je laisse un outil générer mon code pour une partie clé. Ça compile, mais je ne peux pas expliquer la gestion des pipes. En évaluation, **je n’arrive pas à justifier** et **j’échoue**.

---

# Chapitre IV — Partie obligatoire
**Nom du programme** : `webserv`  
**Fichiers à rendre** : `Makefile`, `*.{h,hpp}`, `*.cpp`, `*.tpp`, `*.ipp`, fichiers de **configuration**  
**Règles Makefile** : `$(NAME)`, `all`, `clean`, `fclean`, `re`  
**Arguments** : `[Un fichier de configuration]`  
**Fonctions externes autorisées** : Toute la fonctionnalité doit être implémentée en **C++98**.

`execve`, `pipe`, `strerror`, `gai_strerror`, `errno`, `dup`, `dup2`, `fork`, `socketpair`, `htons`, `htonl`, `ntohs`, `ntohl`, `select`, `poll`, `epoll` (`epoll_create`, `epoll_ctl`, `epoll_wait`), `kqueue` (`kqueue`, `kevent`), `socket`, `accept`, `listen`, `send`, `recv`, `chdir`, `bind`, `connect`, `getaddrinfo`, `freeaddrinfo`, `setsockopt`, `getsockname`, `getprotobyname`, `fcntl`, `close`, `read`, `write`, `waitpid`, `kill`, `signal`, `access`, `stat`, `open`, `opendir`, `readdir`, `closedir`.

**Libft autorisée** : n/a  
**Description** : Un **serveur HTTP** en **C++98**.

Vous devez écrire un serveur HTTP en C++98.

Votre exécutable doit être lancé comme suit :

```
./webserv [fichier_de_configuration]
```

Même si `poll()` est mentionné dans le sujet et la fiche d’évaluation, vous pouvez utiliser **toute fonction équivalente** telle que `select()`, `kqueue()` ou `epoll()`.

Veuillez lire les **RFC** qui définissent HTTP, et effectuer des tests avec **telnet** et **NGINX** avant de commencer. Bien que vous ne soyez pas tenus d’implémenter l’intégralité des RFC, leur lecture vous aidera à développer les fonctionnalités requises. **HTTP/1.0** est suggéré comme point de référence, mais **non imposé**.

## IV.1 — Exigences
- Votre programme doit utiliser un **fichier de configuration**, fourni en argument en ligne de commande ou disponible dans un **chemin par défaut**.
- Vous **ne pouvez pas** `execve` un autre serveur web.
- Votre serveur doit rester **non bloquant** en toutes circonstances et gérer correctement les **déconnexions clients** quand nécessaire.
- Il doit être **non bloquant** et utiliser **un seul** `poll()` (ou équivalent) pour **toutes** les opérations d’E/S entre clients et serveur (**écoute incluse**).
- `poll()` (ou équivalent) doit **surveiller simultanément** lecture **et** écriture.
- Vous ne devez **jamais** faire de `read`/`write` (ou `recv`/`send`) **sans passer par** `poll()` (ou équivalent) **au préalable**.
- **Interdit** : vérifier `errno` pour ajuster le comportement **après** un `read` ou `write`.
- Vous **n’êtes pas tenus** d’utiliser `poll()` pour les **fichiers disque réguliers** ; `read()`/`write()` dessus ne nécessitent pas de notifications de disponibilité.

> Les E/S susceptibles d’attendre des données (sockets, pipes/FIFO, etc.) doivent être **non bloquantes** et pilotées par **un unique** `poll()` (ou équivalent). Appeler `read/recv` ou `write/send` **sans** disponibilité préalable conduira à une **note de 0**. Les fichiers réguliers sont **exemptés**.

- Avec `poll()` (ou équivalent), vous pouvez utiliser toutes les macros/fonctions associées (p. ex. `FD_SET` pour `select()`).
- Une **requête** vers votre serveur ne doit **jamais** rester bloquée indéfiniment.
- Votre serveur doit être compatible avec des **navigateurs web standard** de votre choix.
- **NGINX** peut être utilisé pour comparer **en‑têtes** et **comportements** (attention aux différences entre versions HTTP).
- Vos **codes d’état HTTP** doivent être **précis**.
- Votre serveur doit avoir des **pages d’erreur par défaut** si aucune n’est fournie.
- Vous **ne pouvez utiliser `fork`** que pour les **CGI** (PHP, Python, etc.).
- Vous devez être capable de servir un **site web entièrement statique**.
- Les clients doivent pouvoir **uploader des fichiers**.
- Vous devez supporter **au minimum** les méthodes **GET**, **POST** et **DELETE**.
- **Testez en charge** votre serveur pour garantir sa disponibilité en toutes circonstances.
- Votre serveur doit pouvoir **écouter sur plusieurs ports** pour délivrer des contenus différents (voir **Fichier de configuration**).

> Nous avons délibérément limité le périmètre par rapport aux RFC HTTP. Dans ce contexte, la fonctionnalité **virtual host** est **hors scope**. Mais vous pouvez l’implémenter si vous le souhaitez.

## IV.2 — Uniquement pour macOS
Comme macOS gère `write()` différemment des autres OS de type Unix, vous êtes autorisés à utiliser `fcntl()`. Vous devez mettre les descripteurs **en mode non bloquant** pour obtenir un comportement similaire à celui des autres Unix.

Vous pouvez utiliser `fcntl()` **uniquement** avec les flags suivants : `F_SETFL`, `O_NONBLOCK`, `FD_CLOEXEC`. **Tout autre flag est interdit.**

## IV.3 — Fichier de configuration
Vous pouvez vous inspirer de la section **server** du fichier de configuration **NGINX**.

Dans ce fichier, vous devez pouvoir :
- Définir toutes les paires **interface:port** sur lesquelles votre serveur écoutera (définir plusieurs sites web servis par votre programme).
- Définir des **pages d’erreur par défaut**.
- Définir la **taille maximale** autorisée pour les **corps de requêtes** clients.
- Spécifier des **règles** ou **configurations** sur une **URL/route** (pas besoin de regex) pour un site, parmi :
  - Liste des **méthodes HTTP acceptées** pour la route.
  - **Redirection HTTP**.
  - **Répertoire racine** où se trouve le fichier demandé (ex. si l’URL `/kapouet` est rattachée à `/tmp/www`, l’URL `/kapouet/pouic/toto/pouet` cherchera `/tmp/www/pouic/toto/pouet`).
  - Activation/désactivation de l’**index de répertoire** (directory listing).
  - Fichier **par défaut** à servir quand la ressource demandée est un **répertoire**.
  - Autoriser l’**upload de fichiers** par les clients vers le serveur et **emplacement** de stockage.
  - Exécution de **CGI** en fonction de l’**extension** (par ex. `.php`).
    - Vous vous demandez ce qu’est un **CGI** ?
    - Regardez attentivement les **variables d’environnement** impliquées dans la communication serveur web ↔ CGI. La requête complète et les arguments fournis par le client doivent être disponibles pour le CGI.
    - Pour les requêtes **chunked**, votre serveur doit les **dé‑chunker** ; le CGI s’attend à **EOF** comme fin du corps.
    - Il en va de même pour la **sortie du CGI**. Si aucun `Content-Length` n’est renvoyé, **EOF** marque la fin des données renvoyées.
    - Le CGI doit être exécuté dans le **bon répertoire** pour les chemins relatifs.
    - Votre serveur doit supporter **au moins un** type de CGI (php‑cgi, Python, etc.).

Vous devez fournir des **fichiers de configuration** et des **fichiers par défaut** pour tester et démontrer que **chaque fonctionnalité** marche pendant l’évaluation.

Vous pouvez avoir d’autres règles/informations (ex. **nom de serveur** si vous implémentez des **virtual hosts**).

Si vous avez un doute sur un comportement, comparez avec **NGINX**. Un **petit tester** est fourni. Son utilisation n’est pas obligatoire si tout fonctionne avec votre navigateur et vos tests, mais il peut aider à trouver/corriger des bugs.

> La **résilience** est cruciale. Votre serveur doit rester **opérationnel en permanence**.
>
> Ne testez pas avec un seul programme. Écrivez vos tests dans un langage adapté (**Python**, **Go**, etc.), voire en **C/C++** si vous préférez.

---

# Chapitre V — Partie bonus
Fonctionnalités additionnelles possibles :
- Support des **cookies** et **gestion de session** (fournir des exemples simples).
- Gérer **plusieurs types de CGI**.

> La partie bonus n’est évaluée que si la partie obligatoire est **entièrement** validée **sans problème**. Si vous n’atteignez pas toutes les exigences de la partie obligatoire, la bonus **ne sera pas évaluée**.

---

# Chapitre VI — Rendu et évaluation par les pairs
Soumettez votre projet dans votre **répertoire Git** comme d’habitude. **Seul** le contenu du repo sera évalué. Vérifiez les **noms de fichiers**.

Pendant l’évaluation, une **modification légère** du projet peut être demandée (petit changement de comportement, quelques lignes à écrire/réécrire, ou une fonctionnalité facile à ajouter).

Cette étape n’est pas systématique, mais **vous devez y être préparé** si elle figure dans le barème.

Son but est de vérifier votre **compréhension réelle** d’une partie spécifique. La modification peut être effectuée dans **l’environnement de dev que vous voulez**, et doit être faisable en **quelques minutes** (sauf si un délai est explicitement prévu).

Par exemple, on peut vous demander de **mettre à jour une fonction ou un script**, **modifier un affichage**, ou **ajuster une structure de données** pour stocker une nouvelle info, etc.

Les détails (périmètre, cible, etc.) sont précisés dans la grille d’évaluation et **peuvent varier** d’une évaluation à l’autre.

---

*(Référence utile : https://fr.wikipedia.org/wiki/Common_Gateway_Interface)*

