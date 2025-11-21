#!/usr/bin/env python3
import sys
import time

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")
sys.stdout.write(f"<h1>Test without cookie - Time: {time.time()}</h1>\n")
sys.stdout.flush()
