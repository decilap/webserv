#!/usr/bin/env python3
import os, sys
print("Content-Type: text/plain\n")
print("Hello from CGI Python!")
print("Method:", os.environ.get("REQUEST_METHOD", ""))

# Parse query string
query = os.environ.get("QUERY_STRING", "")
if query:
    print("Query:", query)
    # Parse parameters
    for pair in query.split("&"):
        if "=" in pair:
            key, val = pair.split("=", 1)
            print(f"{key}: {val}")
