#!/usr/bin/env python3
import sys
import os

def send_logout_response():
    """Send logout response and clear all session cookies"""
    # Send headers
    sys.stdout.write("Status: 200 OK\r\n")
    sys.stdout.write("Content-Type: application/json\r\n")

    # Clear all session cookies by setting Max-Age=0
    cookies_to_clear = [
        'session_id',
        'username',
        'user_role',
        'user_name',
        'login_time',
        'authenticated'
    ]

    for cookie_name in cookies_to_clear:
        sys.stdout.write(f"Set-Cookie: {cookie_name}=; Path=/; Max-Age=0\r\n")

    # End headers
    sys.stdout.write("\r\n")

    # Send JSON response
    response = '{"success": true, "message": "Déconnexion réussie"}\n'
    sys.stdout.write(response)
    sys.stdout.flush()

def main():
    """Main function"""
    # Check request method (accept both GET and POST for flexibility)
    request_method = os.environ.get('REQUEST_METHOD', 'GET')

    if request_method not in ['GET', 'POST']:
        sys.stdout.write("Status: 405 Method Not Allowed\r\n")
        sys.stdout.write("Content-Type: application/json\r\n")
        sys.stdout.write("\r\n")
        sys.stdout.write('{"success": false, "error": "Method not allowed"}\n')
        return

    # Send logout response
    send_logout_response()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        # Log error
        sys.stderr.write(f"Error in logout.py: {str(e)}\n")
        sys.stdout.write("Status: 500 Internal Server Error\r\n")
        sys.stdout.write("Content-Type: application/json\r\n")
        sys.stdout.write("\r\n")
        sys.stdout.write('{"success": false, "error": "Internal server error"}\n')
