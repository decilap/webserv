#!/usr/bin/env python3
import os, sys
ct = os.environ.get("CONTENT_TYPE","")
cl = int(os.environ.get("CONTENT_LENGTH","0") or 0)
body = sys.stdin.read(cl) if cl > 0 else ""
print("Status: 200 OK")
print("Content-Type: text/plain")
print()
print("METHOD=" + os.environ.get("REQUEST_METHOD",""))
print("QUERY=" + os.environ.get("QUERY_STRING",""))
print("CONTENT_TYPE=" + ct)
print("BODY=" + body)
