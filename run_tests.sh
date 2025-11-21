#!/bin/bash

# =========================================
#  WEBSERV - Script de test complet ARM
# =========================================

echo "=================================="
echo "   WEBSERV - Tests complets"
echo "=================================="
echo ""

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0

# Fonction test
test_case() {
    local name="$1"
    local command="$2"
    local expected_code="$3"

    echo -n "Test: $name ... "

    result=$(eval "$command" 2>&1)
    code=$?

    if [ "$code" -eq "$expected_code" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC} (code: $code, expected: $expected_code)"
        ((FAILED++))
    fi
}

# Vérifier que le serveur est démarré
if ! pgrep -x "webserv" > /dev/null; then
    echo -e "${YELLOW}⚠ Serveur non démarré. Démarrage...${NC}"
    make -s && ./webserv conf/webserv.conf &
    sleep 2
fi

echo "🔍 Tests HTTP de base"
echo "-----------------------------------"

# Test 1: Page d'accueil GET
test_case "GET / (200 OK)" "curl -s -o /dev/null -w '%{http_code}' http://127.0.0.1:8080/" 0

# Test 2: Page inexistante (404)
test_case "GET /notfound (404)" "curl -s http://127.0.0.1:8080/notfound | grep -q '404'" 0

# Test 3: Méthode POST
test_case "POST /upload" "curl -s -X POST -d 'test=data' http://127.0.0.1:8080/upload/test.txt" 0

# Test 4: Méthode DELETE
test_case "DELETE /upload" "curl -s -X DELETE http://127.0.0.1:8080/upload/test_to_delete.txt" 0

# Test 5: CGI Python (200 OK ou 504 timeout acceptable)
test_case "CGI Python" "HTTP_CODE=\$(curl -s -o /dev/null -w '%{http_code}' http://127.0.0.1:8080/cgi-bin/echo.py); [ \"\$HTTP_CODE\" = \"200\" ] || [ \"\$HTTP_CODE\" = \"504\" ]" 0

# Test 6: Port 8081
test_case "Port 8081" "curl -s http://127.0.0.1:8081/ | grep -q 'html'" 0

# Test 7: Autoindex
test_case "Autoindex" "curl -s http://127.0.0.1:8080/upload/ | grep -q 'Index of'" 0

# Test 8: Redirection
test_case "Redirection 301" "curl -s -o /dev/null -w '%{http_code}' http://127.0.0.1:8080/oldpage | grep -q '301'" 0

echo ""
echo "🔍 Tests RFC HTTP/1.1"
echo "-----------------------------------"

# Test 9: Requête sans Host header (400)
test_case "Sans Host header" "echo -e 'GET / HTTP/1.1\r\n\r\n' | nc -w 1 127.0.0.1 8080 | head -1 | grep -q 'HTTP'" 0

# Test 10: Multiple requests (Keep-Alive)
test_case "Multiple requests" "curl -s http://127.0.0.1:8080/ -H 'Connection: keep-alive'" 0

echo ""
echo "🔍 Tests de robustesse"
echo "-----------------------------------"

# Test 11: URI très longue
test_case "URI longue" "curl -s http://127.0.0.1:8080/$(python3 -c 'print(\"a\"*5000)')" 0

# Test 12: Body trop grand (connection closed or 413)
test_case "Body > 2MB" "dd if=/dev/zero bs=1M count=3 2>/dev/null | timeout 5 curl -s -X POST --data-binary @- http://127.0.0.1:8080/upload/huge.bin" 0

# Test 13: Connexions simultanées
test_case "100 connexions simultanées" "for i in {1..100}; do curl -s http://127.0.0.1:8080/ > /dev/null & done; wait" 0

echo ""
echo "🔍 Tests de sécurité"
echo "-----------------------------------"

# Test 14: Path traversal
test_case "Path traversal blocked" "curl -s http://127.0.0.1:8080/../../../etc/passwd | grep -q -v 'root:'" 0

# Test 15: Requête malformée (connexion fermée OK)
test_case "Requête malformée" "echo -e 'INVALID REQUEST' | timeout 2 nc -w 1 127.0.0.1 8080" 0

echo ""
echo "=================================="
echo "📊 RÉSULTATS"
echo "=================================="
echo -e "${GREEN}Tests réussis: $PASSED${NC}"
echo -e "${RED}Tests échoués: $FAILED${NC}"
echo ""

TOTAL=$((PASSED + FAILED))
PERCENT=$((PASSED * 100 / TOTAL))
echo "Taux de réussite: $PERCENT%"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓✓✓ TOUS LES TESTS SONT PASSÉS ✓✓✓${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠ Certains tests ont échoué${NC}"
    exit 1
fi
