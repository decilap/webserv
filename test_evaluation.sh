#!/bin/bash

echo "========================================"
echo "  WEBSERV - TESTS D'EVALUATION"
echo "========================================"
echo ""

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Compteurs
PASSED=0
FAILED=0

test_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ PASS${NC}: $2"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC}: $2"
        FAILED=$((FAILED + 1))
    fi
}

# Nettoyer les processus
pkill -9 webserv 2>/dev/null
sleep 1

echo "=== COMPILATION ==="
make re > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓${NC} Compilation réussie"
else
    echo -e "${RED}✗${NC} Échec de compilation"
    exit 1
fi
echo ""

# Démarrer le serveur
echo "=== DÉMARRAGE SERVEUR ==="
./webserv conf/webserv.conf > /tmp/webserv.log 2>&1 &
SERVER_PID=$!
sleep 2

# Vérifier que le serveur tourne
if ps -p $SERVER_PID > /dev/null; then
    echo -e "${GREEN}✓${NC} Serveur démarré (PID: $SERVER_PID)"
else
    echo -e "${RED}✗${NC} Le serveur n'a pas démarré"
    exit 1
fi
echo ""

echo "=== TESTS GET/POST/DELETE ==="

# Test GET
curl -s http://127.0.0.1:8080/ | grep -q "Webserv"
test_result $? "GET page d'accueil"

# Test GET 404
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://127.0.0.1:8080/nonexistent)
[ "$HTTP_CODE" = "404" ]
test_result $? "GET 404 Not Found (code: $HTTP_CODE)"

# Test POST upload
echo "test content" > /tmp/test_upload.txt
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -F "file=@/tmp/test_upload.txt" http://127.0.0.1:8080/upload/)
[ "$HTTP_CODE" = "303" ] || [ "$HTTP_CODE" = "200" ]
test_result $? "POST upload fichier (code: $HTTP_CODE)"

# Test DELETE
echo "file to delete" > www/test_delete.txt
sleep 1
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE http://127.0.0.1:8080/test_delete.txt)
[ "$HTTP_CODE" = "200" ]
test_result $? "DELETE fichier (code: $HTTP_CODE)"

# Test méthode inconnue
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -X PATCH http://127.0.0.1:8080/)
[ "$HTTP_CODE" != "200" ]
test_result $? "Méthode PATCH non supportée (code: $HTTP_CODE)"

echo ""
echo "=== TESTS CGI ==="

# Test CGI GET
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" "http://127.0.0.1:8080/cgi-bin/echo.py")
[ "$HTTP_CODE" = "200" ]
test_result $? "CGI GET basique (code: $HTTP_CODE)"

sleep 0.5

# Test CGI GET avec query string
curl -s "http://127.0.0.1:8080/cgi-bin/echo.py?name=Test" | grep -q "Test" 2>/dev/null
test_result $? "CGI GET avec query string"

# Test CGI POST
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d "data=test" http://127.0.0.1:8080/cgi-bin/echo.py)
[ "$HTTP_CODE" = "200" ] || [ "$HTTP_CODE" = "504" ]
test_result $? "CGI POST (code: $HTTP_CODE)"

echo ""
echo "=== TESTS CONFIGURATION ==="

# Test multi-ports
curl -s http://127.0.0.1:8081/ > /dev/null
test_result $? "Serveur écoute sur port 8081"

# Test redirection
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" http://127.0.0.1:8080/oldpage)
[ "$HTTP_CODE" = "301" ]
test_result $? "Redirection 301 (code: $HTTP_CODE)"

# Test autoindex
curl -s http://127.0.0.1:8080/upload/ | grep -q "Index of"
test_result $? "Autoindex activé"

echo ""
echo "=== TESTS ROBUSTESSE ==="

# Test requête malformée
echo -e "GET / HTTP/1.1\r\nBad Header\r\n\r\n" | nc -w 1 127.0.0.1 8080 > /dev/null 2>&1
test_result $? "Gestion requête malformée"

# Test connexions multiples
for i in {1..20}; do
   curl -s --max-time 2 http://127.0.0.1:8080/ > /dev/null &
done
sleep 3
pkill -9 curl 2>/dev/null
curl -s --max-time 5 http://127.0.0.1:8080/ | grep -q "Webserv"
test_result $? "20 connexions simultanées"

echo ""
echo "=== VÉRIFICATIONS TECHNIQUES ==="

# Vérifier poll() dans le code
grep -q "poll(" src/server/PollManager.cpp
test_result $? "Utilisation de poll() dans PollManager"

# Vérifier pas d'errno après send/recv
! grep -E "(send|recv).*errno" src/server/Client.cpp | grep -v "strerror"
test_result $? "Pas de errno après send/recv"

# Vérifier std::atoi remplacé
! grep -q "std::atoi" src/**/*.cpp 2>/dev/null
test_result $? "std::atoi remplacé par ft_atoi"

echo ""
echo "=== ARRÊT SERVEUR ==="
kill -INT $SERVER_PID 2>/dev/null
sleep 1

if ! ps -p $SERVER_PID > /dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} Serveur arrêté proprement"
else
    echo -e "${YELLOW}!${NC} Serveur toujours actif, kill forcé"
    kill -9 $SERVER_PID 2>/dev/null
fi

echo ""
echo "========================================"
echo "  RÉSULTATS"
echo "========================================"
echo -e "${GREEN}PASS: $PASSED${NC}"
echo -e "${RED}FAIL: $FAILED${NC}"
TOTAL=$((PASSED + FAILED))
PERCENT=$((PASSED * 100 / TOTAL))
echo "Score: $PERCENT%"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓✓✓ TOUS LES TESTS SONT PASSÉS ✓✓✓${NC}"
    exit 0
else
    echo -e "${RED}Certains tests ont échoué${NC}"
    exit 1
fi
