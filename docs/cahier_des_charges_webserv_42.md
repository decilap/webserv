# Cahier des charges — Projet Webserv (42)

## 1. Objectif du projet
Créer un serveur HTTP/1.1 complet, conforme aux spécifications du sujet 42, codé en **C++98**, sécurisé contre les fuites mémoire, crashs, débordements et comportements indéterminés. Le serveur doit être **non-bloquant**, **multi-connexion** et **configurable** via un fichier de configuration de type NGINX.

---

## 2. Contraintes techniques

### 2.1 Langage et standard
- Langage : **C++98** (aucune dépendance C++11 ou ultérieure)
- Compilateur : `c++` avec les flags `-Wall -Wextra -Werror -std=c++98`

### 2.2 Interdictions
- Bibliothèques externes interdites (y compris Boost)
- Appels système interdits : tout ce qui n’est pas listé dans le sujet officiel
- Serveurs tiers ou wrappers réseau interdits (`execve` d’un autre serveur interdit)

### 2.3 Fonctionnalités système autorisées
- Gestion des E/S : `poll()`, `select()`, `epoll()`, ou `kqueue()`
- Gestion des sockets : `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`
- Gestion de fichiers : `open`, `read`, `write`, `close`, `stat`, `opendir`, `readdir`, `closedir`
- Processus : `fork`, `execve`, `pipe`, `dup2`, `waitpid`
- Signaux : `signal`, `kill`
- Utilitaires : `fcntl`, `getaddrinfo`, `setsockopt`, `errno`

---

## 3. Architecture logicielle

### 3.1 Structure du projet
```
webserv/
│
├── src/
│   ├── main.cpp
│   ├── server/
│   │   ├── Server.hpp / Server.cpp
│   │   ├── Client.hpp / Client.cpp
│   │   ├── PollManager.hpp / PollManager.cpp
│   │   ├── RequestHandler.hpp / RequestHandler.cpp
│   │   ├── ResponseBuilder.hpp / ResponseBuilder.cpp
│   │   └── CgiExecutor.hpp / CgiExecutor.cpp
│   │
│   ├── config/
│   │   ├── ConfigParser.hpp / ConfigParser.cpp
│   │   ├── Config.hpp / Config.cpp
│   │   └── Location.hpp / Location.cpp
│   │
│   ├── utils/
│   │   ├── Logger.hpp / Logger.cpp
│   │   ├── ErrorPage.hpp / ErrorPage.cpp
│   │   └── FileUtils.hpp / FileUtils.cpp
│
├── include/
│   └── (headers partagés)
│
├── conf/
│   ├── default.conf
│   ├── test_upload.conf
│   └── php_cgi.conf
│
├── www/
│   ├── index.html
│   ├── upload/
│   └── cgi-bin/
│
├── Makefile
└── README.md
```

### 3.2 Composants principaux
- **Server** : gère les sockets d’écoute, la boucle d’événements et la gestion des clients.
- **Client** : représente une connexion active avec son état et ses buffers.
- **PollManager** : abstraction de `poll`/`epoll` pour gérer les FDs non bloquants.
- **RequestHandler** : analyse et valide la requête HTTP entrante.
- **ResponseBuilder** : construit la réponse HTTP selon les codes d’état et les fichiers.
- **CgiExecutor** : exécute un script CGI avec environnement sécurisé.
- **ConfigParser** : lit et valide le fichier de configuration.
- **Logger** : journalisation des événements serveur/erreurs.

---

## 4. Spécifications fonctionnelles

### 4.1 Méthodes HTTP supportées
- `GET` : récupération de ressource.
- `POST` : envoi de données (formulaires, fichiers, CGI).
- `DELETE` : suppression de ressource autorisée.

### 4.2 Gestion des routes et configuration
Chaque bloc `server` et `location` permet de définir :
- Port et IP d’écoute.
- Dossier racine (`root`).
- Fichier index (`index`).
- Liste de méthodes acceptées (`allowed_methods`).
- Redirections (`return 301` ou `return 302`).
- Taille maximale du corps (`client_max_body_size`).
- Page d’erreur personnalisée (`error_page`).
- Activation du listing de répertoire (`autoindex on/off`).
- Upload autorisé et dossier cible (`upload_path`).
- Activation de CGI selon extension (`cgi .php /usr/bin/php-cgi`).

### 4.3 Fonctionnalités essentielles
- Support complet de **HTTP/1.1** (persistance, en-têtes, chunked, etc.)
- Gestion fine des **codes d’erreur** (200, 201, 204, 301, 400, 403, 404, 405, 413, 500, 502, 505...)
- Upload via formulaire multipart.
- Exécution sécurisée de CGI :
  - Transmission via `stdin`/`stdout`.
  - Variables d’environnement correctes.
  - Timeout configurable.
  - Nettoyage des FDs à la fin du processus.

---

## 5. Contraintes de sécurité et robustesse

### 5.1 Mémoire et ressources
- **Aucune fuite mémoire** (valgrind doit être 100% clean).
- **Pas de FD leaks** (chaque socket et pipe est fermé proprement).
- **ULIMIT SAFE** : tests avec `ulimit -n 256` doivent fonctionner sans crash.
- Protection contre les **fork bombs** et **timeout CGI** (kill après N secondes).

### 5.2 Résilience
- Serveur non bloquant (pas de `read()`/`write()` sans disponibilité FD).
- Reconnexion automatique des clients si déconnexion propre.
- Logs détaillés sur chaque événement réseau.

---

## 6. Tests et validation

### 6.1 Tests unitaires
- Parser de configuration : vérification syntaxe et héritage des blocs.
- Parser HTTP : validation des en-têtes, chunked, et limites de taille.
- Gestion CGI : exécution testée sur scripts simples (PHP, Python).

### 6.2 Tests d’intégration
- Navigation depuis un navigateur (`localhost:8080`).
- Tests `curl` :
  ```bash
  curl -v -X GET http://localhost:8080/
  curl -v -X POST -F file=@test.txt http://localhost:8080/upload
  curl -v -X DELETE http://localhost:8080/file.txt
  ```
- Comparaison avec NGINX pour comportements similaires.

### 6.3 Stress tests
- Test simultané de 100+ connexions (`ab`, `wrk`, `siege`).
- Simulation de déconnexions brutales (kill -9 client).
- Monitoring valgrind + lsof pour détection de leaks.

---

## 7. Documentation et livrables
- **README.md** : instructions de build et exécution.
- **Cahier de configuration** : exemples `.conf` commentés.
- **Fichiers de tests** : scripts et jeux de tests.
- **Rapport de sécurité** : documentation sur les protections anti-leak et comportements limites.

---

## 8. Évolutions et bonus (facultatif)
- Support des cookies et sessions.
- Multiples CGI (php, python, perl).
- HTTP/1.1 keep-alive.
- Compression gzip.
- Serveur virtuel (virtual hosts).

---

## 9. Objectif d’évaluation
Ce projet doit être **solide**, **documenté**, **sécurisé** et **compris** à 100% par le candidat. Aucune fonctionnalité ne doit dépendre d’un comportement indéfini. En évaluation :
- Aucun crash ou comportement imprévu sous stress.
- Explications claires du fonctionnement réseau et de la boucle `poll()`.
- Justification complète de la gestion mémoire et du parsing HTTP.

---
**Livraison attendue :** Projet compilable avec `make`, exécutable via :
```
./webserv conf/default.conf
```
**Critère de réussite :** le serveur passe tous les tests du sujet sans fuite mémoire, sans crash et avec conformité HTTP.

