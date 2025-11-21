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

# Headers with \r\n
sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")

# HTML Body - using write() instead of print()
sys.stdout.write("<!DOCTYPE html>\n")
sys.stdout.write("<html>\n")
sys.stdout.write("<head><title>Session Demo</title>\n")
sys.stdout.write("<style>\n")
sys.stdout.write("body { font-family: Arial; padding: 30px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }\n")
sys.stdout.write(".container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 15px; box-shadow: 0 4px 20px rgba(0,0,0,0.2); }\n")
sys.stdout.write("h1 { color: #667eea; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }\n")
sys.stdout.write(".info { background: #e8f5e9; padding: 15px; border-radius: 8px; margin: 15px 0; }\n")
sys.stdout.write(".badge { background: #4CAF50; color: white; padding: 5px 15px; border-radius: 20px; font-weight: bold; }\n")
sys.stdout.write("</style>\n")
sys.stdout.write("</head>\n")
sys.stdout.write("<body>\n")
sys.stdout.write("<div class='container'>\n")
sys.stdout.write("<h1>🍪 Session Management Demo</h1>\n")
sys.stdout.write("<div class='info'>\n")
sys.stdout.write(f"<h2>Session Information:</h2>\n")
sys.stdout.write(f"<p><b>Session ID:</b> <span class='badge'>{session_id}</span></p>\n")
sys.stdout.write(f"<p><b>Visit Count:</b> <span class='badge'>{visit_count}</span></p>\n")
sys.stdout.write(f"<p><b>Status:</b> {'🔄 Returning visitor' if visit_count > 1 else '👋 First visit'}</p>\n")
sys.stdout.write("</div>\n")
sys.stdout.write("<div style='margin-top: 20px; padding: 15px; background: #e3f2fd; border-radius: 8px; border-left: 4px solid #2196F3;'>\n")
sys.stdout.write("<p><b>💡 Tip:</b> Refresh this page to see the visit counter increment!</p>\n")
sys.stdout.write("<p><small>Cookies are stored in your browser for demonstration.</small></p>\n")
sys.stdout.write("</div>\n")
sys.stdout.write("<div style='text-align: center; margin-top: 30px;'>\n")
sys.stdout.write("<a href='/login.html' style='display: inline-block; padding: 12px 30px; background: #667eea; color: white; text-decoration: none; border-radius: 8px; font-weight: bold;'>🔐 Go to Login</a>\n")
sys.stdout.write("</div>\n")
sys.stdout.write("</div>\n")
sys.stdout.write("</body>\n")
sys.stdout.write("</html>\n")

sys.stdout.flush()
