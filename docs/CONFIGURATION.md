# Configuration Guide

## Table des Matières
- [Vue d'Ensemble](#vue-densemble)
- [Structure du Fichier](#structure-du-fichier)
- [Directives Serveur](#directives-serveur)
- [Directives Location](#directives-location)
- [Exemples de Configuration](#exemples-de-configuration)
- [Best Practices](#best-practices)

---

## Vue d'Ensemble

Webserv utilise un format de configuration inspiré de **Nginx**. Le fichier de configuration définit:

- Serveurs virtuels (virtual hosts)
- Ports d'écoute
- Routes et locations
- Pages d'erreur personnalisées
- Configuration CGI
- Limites de taille de requête

### Fichier par Défaut

**Chemin**: `conf/webserv.conf`

**Usage**:
```bash
./webserv conf/webserv.conf
```

---

## Structure du Fichier

### Format Général

```nginx
server {
    # Directives de configuration serveur
    listen <port>;
    server_name <hostname>;
    root <document_root>;

    # Directives globales
    client_max_body_size <size>;
    errors_page <code> <path>;

    # Blocs location
    location <path> {
        # Directives spécifiques à cette route
    }
}

# Plusieurs serveurs peuvent être définis
server {
    # Configuration du second serveur
}
```

### Syntaxe

- **Commentaires**: `# Ceci est un commentaire`
- **Blocs**: Délimités par `{ }`
- **Directives**: `directive_name value;` (terminées par `;`)
- **Paths**: Relatifs au document root ou absolus

---

## Directives Serveur

### listen

**Syntaxe**: `listen <port>;`

**Description**: Définit le port d'écoute du serveur

**Exemple**:
```nginx
listen 8080;
listen 80;
listen 443;
```

**Notes**:
- Plage valide: 1-65535
- Port < 1024 nécessite les droits root
- Plusieurs serveurs peuvent écouter sur des ports différents

---

### server_name

**Syntaxe**: `server_name <hostname>;`

**Description**: Nom d'hôte du serveur virtuel (pour Host header matching)

**Exemple**:
```nginx
server_name localhost;
server_name www.example.com;
server_name api.mysite.local;
```

**Notes**:
- Utilisé pour le routing multi-serveurs
- Comparé au header `Host:` de la requête HTTP
- Si aucun match, le premier serveur du port est utilisé

---

### root

**Syntaxe**: `root <directory>;`

**Description**: Répertoire racine des fichiers statiques

**Exemple**:
```nginx
root www;
root /var/www/html;
root /home/user/public_html;
```

**Notes**:
- Peut être relatif (au répertoire d'exécution) ou absolu
- Le serveur cherche les fichiers dans `root + uri`
- Exemple: `root www;` + URI `/index.html` → `www/index.html`

---

### client_max_body_size

**Syntaxe**: `client_max_body_size <size>;`

**Description**: Taille maximale du body des requêtes POST/PUT

**Format**:
- Octets: `1000` (1000 bytes)
- Kilobytes: `100K` ou `100k`
- Megabytes: `2M` ou `2m`
- Gigabytes: `1G` ou `1g`

**Exemple**:
```nginx
client_max_body_size 2M;      # 2 mégabytes
client_max_body_size 512K;    # 512 kilobytes
client_max_body_size 1G;      # 1 gigabyte
```

**Notes**:
- Si dépassé, retourne **413 Payload Too Large**
- Valeur par défaut: 1M
- Protège contre les DoS par upload massif

---

### error_pages

**Syntaxe**: `error_pages <code> <path>;`

**Description**: Page HTML personnalisée pour un code d'erreur

**Exemple**:
```nginx
error_pages 404 /errors/404.html;
error_pages 500 /errors/500.html;
error_pages 403 /errors/forbidden.html;
```

**Codes supportés**:
- `400` Bad Request
- `403` Forbidden
- `404` Not Found
- `405` Method Not Allowed
- `413` Payload Too Large
- `500` Internal Server Error
- `504` Gateway Timeout

**Notes**:
- Le path est relatif au `root`
- Si le fichier n'existe pas, une page par défaut est générée
- Permet un branding cohérent des erreurs

---

## Directives Location

### location

**Syntaxe**: `location <path> { ... }`

**Description**: Définit une route spécifique avec sa propre configuration

**Exemple**:
```nginx
location / {
    # Configuration pour toutes les routes
}

location /api {
    # Configuration spécifique à /api/*
}

location /cgi-bin {
    # Configuration pour les scripts CGI
}
```

**Matching**:
- Recherche du préfixe le plus long
- Exemple: requête `/api/users` → match avec `/api` plutôt que `/`

---

### root (dans location)

**Syntaxe**: `root <directory>;`

**Description**: Surcharge le root du serveur pour cette location

**Exemple**:
```nginx
server {
    root www;

    location /cgi-bin {
        root www/cgi-bin;  # Scripts CGI dans un dossier séparé
    }

    location /uploads {
        root /var/uploads;  # Uploads dans un autre filesystem
    }
}
```

---

### allow_methods

**Syntaxe**: `allow_methods <method1> <method2> ...;`

**Description**: Liste des méthodes HTTP autorisées pour cette location

**Exemple**:
```nginx
location / {
    allow_methods GET POST;  # Lecture et écriture
}

location /api {
    allow_methods GET POST DELETE;  # API RESTful
}

location /static {
    allow_methods GET;  # Lecture seule
}
```

**Méthodes disponibles**:
- `GET` - Récupération de ressources
- `POST` - Création/upload de ressources
- `DELETE` - Suppression de ressources

**Notes**:
- Si méthode non listée → **405 Method Not Allowed**
- Header `Allow:` retourné avec les méthodes permises

---

### index

**Syntaxe**: `index <filename>;`

**Description**: Fichier par défaut à servir si l'URI est un répertoire

**Exemple**:
```nginx
location / {
    index index.html;
}

location /admin {
    index admin.html;
}
```

**Comportement**:
- Requête sur `/` → cherche `www/index.html`
- Si le fichier n'existe pas → 404 ou autoindex

---

### autoindex

**Syntaxe**: `autoindex on|off;`

**Description**: Active la génération automatique d'un listing de répertoire

**Exemple**:
```nginx
location /upload {
    autoindex on;  # Liste les fichiers uploadés
}

location /private {
    autoindex off;  # Pas de listing (404 si pas d'index)
}
```

**Format du listing**:
```html
<html>
<head><title>Index of /upload/</title></head>
<body>
<h1>Index of /upload/</h1>
<ul>
<li><a href="../">../</a></li>
<li><a href="file1.txt">file1.txt</a> - 1.2 KB - 2025-01-30 14:30:00</li>
<li><a href="image.jpg">image.jpg</a> - 45.6 KB - 2025-01-30 12:15:00</li>
</ul>
</body>
</html>
```

---

### upload_path

**Syntaxe**: `upload_path <directory>;`

**Description**: Répertoire où les fichiers uploadés sont sauvegardés

**Exemple**:
```nginx
location /upload {
    upload_path www/upload/;
    allow_methods GET POST DELETE;
}
```

**Notes**:
- Utilisé pour les requêtes POST avec `multipart/form-data`
- Le répertoire doit exister et être accessible en écriture
- Permissions recommandées: `755` (répertoire), `644` (fichiers)

---

### cgi

**Syntaxe**: `cgi <extension> <interpreter_path>;`

**Description**: Associe une extension de fichier à un interpréteur CGI

**Exemple**:
```nginx
location /cgi-bin {
    cgi .py /usr/bin/python3;
    cgi .sh /bin/bash;
    cgi .pl /usr/bin/perl;
}
```

**Process**:
1. Requête: `GET /cgi-bin/script.py`
2. Serveur détecte extension `.py`
3. Fork + execve: `/usr/bin/python3 www/cgi-bin/script.py`
4. Variables CGI passées dans l'environnement
5. Sortie du script retournée comme réponse HTTP

**Variables CGI passées**:
- `REQUEST_METHOD` (GET, POST, etc.)
- `QUERY_STRING` (paramètres URL)
- `CONTENT_TYPE` (type du body POST)
- `CONTENT_LENGTH` (taille du body POST)
- `SCRIPT_NAME` (nom du script)
- `SERVER_NAME` (hostname du serveur)
- `SERVER_PORT` (port d'écoute)
- `HTTP_*` (tous les headers HTTP)

---

### return

**Syntaxe**: `return <code> <url>;`

**Description**: Retourne une redirection HTTP

**Exemple**:
```nginx
location /old-page {
    return 301 /new-page.html;
}

location /external {
    return 302 https://www.example.com;
}

location /temp-redirect {
    return 303 /thanks.html;
}
```

**Codes de redirection**:
- `301` Moved Permanently (redirection permanente)
- `302` Found (redirection temporaire)
- `303` See Other (redirection POST → GET)

---

## Exemples de Configuration

### Configuration Minimale

```nginx
server {
    listen 8080;
    server_name localhost;
    root www;

    location / {
        allow_methods GET;
        index index.html;
    }
}
```

**Usage**: Serveur HTTP simple pour fichiers statiques

---

### Configuration Multi-Serveurs

```nginx
# Serveur principal sur port 8080
server {
    listen 8080;
    server_name localhost;
    root www;

    location / {
        allow_methods GET POST;
        index index.html;
    }
}

# API sur port 8081
server {
    listen 8081;
    server_name api.localhost;
    root www/api;

    location / {
        allow_methods GET POST DELETE;
    }
}

# Admin sur port 8082
server {
    listen 8082;
    server_name admin.localhost;
    root www/admin;
    client_max_body_size 10M;

    location / {
        allow_methods GET POST;
        index admin.html;
    }
}
```

---

### Configuration avec CGI

```nginx
server {
    listen 8080;
    server_name localhost;
    root www;
    client_max_body_size 2M;

    # Pages statiques
    location / {
        allow_methods GET;
        index index.html;
    }

    # Scripts CGI
    location /cgi-bin {
        root www/cgi-bin;
        cgi .py /usr/bin/python3;
        cgi .sh /bin/bash;
        allow_methods GET POST;
    }
}
```

---

### Configuration avec Upload

```nginx
server {
    listen 8080;
    server_name localhost;
    root www;
    client_max_body_size 5M;

    error_pages 413 /errors/too_large.html;

    location / {
        allow_methods GET;
        index index.html;
    }

    location /upload {
        upload_path www/upload/;
        allow_methods GET POST DELETE;
        autoindex on;
    }
}
```

---

### Configuration Complète (Production)

```nginx
server {
    listen 8080;
    server_name www.mysite.com;
    root www;
    client_max_body_size 2M;

    # Error pages personnalisées
    error_pages 404 /errors/404.html;
    error_pages 500 /errors/500.html;
    error_pages 403 /errors/403.html;
    error_pages 413 /errors/413.html;

    # Page d'accueil
    location / {
        allow_methods GET;
        index index.html;
    }

    # Documentation
    location /docs {
        allow_methods GET;
        index docs.html;
        autoindex on;
    }

    # API CGI
    location /cgi-bin {
        root www/cgi-bin;
        cgi .py /usr/bin/python3;
        cgi .sh /bin/bash;
        allow_methods GET POST;
    }

    # Zone d'upload
    location /upload {
        upload_path www/upload/;
        allow_methods GET POST DELETE;
        autoindex on;
    }

    # Redirections
    location /old-site {
        return 301 /;
    }

    location /external-link {
        return 302 https://github.com;
    }
}
```

---

## Best Practices

### Sécurité

#### 1. Limiter la taille des uploads

```nginx
client_max_body_size 2M;  # Évite les DoS par upload massif
```

#### 2. Restreindre les méthodes HTTP

```nginx
location /static {
    allow_methods GET;  # Pas de modification des fichiers statiques
}

location /api {
    allow_methods GET POST DELETE;  # Seulement ce qui est nécessaire
}
```

#### 3. Séparer les répertoires CGI

```nginx
location /cgi-bin {
    root www/cgi-bin;  # Scripts dans un dossier dédié
    # Évite l'exécution de scripts uploadés par les users
}
```

#### 4. Configurer les error pages

```nginx
error_pages 404 /errors/404.html;
error_pages 500 /errors/500.html;
# Évite de révéler des informations sensibles
```

---

### Performance

#### 1. Utiliser autoindex avec parcimonie

```nginx
# OK pour les répertoires d'upload
location /upload {
    autoindex on;
}

# Pas OK pour la racine (performance)
location / {
    autoindex off;  # Forcer index.html
    index index.html;
}
```

#### 2. Organiser les locations par spécificité

```nginx
# Plus spécifique en premier pour un matching rapide
location /api/v2/users {
    # ...
}

location /api/v2 {
    # ...
}

location /api {
    # ...
}

location / {
    # Catch-all en dernier
}
```

---

### Maintenance

#### 1. Commenter la configuration

```nginx
server {
    listen 8080;
    server_name localhost;
    root www;

    # Limite uploads à 2MB (policy: 2025-01-30)
    client_max_body_size 2M;

    # Redirection ancien site (migration: 2025-01)
    location /old {
        return 301 /new;
    }
}
```

#### 2. Grouper les directives similaires

```nginx
# Error pages ensemble
error_pages 404 /errors/404.html;
error_pages 500 /errors/500.html;
error_pages 403 /errors/403.html;

# CGI extensions ensemble
cgi .py /usr/bin/python3;
cgi .sh /bin/bash;
cgi .pl /usr/bin/perl;
```

#### 3. Utiliser des noms de fichiers descriptifs

```bash
conf/
├── webserv.conf           # Configuration principale
├── webserv-dev.conf       # Configuration développement
├── webserv-prod.conf      # Configuration production
└── webserv-test.conf      # Configuration tests
```

---

### Validation de Configuration

#### 1. Tester avant de déployer

```bash
# Lancer en test
./webserv conf/new-config.conf &
PID=$!

# Tester
curl http://127.0.0.1:8080/

# Si OK, déployer
kill $PID
cp conf/new-config.conf conf/webserv.conf
```

#### 2. Vérifier les permissions

```bash
# Configuration lisible
chmod 644 conf/webserv.conf

# Root readable/listable
chmod 755 www/
chmod 644 www/index.html

# Upload writable
chmod 755 www/upload/

# CGI exécutable
chmod 755 www/cgi-bin/
chmod +x www/cgi-bin/*.py
chmod +x www/cgi-bin/*.sh
```

#### 3. Valider les paths

```bash
# Vérifier que les fichiers existent
ls -la www/errors/404.html
ls -la www/index.html
ls -la www/upload/

# Vérifier les interpréteurs CGI
which python3  # /usr/bin/python3
which bash     # /bin/bash
```

---

## Troubleshooting

### Serveur ne démarre pas

```bash
# Erreur: "Address already in use"
lsof -i :8080        # Vérifier si port occupé
pkill -9 webserv     # Tuer processus existant

# Erreur: "Permission denied" (port < 1024)
sudo ./webserv conf/webserv.conf

# Erreur: "Config file not found"
ls -la conf/webserv.conf
```

### CGI ne fonctionne pas

```bash
# Vérifier configuration
grep "cgi" conf/webserv.conf

# Vérifier interpréteur
which python3
ls -la /usr/bin/python3

# Vérifier permissions
ls -la www/cgi-bin/script.py
chmod +x www/cgi-bin/script.py

# Tester script manuellement
./www/cgi-bin/script.py
```

### Upload échoue

```bash
# Vérifier limit
grep "client_max_body_size" conf/webserv.conf

# Vérifier permissions
ls -la www/upload/
chmod 755 www/upload/

# Tester avec petit fichier
echo "test" | curl -X POST --data-binary @- http://127.0.0.1:8080/upload/test.txt
```

---

**Dernière mise à jour**: 2025-01-30
**Version**: 1.0.0
