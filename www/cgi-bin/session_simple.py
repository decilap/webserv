#!/usr/bin/env python3
import os
import sys
import time

# Generate simple session ID
session_id = str(int(time.time() * 1000))

# Headers
sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write(f"Set-Cookie: session_id={session_id}; Path=/; Max-Age=3600\r\n")
sys.stdout.write(f"Set-Cookie: visit_count=1; Path=/; Max-Age=3600\r\n")
sys.stdout.write("\r\n")

# Simple HTML body
sys.stdout.write("<html><head><title>Session Test</title></head>\n")
sys.stdout.write("<body>\n")
sys.stdout.write(f"<h1>Session ID: {session_id}</h1>\n")
sys.stdout.write("<p>Visit count: 1</p>\n")
sys.stdout.write("</body></html>\n")

sys.stdout.flush()
