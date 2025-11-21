#!/usr/bin/env python3
import sys

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Set-Cookie: test=hello; Path=/\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("<h1>Test with one cookie</h1>\n")
sys.stdout.flush()
