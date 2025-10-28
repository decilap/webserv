# Plan de sprints détaillé — Projet Webserv (42)

> **Objectif** : livrer un serveur HTTP/1.1 non bloquant en C++98, conforme au sujet 42, robuste (anti-leaks, anti-crash), prêt pour l’évaluation avec jeux de tests, documentation et configuration de démonstration.

**Référence temporelle** (indicative, basée sur la date du jour 2025-10-28 — Europe/Warsaw) :  
- **Sprint 0 (Kickoff & Setup)** : 2025-10-28 → 2025-10-29
- **Sprint 1 (Réseau & Événements)** : 2025-10-30 → 2025-11-03
- **Sprint 2 (HTTP & Statique)** : 2025-11-04 → 2025-11-08
- **Sprint 3 (Upload, Autoindex, Erreurs)** : 2025-11-09 → 2025-11-13
- **Sprint 4 (CGI & Sécurité)** : 2025-11-14 → 2025-11-19
- **Sprint 5 (Perf, Durcissement, Éval)** : 2025-11-20 → 2025-11-24

> Les dates sont adaptables selon votre rythme 42. Les jalons et livrables restent identiques.

---

## 1) Règles d’équipe & flux de travail

- **Branching** : `main` (protégée) ; branches par feature `feat/<module>` ; `bugfix/<id>` ; `chore/<task>`.
- **Commits** (concis, FR/EN) : _type(scope): message_ — ex. `feat(poll): add edge-triggered loop`.
- **Revues** : 1 relecture croisée minimale par PR, checklist anti-leaks incluse.
- **CI locale** : `make`, `make test`, `make valgrind`, `make sanit` (si autorisé pour dev seulement), `make lint`.
- **Definition of Ready (DoR)** : user story comprise, critères d’acceptation écrits, estimation posée, impacts connus.
- **Definition of Done (DoD)** : code + tests passent, pas de fuite (valgrind clean), doc & conf mises à jour, démontrable via `curl`/navigateur.

---

## 2) Backlog (épics → user stories)

### EPIC A — Infrastructure & Boucle d’événements
- A1. En tant qu’**opérateur**, je peux **lancer webserv** avec un fichier `.conf` et voir les sockets d’écoute ouvertes.
- A2. En tant que **serveur**, je gère **N connexions simultanées** via un **seul** `poll()` (ou équivalent).
- A3. En tant que **serveur**, je **n’écris/lis jamais** sans disponibilité préalable du FD.

### EPIC B — HTTP Core
- B1. En tant que **client**, je reçois une **réponse 200** pour un fichier statique existant.
- B2. En tant que **client**, je reçois les **codes d’erreur** adaptés (400, 403, 404, 405, 413, 500…).
- B3. En tant que **client**, je peux utiliser **GET/POST/DELETE** selon les règles de la location.
- B4. En tant que **client**, je peux envoyer une requête **chunked** et recevoir une réponse correcte.

### EPIC C — Configuration & Routage
- C1. En tant qu’**admin**, je définis **plusieurs ports/IP** et **locations** (root, index, autoindex, redir, méthodes, upload, cgi).
- C2. En tant qu’**admin**, je configure des **pages d’erreur** personnalisées.
- C3. En tant qu’**admin**, je limite **client_max_body_size**.

### EPIC D — Upload & Autoindex
- D1. En tant que **utilisateur**, je peux **uploader** un fichier (multipart) vers `upload_path`.
- D2. En tant que **visiteur**, je vois un **listing HTML** si `autoindex on` et pas d’index.

### EPIC E — CGI
- E1. En tant que **développeur**, je peux exécuter un **CGI** (.php, .py) avec env correct.
- E2. En tant que **serveur**, je gère **timeout**, **EOF**, `Content-Length` absent, et **nettoyage FDs**.

### EPIC F — Robustesse & Perf
- F1. En tant qu’**évaluateur**, je ne détecte **aucune fuite** (mémoire/FD).
- F2. En tant que **client**, le serveur reste **réactif** sous charge (wrk/ab).
- F3. En tant qu’**évaluateur**, je peux comparer le comportement avec **NGINX** et constater la cohérence.

---

## 3) Estimations (référentiel indicatif)
- XS = 0.5 j ; S = 1 j ; M = 2 j ; L = 3–4 j ; XL = 5+ j  
> Valeurs à affiner selon équipe/rythme

---

## 4) Sprints détaillés (stories, tâches, DoD, livrables)

### 🟦 Sprint 0 — Kickoff & Setup (2025-10-28 → 2025-10-29)
**Objectifs** : Base de repo, outillage, pipeline local, squelettes.

**Stories** : A1 (XS), C1-part (S)

**Tâches** :

- Init repo + Makefile minimal (`-Wall -Wextra -Werror -std=c++98`).

- Arborescence (`src/`, `include/`, `conf/`, `www/`…).

- Logger minimal + gestion erreurs fatales.

- Conf par défaut `conf/default.conf` (listen 127.0.0.1:8080, root www/).

- Scripts utilitaires : `tools/run_valgrind.sh`, `tools/stress.sh`.

**DoD** : build OK, exécutable lance lecture conf (même partielle), logs init.

**Livrable** : repo clonable, README, conf par défaut.

---

### 🟦 Sprint 1 — Réseau & Boucle d’événements (2025-10-30 → 2025-11-03)
**Objectifs** : Sockets non bloquantes, accept, clients, **un** `poll()` central.

**Stories** : A1(S), A2(L), A3(M)

**Tâches** :

- Création sockets d’écoute (multi-port), `fcntl(O_NONBLOCK)`.

- PollManager : registre FD, intentions R/W, timeouts.

- ClientState : buffers RX/TX, états (READ_HEADERS, READ_BODY, WRITE, CLOSE).

- Discipline : jamais de `read/write` hors disponibilité (assert + tests).

**Tests** : `nc`/`telnet`, connexions multiples, timeouts.

**DoD** : échos simples possibles, pas de blocage, pas de fuites FD.

**Livrable** : démonstration accept multi-clients (sans HTTP complet).

---

### 🟦 Sprint 2 — HTTP & Statique (2025-11-04 → 2025-11-08)
**Objectifs** : Parsing requêtes, réponses, fichiers statiques, erreurs.

**Stories** : B1(L), B2(M), C2(S), C3(S)

**Tâches** :

- Parser HTTP/1.1 : start-line, headers, `Connection`, `Host`, validation.

- Gestion body : `Content-Length`, **chunked** (déchunking), limites.

- MIME types, `ResponseBuilder`, pages d’erreur par défaut + configurables.

- Static handler : mapping URL→filesystem (root + sécurité path traversal).

**Tests** : `curl -v` GET/HEAD (optionnel), fichiers lourds, 404/403/405/413.

**DoD** : servir `www/index.html`, erreurs précises, valgrind clean.

**Livrable** : mini site statique complet.

---

### 🟦 Sprint 3 — Upload, Autoindex, Redirections (2025-11-09 → 2025-11-13)
**Objectifs** : multipart upload, autoindex, `return 3xx`, DELETE.

**Stories** : D1(M), D2(S), B3(M), C1-complément(S)

**Tâches** :

- Multipart parser (boundary, fichiers, limites, droits).

- Autoindex HTML (tri, lien parent, taille/date).

- DELETE sécurisé (vérif location/permissions).

- Redirections 301/302 + `Location`.

**Tests** : `curl -F`, upload > 1 Mo, fichiers binaires, DELETE success/403.

**DoD** : upload fonctionnel vers `upload_path`, autoindex propre.

**Livrable** : démo formulaire upload + delete tool page.

---

### 🟦 Sprint 4 — CGI & Sécurité (2025-11-14 → 2025-11-19)
**Objectifs** : exécution d’un CGI (php-cgi ou python), env complet, timeouts, nettoyage.

**Stories** : E1(L), E2(L)

**Tâches** :

- CgiExecutor : fork/execve, pipes, dup2, env (REQUEST_METHOD, SCRIPT_NAME, PATH_INFO, QUERY_STRING, CONTENT_LENGTH, CONTENT_TYPE, SERVER_PROTOCOL, GATEWAY_INTERFACE, SERVER_NAME, SERVER_PORT…).

- Gestion entrée chunked → **déchunk** avant CGI (EOF attendu).

- Sortie CGI : headers + body, `Content-Length` optionnel, EOF.

- Sécurité : chdir au bon répertoire, droits d’exec, timeout (kill).

**Tests** : scripts `cgi-bin/hello.py` et/ou `index.php`, POST variables, temps long.

**DoD** : CGI stable, pas de zombie, pas de fuite FD, erreurs 502 si CGI ko.

**Livrable** : exemples CGI et conf dédiée.

---

### 🟦 Sprint 5 — Performance, Durcissement & Préparation Éval (2025-11-20 → 2025-11-24)
**Objectifs** : perf/stress, robustesse, doc, démo, parity NGINX.

**Stories** : F1(M), F2(M), F3(S)

**Tâches** :

- Bench `wrk`/`ab`/`siege` (RPS, latence, 95p/99p).

- Limites : `ulimit -n`, connexions courtes/longues, Keep-Alive.

- Audit sécurité : traversal, grosse charge, corps invalide, headers malformés.

- Documentation finale + scripts démo + checklists éval.

**Tests** : campagnes reproductibles via `tools/stress.sh`.

**DoD** : tout vert, valgrind 0 leak, démo prête.

**Livrable** : paquet de soutenance (conf, scénarios, pages erreurs, scripts).

---

## 5) Critères d’acceptation (extraits par story)

- **A2** : 200 connexions concurrentes gérées sans blocage, aucune lecture/écriture sans poll prêt (logs debug activables), pas de EAGAIN non traité fatale.

- **B4** : requêtes chunked de tailles variées acceptées ; reconstruire le corps exact ; cas `0\r\n\r\n` traité.

- **C3** : requêtes > `client_max_body_size` retournent **413**.

- **D1** : upload multipart de 0–10 Mo vers `upload_path` ; noms sûrs ; refus si non autorisé.

- **E2** : CGI lent > timeout → **504/502** selon cas ; tous FDs fermés ; pas de zombie.

- **F2** : sous `wrk -c 200 -d 30s` le serveur reste réactif, taux d’erreur < 1% sur statique.

---

## 6) Checklists — Anti-leaks & Discipline I/O

### Mémoire & FDs
- [ ] Valgrind `--leak-check=full --track-fds=yes` : 0 leak, 0 FD perdu.
- [ ] Tous les chemins d’erreur ferment les FDs ouverts (RAII/guards).

- [ ] Pas de `new` sans `delete`/smart wrapper local (C++98 : auto_ptr interdit → wrappers maison).

- [ ] Pas de `throw` non rattrapé dans la boucle serveur.

### Réseau
- [ ] Tous FDs `O_NONBLOCK`.

- [ ] Jamais `read/write` sans poll prêt.

- [ ] Gestion **EINTR/EAGAIN/EWOULDBLOCK** cohérente.

- [ ] Timeout inactifs → fermeture propre.

### HTTP
- [ ] CRLF strict ; headers multi-lignes ; taille max headers.

- [ ] Sécurité chemin (normalisation, pas de `..`).

- [ ] Types MIME corrects.

- [ ] Pages d’erreur par défaut + personnalisées.

### CGI
- [ ] Env complet ; chdir ; PATH_INFO/QUERY_STRING corrects.

- [ ] Déchunk avant CGI.

- [ ] Limitation durée/ressources.

- [ ] Nettoyage processus enfant (waitpid non bloquant).

---

## 7) Jeux de tests (exemples)

```bash
# Statique & erreurs
curl -v http://127.0.0.1:8080/
curl -v http://127.0.0.1:8080/notfound
curl -v -H 'Expect:' --data-binary @big.bin http://127.0.0.1:8080/limited

# Methods
curl -X DELETE -v http://127.0.0.1:8080/files/a.txt

# Upload multipart
curl -v -F 'file=@test.txt' http://127.0.0.1:8080/upload

# Chunked
python tools/send_chunked.py http://127.0.0.1:8080/chunked

# CGI
curl -v 'http://127.0.0.1:8080/cgi-bin/hello.py?name=42'
```

---

## 8) Matrice de risques

| Risque | Prob. | Impact | Stratégie | Mesure |
|---|---:|---:|---|---|
| Leaks mémoire/FD | Haute | Haute | Valgrind en continu | CI locale + hooks |
| Blocage I/O | Moyenne | Haute | Discipline poll | Tests EAGAIN |
| Temps CGI | Moyenne | Moyenne | Timeout + kill | Alarme + logs |
| Traversal path | Basse | Haute | Normalisation | Tests path |
| Surcharge connexions | Moyenne | Moyenne | Limites + backoff | Bench régulier |

---

## 9) Livrables par sprint

- **Code** + **config** + **tests** reproductibles.

- **Journal de sprint** (burndown simplifié, obstacles, décisions).

- **Démonstration** (`curl` + navigateur) + **notes** d’écarts/alignements.

- **Docs** mises à jour (README, cahier des charges, sujets).

---

## 10) Rituels & gabarits

- **Daily (15 min)** : hier/aujourd’hui/risques/besoins.

- **Sprint planning** : sélection stories (capacité), critères d’acceptation.

- **Review** : démo feature → coche DoD.

- **Rétro** : +/−/∆, actions concrètes.

**Template ticket** :  
```
Story: <id> - <titre>
Estimation: S/M/L
Description: ...
Critères d’acceptation:
- [ ] ...
- [ ] ...
Tests:
- ...
Impacts: réseau / http / cgi / conf / doc
Checklist: anti-leaks / fd / poll / http / cgi
```

---

## 11) Notes
- Respect strict des contraintes 42 (C++98, pas de libs externes, un seul poll/select/epoll/kqueue, fork uniquement pour CGI, etc.).
- Comparaison comportementale avec **NGINX** pour valider les choix HTTP.
- Préparer un **paquet de soutenance** (scénarios + conf + scripts + pages d’erreur).
