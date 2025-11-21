#!/usr/bin/env python3
import os
import sys

# IMPORTANT: CGI headers must end with \r\n
sys.stdout.write("Content-Type: text/plain\r\n")
sys.stdout.write("\r\n")

# Body
sys.stdout.write("Hello from CGI Python!\n")
sys.stdout.write(f"Method: {os.environ.get('REQUEST_METHOD', 'N/A')}\n")

# Parse query string
query = os.environ.get("QUERY_STRING", "")
if query:
    sys.stdout.write(f"Query: {query}\n")
    # Parse parameters
    for pair in query.split("&"):
        if "=" in pair:
            key, val = pair.split("=", 1)
            sys.stdout.write(f"{key}: {val}\n")

sys.stdout.flush()
