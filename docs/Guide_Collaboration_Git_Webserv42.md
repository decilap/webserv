# Guide de collaboration Git — Projet Webserv (42)

## 🎯 Objectif
Ce document décrit **comment travailler à 3 sur le projet Webserv (42)** de manière efficace, sécurisée et alignée avec les sprints définis dans le backlog.

Basé sur :  
- **Méthodologie agile (sprint + backlog produit)**  
- **Flux Git standard (GitFlow simplifié)**  
- **Règles 42 (propreté, non‑crash, anti‑leaks)**  

_Date de création : 2025-10-28_

---

## 👥 1. Rôles et responsabilités

| Rôle | Nom | Responsabilités principales |
|------|------|------------------------------|
| **Chef de sprint** | Dev A | Planifie, fusionne les PRs, maintient la stabilité de `main` |
| **Développeur réseau/serveur** | Dev B | Sockets, poll(), gestion clients, perf |
| **Développeur HTTP/CGI** | Dev C | Parsing, réponses, CGI, gestion des routes |

> 🔁 Les rôles peuvent tourner à chaque sprint pour équilibrer les compétences.

---

## 🧱 2. Organisation du dépôt Git

### 🌳 Arborescence standard
```
webserv/
├── src/
├── include/
├── conf/
├── www/
├── tests/
├── docs/
│   ├── cahier_des_charges.md
│   ├── plan_sprints.md
│   ├── guide_git.md
│   └── backlog.csv
├── Makefile
└── README.md
```

### 🌿 Branches principales

| Branche | Description | Protection |
|----------|--------------|-------------|
| `main` | Version stable, testée et propre | ✅ protégée (merge via PR uniquement) |
| `develop` | Version en cours de sprint | ⚠️ merge autorisé via review |
| `feat/<nom>` | Nouvelle fonctionnalité | créée depuis `develop` |
| `fix/<nom>` | Correction d’un bug | créée depuis `develop` |
| `test/<nom>` | Scripts et stress tests | temporaire, merge si validé |

---

## 🌀 3. Workflow Git (GitFlow simplifié)

### 🧩 Création de branche
```bash
git checkout develop
git pull origin develop
git checkout -b feat/http_parser
```

### 💻 Travail local
- commits fréquents et atomiques :  
  `git commit -m "feat(http): ajout parsing headers"`  
- rebase avant push pour garder un historique propre :  
  `git pull --rebase origin develop`

### 🚀 Push et PR
```bash
git push origin feat/http_parser
```
- Créer une **Pull Request** vers `develop`
- Ajouter une **review obligatoire** d’un pair  
- Utiliser la **checklist anti-leaks / valgrind clean** avant merge

### 🔁 Fusion vers `main`
- À la **fin du sprint**, le Chef de sprint fusionne `develop → main`
- Taguer la version :  
  ```bash
  git tag -a v1.0-sprint2 -m "Fin sprint 2 — HTTP & Statique"
  git push origin --tags
  ```

---

## 📆 4. Sprints et gestion du backlog

### Outils
- **Trello / Notion** : backlog importé depuis `Webserv42_Backlog.csv`
- **GitHub Projects (ou Gitea Projects)** : kanban de suivi  
  - Colonnes : `Backlog`, `Sprint en cours`, `Review`, `Terminé`

### Plan d’exécution (résumé)
| Sprint | Objectif principal | Branche principale | Responsable |
|--------|--------------------|--------------------|--------------|
| 0 | Setup, Makefile, conf, base du projet | `develop` | Dev A |
| 1 | Réseau & poll() | `feat/network` | Dev B |
| 2 | HTTP statique & erreurs | `feat/http` | Dev C |
| 3 | Upload & autoindex | `feat/upload` | Dev A |
| 4 | CGI & sécurité | `feat/cgi` | Dev C |
| 5 | Optimisation, éval, release | `release/v1.0` | Tous |

---

## 🔍 5. Règles de qualité et discipline

### ✅ Avant merge
- [ ] Compilation `make re` sans warnings  
- [ ] `valgrind --leak-check=full` → 0 leaks  
- [ ] Aucun FD non fermé (`lsof` clean)  
- [ ] Respect C++98 (`-std=c++98`)  
- [ ] Tests unitaires et d’intégration passent  
- [ ] Documentation à jour (`README`, `conf`, `docs/`)  

### 🔥 Anti-conflits Git
- Rebase avant tout push (`git pull --rebase origin develop`)  
- Pas de merge direct dans `main`  
- Un seul merge par PR  
- Reviewer ≠ auteur (1 review croisée minimum)

### 🧠 Bonnes pratiques
- **Petites PRs** (< 300 lignes si possible)  
- **Commit clair** : type(scope): message  
  - ex: `fix(cgi): timeout kill process`  
- **Pas de code mort / debug print** dans `main`  
- **Logger propre** : `Logger::info()`, `Logger::error()`  

---

## 🧪 6. Intégration continue (optionnelle mais conseillée)
### Exemple de script local `.github/workflows/test.yml`
```yaml
name: CI Build & Valgrind
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Build
        run: make re
      - name: Test valgrind
        run: |
          sudo apt install -y valgrind
          valgrind --leak-check=full ./webserv conf/default.conf &
          sleep 5
          pkill webserv
```

---

## 🧾 7. Exemple de journée de sprint

| Heure | Activité | Responsable |
|-------|-----------|-------------|
| 09:00 | Daily meeting (15 min) | Tous |
| 09:30 | Dev des stories du sprint | A, B, C |
| 13:00 | Pause / revue rapide | A, B, C |
| 15:00 | Push + PR + review croisée | Tous |
| 17:00 | Tests / Valgrind / Debug | Tous |
| 18:00 | Sprint log (update Trello) | Chef de sprint |

---

## 📦 8. Livraison et évaluation
Avant la soutenance :  
- `main` = build 100% propre, tests passés, doc complète  
- Livrables :  
  - `webserv` exécutable  
  - `conf/*.conf` fonctionnels  
  - `www/` site test + CGI  
  - `docs/` (cahier + plan + backlog + guide Git)  

Tag final :  
```bash
git tag -a v1.0-final -m "Livraison finale Webserv 42"
git push origin --tags
```

---

## 🧩 9. Annexes

### Checklist de sprint
- [ ] Stories du sprint terminées
- [ ] Merge propre vers `develop`
- [ ] Tests unitaires et manuels faits
- [ ] Zéro fuite mémoire/FD
- [ ] Doc mise à jour
- [ ] Release taggée

---

> 💡 **Astuce 42 :** gardez vos PRs courtes, vos explications claires, et vos valgrinds toujours verts. Rien ne fait plus plaisir à un évaluateur qu’un projet propre et compréhensible.
