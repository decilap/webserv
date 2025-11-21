#!/bin/bash

# Script de test des fonctionnalités bonus
# Tests du système de login et du gestionnaire d'uploads

echo "🧪 Tests des fonctionnalités bonus - Webserv"
echo "=============================================="
echo ""

SERVER="http://localhost:8080"
SUCCESS=0
FAILED=0

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test function
test_endpoint() {
    local name=$1
    local method=$2
    local url=$3
    local data=$4
    local expected_code=$5

    echo -n "Testing: $name ... "

    if [ "$method" == "POST" ]; then
        response=$(curl -s -w "\n%{http_code}" -X POST "$url" -d "$data" 2>/dev/null)
    elif [ "$method" == "DELETE" ]; then
        response=$(curl -s -w "\n%{http_code}" -X DELETE "$url" 2>/dev/null)
    else
        response=$(curl -s -w "\n%{http_code}" "$url" 2>/dev/null)
    fi

    http_code=$(echo "$response" | tail -n 1)
    body=$(echo "$response" | head -n -1)

    if [[ "$http_code" == "$expected_code"* ]]; then
        echo -e "${GREEN}✓ PASS${NC} (HTTP $http_code)"
        ((SUCCESS++))
        return 0
    else
        echo -e "${RED}✗ FAIL${NC} (Expected $expected_code, got $http_code)"
        ((FAILED++))
        return 1
    fi
}

echo "📋 Section 1: Pages HTML"
echo "------------------------"
test_endpoint "Login page" "GET" "$SERVER/login.html" "" "200"
test_endpoint "Dashboard page" "GET" "$SERVER/dashboard.html" "" "200"
test_endpoint "Upload page" "GET" "$SERVER/upload.html" "" "200"
echo ""

echo "🔐 Section 2: Authentification"
echo "-------------------------------"
test_endpoint "Login avec admin/password" "POST" "$SERVER/cgi-bin/login.py" "username=admin&password=password&remember=0" "200"
test_endpoint "Login avec credentials invalides" "POST" "$SERVER/cgi-bin/login.py" "username=admin&password=wrong" "401"
test_endpoint "Login sans données" "POST" "$SERVER/cgi-bin/login.py" "" "400"
test_endpoint "Logout" "POST" "$SERVER/cgi-bin/logout.py" "" "200"
echo ""

echo "📤 Section 3: Upload de fichiers"
echo "---------------------------------"

# Create a test file
TEST_FILE="/tmp/webserv_test_$(date +%s).txt"
echo "Test content from bonus test script" > "$TEST_FILE"

# Test upload
echo -n "Testing: Upload d'un fichier ... "
response=$(curl -s -w "\n%{http_code}" -X POST "$SERVER/upload" -F "file=@$TEST_FILE" 2>/dev/null)
http_code=$(echo "$response" | tail -n 1)
# Accept both 200 (OK) and 303 (See Other redirect after upload)
if [[ "$http_code" == "2"* ]] || [[ "$http_code" == "303" ]]; then
    echo -e "${GREEN}✓ PASS${NC} (HTTP $http_code)"
    ((SUCCESS++))
else
    echo -e "${RED}✗ FAIL${NC} (HTTP $http_code)"
    ((FAILED++))
fi

# Test listing
test_endpoint "Liste des fichiers uploadés" "GET" "$SERVER/upload/" "" "200"

# Test delete
FILENAME=$(basename "$TEST_FILE")
echo -n "Testing: Suppression du fichier ... "
response=$(curl -s -w "\n%{http_code}" -X DELETE "$SERVER/upload/$FILENAME" 2>/dev/null)
http_code=$(echo "$response" | tail -n 1)
if [[ "$http_code" == "2"* ]]; then
    echo -e "${GREEN}✓ PASS${NC} (HTTP $http_code)"
    ((SUCCESS++))
else
    echo -e "${RED}✗ FAIL${NC} (HTTP $http_code)"
    ((FAILED++))
fi

# Cleanup
rm -f "$TEST_FILE"
echo ""

echo "📊 Section 4: Tests CGI Python"
echo "-------------------------------"
test_endpoint "Session CGI demo" "GET" "$SERVER/cgi-bin/session_demo.py" "" "200"
test_endpoint "Echo CGI" "GET" "$SERVER/cgi-bin/echo.py?test=hello" "" "200"
test_endpoint "Info CGI" "GET" "$SERVER/cgi-bin/info.sh" "" "200"
echo ""

echo "=============================================="
echo "📊 Résultats des tests"
echo "=============================================="
echo -e "Tests réussis: ${GREEN}$SUCCESS${NC}"
echo -e "Tests échoués: ${RED}$FAILED${NC}"
echo -e "Total: $((SUCCESS + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 Tous les tests sont passés avec succès!${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠️  Certains tests ont échoué.${NC}"
    exit 1
fi
