#!/usr/bin/env python3
import os
import sys
import time
from http.cookies import SimpleCookie

# Parse cookies from HTTP_COOKIE
cookies = SimpleCookie()
cookie_header = os.environ.get('HTTP_COOKIE', '')
if cookie_header:
    cookies.load(cookie_header)

# Get session ID from cookie or create new one
session_id = cookies.get('session_id')
if session_id:
    session_id = session_id.value
    visit_count_cookie = cookies.get('visit_count')
    visit_count = int(visit_count_cookie.value) if visit_count_cookie else 1
    visit_count += 1
else:
    session_id = str(int(time.time() * 1000))
    visit_count = 1

# Set cookies in response - CGI headers must use \r\n
sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write(f"Set-Cookie: session_id={session_id}; Path=/; Max-Age=3600\r\n")
sys.stdout.write(f"Set-Cookie: visit_count={visit_count}; Path=/; Max-Age=3600\r\n")
sys.stdout.write(f"Set-Cookie: username=demo_user; Path=/; Max-Age=3600\r\n")
sys.stdout.write("\r\n")

# HTML response
print("<html>")
print("<head><title>Session Management Demo</title>")
print("<style>")
print("body { font-family: Arial; padding: 30px; background: #f5f5f5; }")
print(".container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }")
print("h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }")
print(".info { background: #e8f5e9; padding: 15px; border-radius: 5px; margin: 10px 0; }")
print(".cookie { background: #fff3e0; padding: 10px; margin: 5px 0; border-left: 4px solid #ff9800; }")
print("</style>")
print("</head>")
print("<body>")
print("<div class='container'>")
print("<h1>🍪 Cookie & Session Management Demo</h1>")
print("<div class='info'>")
print(f"<h2>Session Information:</h2>")
print(f"<p><b>Session ID:</b> {session_id}</p>")
print(f"<p><b>Visit Count:</b> {visit_count}</p>")
print(f"<p><b>Status:</b> {'Returning visitor' if visit_count > 1 else 'First visit'}</p>")
print("</div>")

print("<h2>Cookies Set:</h2>")
print("<div class='cookie'><b>session_id:</b> " + session_id + " (expires in 1 hour)</div>")
print("<div class='cookie'><b>visit_count:</b> " + str(visit_count) + " (expires in 1 hour)</div>")
print("<div class='cookie'><b>username:</b> demo_user (expires in 1 hour)</div>")

print("<h2>Request Information:</h2>")
print(f"<p><b>Method:</b> {os.environ.get('REQUEST_METHOD', 'N/A')}</p>")
print(f"<p><b>Query String:</b> {os.environ.get('QUERY_STRING', 'N/A')}</p>")
print(f"<p><b>User Agent:</b> {os.environ.get('HTTP_USER_AGENT', 'N/A')}</p>")

print("<p style='margin-top: 20px; padding: 15px; background: #e3f2fd; border-radius: 5px;'>")
print("<b>💡 Tip:</b> Refresh this page to see the visit counter increment!")
print("</p>")
print("</div>")
print("</body>")
print("</html>")
