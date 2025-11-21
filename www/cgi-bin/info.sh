#!/bin/bash
# IMPORTANT: CGI headers must end with \r\n
printf "Content-Type: text/html\r\n"
printf "\r\n"

cat << 'HTML'
<!DOCTYPE html>
<html>
<head>
    <title>CGI Environment - Shell Script</title>
    <style>
        body {
            font-family: 'Courier New', monospace;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(0,0,0,0.3);
            padding: 30px;
            border-radius: 15px;
            backdrop-filter: blur(10px);
        }
        h1 {
            color: #fff;
            border-bottom: 3px solid #4CAF50;
            padding-bottom: 15px;
            font-size: 2.5em;
        }
        h2 {
            color: #4CAF50;
            margin-top: 30px;
            font-size: 1.8em;
        }
        .var-table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            background: rgba(255,255,255,0.1);
            border-radius: 10px;
            overflow: hidden;
        }
        .var-table th {
            background: rgba(76, 175, 80, 0.8);
            color: white;
            padding: 12px;
            text-align: left;
            font-weight: bold;
        }
        .var-table td {
            padding: 10px 12px;
            border-bottom: 1px solid rgba(255,255,255,0.1);
        }
        .var-table td:first-child {
            color: #4CAF50;
            font-weight: bold;
            width: 250px;
        }
        .var-table td:last-child {
            color: #fff;
            word-break: break-all;
        }
        .var-table tr:hover {
            background: rgba(255,255,255,0.1);
        }
        .badge {
            display: inline-block;
            padding: 5px 10px;
            background: #4CAF50;
            color: white;
            border-radius: 5px;
            font-size: 0.9em;
            margin-left: 10px;
        }
        .empty {
            color: #999;
            font-style: italic;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>💻 CGI Environment Variables <span class="badge">Shell Script</span></h1>

        <h2>🌐 Request Information</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

# Request variables
echo "<tr><td>REQUEST_METHOD</td><td>${REQUEST_METHOD:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>REQUEST_URI</td><td>${REQUEST_URI:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>QUERY_STRING</td><td>${QUERY_STRING:-<span class='empty'>empty</span>}</td></tr>"
echo "<tr><td>CONTENT_TYPE</td><td>${CONTENT_TYPE:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>CONTENT_LENGTH</td><td>${CONTENT_LENGTH:-<span class='empty'>not set</span>}</td></tr>"

cat << 'HTML'
        </table>

        <h2>🖥️ Server Information</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

echo "<tr><td>SERVER_SOFTWARE</td><td>${SERVER_SOFTWARE:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>SERVER_NAME</td><td>${SERVER_NAME:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>SERVER_PORT</td><td>${SERVER_PORT:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>SERVER_PROTOCOL</td><td>${SERVER_PROTOCOL:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>GATEWAY_INTERFACE</td><td>${GATEWAY_INTERFACE:-<span class='empty'>not set</span>}</td></tr>"

cat << 'HTML'
        </table>

        <h2>📁 Script Information</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

echo "<tr><td>SCRIPT_NAME</td><td>${SCRIPT_NAME:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>SCRIPT_FILENAME</td><td>${SCRIPT_FILENAME:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>PATH_INFO</td><td>${PATH_INFO:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>PATH_TRANSLATED</td><td>${PATH_TRANSLATED:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>DOCUMENT_ROOT</td><td>${DOCUMENT_ROOT:-<span class='empty'>not set</span>}</td></tr>"

cat << 'HTML'
        </table>

        <h2>👤 Client Information</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

echo "<tr><td>REMOTE_ADDR</td><td>${REMOTE_ADDR:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>REMOTE_HOST</td><td>${REMOTE_HOST:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>REMOTE_PORT</td><td>${REMOTE_PORT:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>REMOTE_USER</td><td>${REMOTE_USER:-<span class='empty'>not set</span>}</td></tr>"

cat << 'HTML'
        </table>

        <h2>📨 HTTP Headers</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

echo "<tr><td>HTTP_HOST</td><td>${HTTP_HOST:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_USER_AGENT</td><td>${HTTP_USER_AGENT:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_ACCEPT</td><td>${HTTP_ACCEPT:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_ACCEPT_LANGUAGE</td><td>${HTTP_ACCEPT_LANGUAGE:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_ACCEPT_ENCODING</td><td>${HTTP_ACCEPT_ENCODING:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_COOKIE</td><td>${HTTP_COOKIE:-<span class='empty'>not set</span>}</td></tr>"
echo "<tr><td>HTTP_REFERER</td><td>${HTTP_REFERER:-<span class='empty'>not set</span>}</td></tr>"

cat << 'HTML'
        </table>

        <h2>⚙️ System Information</h2>
        <table class="var-table">
            <tr>
                <th>Information</th>
                <th>Value</th>
            </tr>
HTML

echo "<tr><td>Current Date/Time</td><td>$(date '+%Y-%m-%d %H:%M:%S %Z')</td></tr>"
echo "<tr><td>Server Uptime</td><td>$(uptime -p 2>/dev/null || uptime)</td></tr>"
echo "<tr><td>Current User</td><td>$(whoami)</td></tr>"
echo "<tr><td>Shell</td><td>$SHELL</td></tr>"
echo "<tr><td>Working Directory</td><td>$(pwd)</td></tr>"
echo "<tr><td>Process ID</td><td>$$</td></tr>"

cat << 'HTML'
        </table>

        <h2>🔍 All Environment Variables</h2>
        <table class="var-table">
            <tr>
                <th>Variable</th>
                <th>Value</th>
            </tr>
HTML

# Display all environment variables
env | sort | while IFS='=' read -r key value; do
    echo "<tr><td>$key</td><td>$value</td></tr>"
done

cat << 'HTML'
        </table>

        <div style="text-align: center; margin-top: 40px; padding: 20px; background: rgba(255,255,255,0.1); border-radius: 10px;">
            <p style="font-size: 0.9em; color: #ccc;">
                🐚 CGI Script executed with Bash<br>
                📊 Displaying all CGI/1.1 environment variables<br>
                🔄 <a href="javascript:location.reload()" style="color: #4CAF50;">Refresh Page</a>
            </p>
        </div>
    </div>
</body>
</html>
HTML
