#!/bin/bash

echo "╔═══════════════════════════════════════════════════════════╗"
echo "║        🔍 TEST VALGRIND - Webserv Memory Check           ║"
echo "╚═══════════════════════════════════════════════════════════╝"
echo ""

# Nettoyage
pkill -9 webserv 2>/dev/null
sleep 1
rm -f /tmp/valgrind_webserv.log

echo "🚀 Démarrage du serveur sous Valgrind..."
timeout 15 valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --log-file=/tmp/valgrind_webserv.log \
    ./webserv conf/webserv.conf > /tmp/webserv_stdout.log 2>&1 &

VALGRIND_PID=$!

# Attendre que le serveur démarre
echo "⏳ Attente du démarrage..."
sleep 3

echo "📡 Envoi de requêtes de test..."

# Test 1: Page statique
echo "  • GET /"
curl -s -o /dev/null "http://127.0.0.1:8080/"

# Test 2: CGI
echo "  • CGI info.sh"
curl -s -o /dev/null "http://127.0.0.1:8080/cgi-bin/info.sh"

# Test 3: POST
echo "  • POST /upload"
echo "test data" | curl -s -X POST -d @- -o /dev/null "http://127.0.0.1:8080/upload"

# Test 4: Multiple requests
echo "  • 5 requêtes multiples"
for i in {1..5}; do
    curl -s -o /dev/null "http://127.0.0.1:8080/" &
done
wait

sleep 1

echo ""
echo "🛑 Arrêt du serveur (SIGTERM pour rapport propre)..."
kill -TERM $VALGRIND_PID 2>/dev/null
sleep 3

# Si toujours en cours, forcer
if ps -p $VALGRIND_PID > /dev/null 2>&1; then
    echo "   Force kill..."
    kill -9 $VALGRIND_PID
    sleep 2
fi

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "               📊 RAPPORT VALGRIND"
echo "═══════════════════════════════════════════════════════════"
echo ""

if [ -f /tmp/valgrind_webserv.log ]; then
    # Résumé des erreurs
    if grep -q "ERROR SUMMARY" /tmp/valgrind_webserv.log; then
        echo "🔍 ERREURS:"
        grep "ERROR SUMMARY" /tmp/valgrind_webserv.log
        echo ""
    fi

    # Résumé des fuites
    if grep -q "LEAK SUMMARY" /tmp/valgrind_webserv.log; then
        echo "💧 FUITES MÉMOIRE:"
        grep -A 10 "LEAK SUMMARY" /tmp/valgrind_webserv.log
        echo ""
    fi

    # Résumé heap
    if grep -q "HEAP SUMMARY" /tmp/valgrind_webserv.log; then
        echo "📦 HEAP:"
        grep -A 5 "HEAP SUMMARY" /tmp/valgrind_webserv.log
        echo ""
    fi

    # Si pas de rapport complet (timeout), afficher les dernières lignes
    if ! grep -q "LEAK SUMMARY" /tmp/valgrind_webserv.log; then
        echo "⚠️  Rapport incomplet (serveur timeout)"
        echo "Dernières lignes du log:"
        tail -20 /tmp/valgrind_webserv.log
    fi
else
    echo "❌ Fichier log Valgrind introuvable"
fi

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "📁 Log complet: /tmp/valgrind_webserv.log"
echo "═══════════════════════════════════════════════════════════"
