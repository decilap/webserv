# 🎁 Bonus Session & Upload Manager - Webserv

Système complet de gestion de sessions avec login jQuery et gestionnaire d'uploads pour le projet Webserv 42.

## 📁 Fichiers créés

### 1. Système de Login & Sessions

#### `www/login.html`
Page de connexion moderne avec jQuery
- Interface responsive avec Tailwind CSS
- Validation de formulaire côté client
- Requêtes AJAX vers le CGI
- Gestion des cookies côté client
- Messages d'erreur animés
- Easter egg (code Konami: ↑↑↓↓←→←→BA)

#### `www/cgi-bin/login.py`
Script CGI d'authentification Python
- Authentification des utilisateurs
- Génération de session ID sécurisée (SHA256)
- Support "Se souvenir de moi" (1h ou 24h)
- Création de 6 cookies:
  - `session_id` (HttpOnly)
  - `username`
  - `user_role`
  - `user_name`
  - `login_time`
  - `authenticated`
- Réponses JSON

#### `www/dashboard.html`
Dashboard protégé après authentification
- Vérification automatique de session
- Redirection si non authentifié
- Affichage des informations utilisateur
- Liste des cookies actifs
- Compteur de temps de connexion en temps réel
- Actions rapides (test AJAX, refresh)

#### `www/cgi-bin/logout.py`
Script de déconnexion
- Suppression de tous les cookies de session
- Réponse JSON

### 2. Gestionnaire d'Uploads

#### `www/upload.html`
Interface complète de gestion des fichiers
- Upload AJAX sans rechargement
- Drag & Drop
- Upload multiple simultané
- Barre de progression
- Liste dynamique des fichiers
- Recherche/filtrage
- Sélection multiple
- Suppression avec confirmation
- Statistiques en temps réel

## 🚀 Utilisation

### Démarrer le serveur

```bash
cd /home/lamalice/Documents/ecol42/webserv
./webserv conf/webserv.conf
```

### 1. Système de Login

#### Accéder à la page de login
```
http://localhost:8080/login.html
```

#### Comptes de test disponibles

| Username | Password | Rôle | Nom affiché |
|----------|----------|------|-------------|
| `admin` | `password` | administrator | Administrateur |
| `user` | `password` | user | Utilisateur Standard |
| `guest` | `password` | guest | Invité |

#### Tester avec curl

**Login:**
```bash
curl -i -X POST http://localhost:8080/cgi-bin/login.py \
  -d "username=admin&password=password&remember=1"
```

**Logout:**
```bash
curl -i -X POST http://localhost:8080/cgi-bin/logout.py
```

**Dashboard:**
```
http://localhost:8080/dashboard.html
```

### 2. Gestionnaire d'Uploads

#### Accéder à la page d'upload
```
http://localhost:8080/upload.html
```

#### Fonctionnalités disponibles

1. **Upload de fichiers**
   - Cliquez sur "Parcourir les fichiers"
   - Ou glissez-déposez directement dans la zone
   - Upload multiple supporté
   - Taille max: 2MB par fichier

2. **Liste des fichiers**
   - Affichage automatique de tous les fichiers uploadés
   - Recherche en temps réel
   - Icônes par type de fichier
   - Liens de téléchargement

3. **Suppression**
   - Suppression individuelle (bouton 🗑️)
   - Sélection multiple + suppression en masse
   - Confirmation avant suppression

#### Tester avec curl

**Upload un fichier:**
```bash
curl -X POST http://localhost:8080/upload \
  -F "file=@test.txt"
```

**Lister les fichiers:**
```bash
curl http://localhost:8080/upload/
```

**Supprimer un fichier:**
```bash
curl -X DELETE http://localhost:8080/upload/test.txt
```

## ✨ Fonctionnalités jQuery

### Login System

```javascript
// Validation de formulaire
function validateForm() {
    const username = $('#username').val().trim();
    const password = $('#password').val();
    // ... validation logic
}

// AJAX Login Request
$.ajax({
    url: '/cgi-bin/login.py',
    method: 'POST',
    data: formData,
    success: function(response) {
        // Set cookies client-side
        for (const [key, value] of Object.entries(response.cookies)) {
            document.cookie = `${key}=${value}; path=/; expires=${expires}`;
        }
        // Redirect to dashboard
        window.location.href = '/dashboard.html';
    }
});
```

### Upload Manager

```javascript
// Drag & Drop Handler
$('#upload-zone').on('drop', function(e) {
    const files = e.originalEvent.dataTransfer.files;
    handleFiles(files);
});

// File Upload with FormData
function uploadFile(file, callback) {
    const formData = new FormData();
    formData.append('file', file);

    $.ajax({
        url: '/upload',
        method: 'POST',
        data: formData,
        processData: false,
        contentType: false,
        success: function(response) {
            // File uploaded successfully
        }
    });
}

// File Deletion
function deleteFile(filename) {
    $.ajax({
        url: '/upload/' + filename,
        method: 'DELETE',
        success: function() {
            // File deleted successfully
        }
    });
}
```

## 🔒 Sécurité

### Login System
- **HttpOnly** sur session_id (protection XSS)
- **Validation** des inputs côté serveur
- **Expiration** automatique des cookies (1h ou 24h)
- **Vérification** de session sur le dashboard
- **Redirection** automatique si non authentifié
- **Hash SHA256** pour les session IDs

### Upload System
- **Limite de taille** : 2MB par fichier (configurable)
- **Validation** des requêtes
- **Timeout** de 30 secondes pour les uploads
- **Confirmation** avant suppression

## 🎯 Points Bonus Démontrés

### 1. Cookies & Sessions ✅
- Création de cookies via Set-Cookie headers (CGI Python)
- Lecture de cookies via HTTP_COOKIE
- Gestion de sessions persistantes
- Expiration configurable

### 2. jQuery ✅
- Framework JavaScript moderne
- Manipulation du DOM
- Événements dynamiques
- Requêtes AJAX
- Animations CSS

### 3. AJAX ✅
- Communication asynchrone avec le serveur
- Upload sans rechargement de page
- Mise à jour dynamique de l'interface
- Gestion des erreurs

### 4. CGI Python ✅
- Scripts CGI pour authentification
- Parsing de POST data
- Génération de réponses JSON
- Gestion des variables d'environnement

### 5. Gestion de fichiers ✅
- Upload via POST
- Listing via GET
- Suppression via DELETE
- Support multipart/form-data

## 📊 Architecture

```
┌─────────────────────────────────────────────────────┐
│                    CLIENT (Browser)                  │
│  ┌──────────────┐  ┌──────────────┐  ┌────────────┐ │
│  │  login.html  │  │dashboard.html│  │upload.html │ │
│  │   (jQuery)   │  │   (jQuery)   │  │  (jQuery)  │ │
│  └──────┬───────┘  └──────┬───────┘  └─────┬──────┘ │
└─────────┼──────────────────┼────────────────┼────────┘
          │ AJAX              │ AJAX           │ AJAX
          ↓                   ↓                ↓
┌─────────────────────────────────────────────────────┐
│                 WEBSERV SERVER (C++)                 │
│                                                       │
│  ┌──────────────┐              ┌──────────────────┐ │
│  │ CGI Handler  │              │ Upload Handler   │ │
│  │              │              │                  │ │
│  │  ┌────────┐  │              │  ┌────────────┐ │ │
│  │  │login.py│  │              │  │ POST /upload│ │ │
│  │  └────────┘  │              │  └────────────┘ │ │
│  │  ┌────────┐  │              │  ┌────────────┐ │ │
│  │  │logout.py│ │              │  │ GET /upload │ │ │
│  │  └────────┘  │              │  └────────────┘ │ │
│  └──────────────┘              │  ┌────────────┐ │ │
│                                 │  │DELETE /file│ │ │
│                                 │  └────────────┘ │ │
│                                 └──────────────────┘ │
└─────────────────────────────────────────────────────┘
          │                                    │
          ↓                                    ↓
    [Cookies]                          [www/upload/]
```

## 🧪 Tests

### Test du système de login

1. **Test de connexion réussie:**
   ```bash
   curl -i -X POST http://localhost:8080/cgi-bin/login.py \
     -d "username=admin&password=password&remember=0"
   # Doit retourner: 200 OK avec cookies
   ```

2. **Test d'authentification échouée:**
   ```bash
   curl -i -X POST http://localhost:8080/cgi-bin/login.py \
     -d "username=admin&password=wrong"
   # Doit retourner: 401 Unauthorized
   ```

3. **Test de déconnexion:**
   ```bash
   curl -i -X POST http://localhost:8080/cgi-bin/logout.py
   # Doit retourner: 200 OK avec cookies expirés
   ```

### Test du gestionnaire d'uploads

1. **Test d'upload:**
   ```bash
   echo "Test content" > test.txt
   curl -X POST http://localhost:8080/upload -F "file=@test.txt"
   # Doit retourner: 200 OK
   ```

2. **Test de listing:**
   ```bash
   curl http://localhost:8080/upload/
   # Doit retourner: HTML avec liste des fichiers
   ```

3. **Test de suppression:**
   ```bash
   curl -X DELETE http://localhost:8080/upload/test.txt
   # Doit retourner: 200 OK
   ```

## 📝 Notes

- Tous les scripts CGI sont exécutables (`chmod +x`)
- Les mots de passe sont en clair pour la démonstration (à hasher en production)
- Le système utilise `os.read()` pour éviter les timeouts de stdin
- Les cookies sont définis côté client ET serveur pour compatibilité
- L'autoindex est activé sur `/upload` pour le listing

## 🎨 Captures d'écran

### Login Page
- Interface moderne avec gradient
- Formulaire de validation
- Messages d'erreur animés

### Dashboard
- Informations de session
- Liste des cookies
- Compteur temps réel

### Upload Manager
- Zone drag & drop
- Barre de progression
- Liste avec recherche
- Suppression avec confirmation

## 🔗 Liens rapides

- **Login**: http://localhost:8080/login.html
- **Dashboard**: http://localhost:8080/dashboard.html
- **Upload Manager**: http://localhost:8080/upload.html
- **Bonus Features**: http://localhost:8080/bonus.html
- **Home**: http://localhost:8080/

---

**Auteur**: Claude Code
**Projet**: Webserv 42
**Date**: 2025-10-30
**Technologies**: C++, Python CGI, jQuery, HTML5, CSS3, Tailwind
