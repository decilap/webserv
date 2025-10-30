#!/usr/bin/env python3
import os, sys
print("Content-Type: text/plain\n")
print("Hello from CGI Python!")
print("Method:", os.environ.get("REQUEST_METHOD", ""))
