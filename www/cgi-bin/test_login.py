#!/usr/bin/env python3
import sys

# Immediate response without reading stdin
sys.stdout.write("Content-Type: text/plain\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("Hello from CGI!\n")
sys.stdout.flush()
