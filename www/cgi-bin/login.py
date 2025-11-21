#!/usr/bin/env python3
import os
import sys
import time
import hashlib
from http.cookies import SimpleCookie
from urllib.parse import parse_qs

# Demo users database (in production, use a real database)
USERS = {
    'admin': {'password': 'password', 'role': 'administrator', 'name': 'Administrateur'},
    'user': {'password': 'password', 'role': 'user', 'name': 'Utilisateur Standard'},
    'guest': {'password': 'password', 'role': 'guest', 'name': 'Invité'}
}

def generate_session_id(username):
    """Generate a unique session ID"""
    timestamp = str(time.time())
    data = f"{username}{timestamp}{os.getpid()}"
    return hashlib.sha256(data.encode()).hexdigest()

def parse_post_data():
    """Parse POST data from stdin"""
    content_length = os.environ.get('CONTENT_LENGTH', '0')
    try:
        length = int(content_length)
        # Only read if there's actual data
        if length <= 0:
            return {}
        if length > 10000:  # Safety limit
            return {}
        # Read from stdin file descriptor directly
        data = os.read(sys.stdin.fileno(), length)
        post_data = data.decode('utf-8')
        return parse_qs(post_data)
    except Exception as e:
        sys.stderr.write(f"Error parsing POST data: {str(e)}\n")
    return {}

def get_user_credentials(post_data):
    """Extract username and password from POST data"""
    username = post_data.get('username', [''])[0]
    password = post_data.get('password', [''])[0]
    remember = post_data.get('remember', ['0'])[0]
    return username, password, remember == '1'

def authenticate(username, password):
    """Authenticate user"""
    if username in USERS:
        if USERS[username]['password'] == password:
            return True, USERS[username]
    return False, None

def send_error_response(status_code, message):
    """Send error response"""
    sys.stdout.write(f"Status: {status_code}\r\n")
    sys.stdout.write("Content-Type: application/json\r\n")
    sys.stdout.write("\r\n")
    sys.stdout.write(f'{{"success": false, "error": "{message}"}}\n')
    sys.stdout.flush()

def send_success_response(username, user_info, session_id, remember):
    """Send success response with session cookies"""
    # Determine cookie expiration
    max_age = 86400 if remember else 3600  # 24h if remember, 1h otherwise

    # Send headers
    sys.stdout.write("Status: 200 OK\r\n")
    sys.stdout.write("Content-Type: application/json\r\n")

    # Combine multiple cookies in one Set-Cookie header (workaround for server limitation)
    # Alternative: use document.cookie in JavaScript to set cookies client-side
    cookie_header = f"session_id={session_id}; Path=/; Max-Age={max_age}; HttpOnly, "
    cookie_header += f"username={username}; Path=/; Max-Age={max_age}, "
    cookie_header += f"user_role={user_info['role']}; Path=/; Max-Age={max_age}, "
    cookie_header += f"user_name={user_info['name'].replace(' ', '_')}; Path=/; Max-Age={max_age}, "
    cookie_header += f"login_time={int(time.time())}; Path=/; Max-Age={max_age}, "
    cookie_header += f"authenticated=true; Path=/; Max-Age={max_age}"

    sys.stdout.write(f"Set-Cookie: {cookie_header}\r\n")

    # End headers
    sys.stdout.write("\r\n")

    # Send JSON response with cookies data so JavaScript can set them
    response = {
        "success": True,
        "message": "Connexion réussie",
        "user": {
            "username": username,
            "name": user_info['name'],
            "role": user_info['role']
        },
        "session_id": session_id,
        "remember": remember,
        "max_age": max_age,
        "cookies": {
            "session_id": session_id,
            "username": username,
            "user_role": user_info['role'],
            "user_name": user_info['name'],
            "login_time": str(int(time.time())),
            "authenticated": "true"
        }
    }

    import json
    sys.stdout.write(json.dumps(response, ensure_ascii=False) + '\n')
    sys.stdout.flush()

def main():
    """Main function"""
    # Check request method
    request_method = os.environ.get('REQUEST_METHOD', 'GET')

    if request_method != 'POST':
        send_error_response("405 Method Not Allowed", "Only POST method is allowed")
        return

    # Parse POST data
    post_data = parse_post_data()

    if not post_data:
        send_error_response("400 Bad Request", "No data received")
        return

    # Get credentials
    username, password, remember = get_user_credentials(post_data)

    # Validate input
    if not username or not password:
        send_error_response("400 Bad Request", "Username and password are required")
        return

    if len(username) < 3:
        send_error_response("400 Bad Request", "Username must be at least 3 characters")
        return

    # Authenticate
    authenticated, user_info = authenticate(username, password)

    if not authenticated:
        send_error_response("401 Unauthorized", "Invalid username or password")
        return

    # Generate session ID
    session_id = generate_session_id(username)

    # Send success response
    send_success_response(username, user_info, session_id, remember)

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        # Log error (in production, log to file)
        sys.stderr.write(f"Error in login.py: {str(e)}\n")
        send_error_response("500 Internal Server Error", "An error occurred")
