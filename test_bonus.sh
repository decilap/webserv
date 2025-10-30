#!/bin/bash

echo "========================================"
echo "  WEBSERV - TESTS BONUS"
echo "========================================"
echo ""

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

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

# Fonction pour nettoyer complètement
cleanup() {
    pkill -9 webserv 2>/dev/null
    sleep 1
    # Vérifier qu'aucun processus ne reste
    while pgrep webserv > /dev/null 2>&1; do
        pkill -9 webserv 2>/dev/null
        sleep 0.5
    done
}

# Nettoyage initial complet
cleanup

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
./webserv conf/webserv.conf > /tmp/webserv_bonus.log 2>&1 &
SERVER_PID=$!
sleep 3

if ! ps -p $SERVER_PID > /dev/null; then
    echo -e "${RED}✗${NC} Le serveur n'a pas démarré"
    cat /tmp/webserv_bonus.log
    exit 1
fi
echo -e "${GREEN}✓${NC} Serveur démarré (PID: $SERVER_PID)"
echo ""

echo "=== BONUS 1: Multiple CGI Types ===" 

# Test Python CGI
sleep 1
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" --max-time 5 "http://127.0.0.1:8080/cgi-bin/echo.py")
[ "$HTTP_CODE" = "200" ]
test_result $? "Python CGI (.py) - Code: $HTTP_CODE"

sleep 1

# Test Shell CGI
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" --max-time 5 "http://127.0.0.1:8080/cgi-bin/info.sh")
[ "$HTTP_CODE" = "200" ]
test_result $? "Shell CGI (.sh) - Code: $HTTP_CODE"

sleep 1

# Vérifier contenu Shell CGI
CONTENT=$(curl -s --max-time 5 "http://127.0.0.1:8080/cgi-bin/info.sh")
echo "$CONTENT" | grep -q "Server Information"
test_result $? "Shell CGI retourne du contenu valide"

echo ""
echo "=== BONUS 2: Cookies & Session Management ===" 

sleep 1

# Test session CGI basique
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" --max-time 5 "http://127.0.0.1:8080/cgi-bin/session.py")
[ "$HTTP_CODE" = "200" ]
test_result $? "Session CGI accessible (Code: $HTTP_CODE)"

sleep 1

# Test contenu de la page
CONTENT=$(curl -s --max-time 5 "http://127.0.0.1:8080/cgi-bin/session.py")
echo "$CONTENT" | grep -q "Cookie"
test_result $? "Page contient le mot 'Cookie'"

echo "$CONTENT" | grep -q "Session"
test_result $? "Page contient le mot 'Session'"

# Test avec headers pour voir les Set-Cookie
sleep 1
HEADERS=$(curl -s -i --max-time 5 "http://127.0.0.1:8080/cgi-bin/session.py" | head -20)
echo "$HEADERS" | grep -q "Set-Cookie"
test_result $? "Le serveur envoie des headers Set-Cookie"

echo ""
echo "=== VÉRIFICATION CONFIGURATION ==="

# Vérifier que la config contient bien les deux CGI
grep -q "cgi .py" conf/webserv.conf
test_result $? "Configuration contient Python CGI"

grep -q "cgi .sh" conf/webserv.conf
test_result $? "Configuration contient Shell CGI"

echo ""
echo "=== ARRÊT SERVEUR ==="
kill -INT $SERVER_PID 2>/dev/null
sleep 2

if ! ps -p $SERVER_PID > /dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} Serveur arrêté proprement"
else
    echo -e "${YELLOW}!${NC} Serveur toujours actif, kill forcé"
    kill -9 $SERVER_PID 2>/dev/null
fi

# Nettoyage final
cleanup

echo ""
echo "========================================"
echo "  RÉSULTATS BONUS"
echo "========================================"
echo -e "${GREEN}PASS: $PASSED${NC}"
echo -e "${RED}FAIL: $FAILED${NC}"
TOTAL=$((PASSED + FAILED))
if [ $TOTAL -gt 0 ]; then
    PERCENT=$((PASSED * 100 / TOTAL))
    echo "Score: $PERCENT%"
fi
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓✓✓ TOUS LES BONUS SONT IMPLÉMENTÉS ✓✓✓${NC}"
    exit 0
else
    echo -e "${YELLOW}Note: Les bonus sont optionnels et ajoutent des points supplémentaires${NC}"
    exit 0
fi
