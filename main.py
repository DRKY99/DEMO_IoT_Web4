from http.server import BaseHTTPRequestHandler, HTTPServer
import sqlite3
import json
from sys import argv

DATABASE = "main.db"
port = 80
value = "default"
initial_distance = 0


class Server(BaseHTTPRequestHandler):
    def do_GET(self):
        print(self.path)
        # execute query
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()
        response_json = []
        data = cursor.execute(f"SELECT * FROM record")
        for row in data:
            response_json.append({
                "id": row[0],
                "value": row[1],
                "distance": row[2],
                "datetime": row[3],
            })
        conn.commit()
        conn.close()
        # set response
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        data = json.dumps(response_json)
        self.wfile.write(data.encode('utf-8'))

    def do_POST(self):
        # execure query
        conn = sqlite3.connect(DATABASE)
        cursor = conn.cursor()
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        body = json.loads(post_data.decode('utf-8'))
        distance = float(body["distance"])
        print(distance, initial_distance)
        distance = distance + initial_distance
        # print(type(distance), type(initial_distance))
        cursor.execute(
            f"INSERT INTO record values(NULL,\'{value}\',\'{distance}\',DATETIME())")
        conn.commit()
        conn.close()
        # set response
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write("{\"message\":\"success\"}".encode('utf-8'))


if __name__ == '__main__':
    if len(argv) == 4:
        value = str(argv[1])
        initial_distance = float(argv[2])
        port = int(argv[3])

    if len(argv) == 3:
        value = str(argv[1])
        initial_distance = float(argv[2])

    server_address = ('', port)
    httpd = HTTPServer(server_address, Server)
    print(f"Starting http server on port {port}...\n")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print('Stopping http server...\n')
