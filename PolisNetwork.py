import http.server, os

# Custom HTTPRequestHandler Class
class MyHttpHandler(http.server.BaseHTTPRequestHandler):
    # GET Command Handler
    def do_GET(self):
        try:
            print(self.path.encode())
            self.wfile.write(self.path.encode())
        except IOError:
            self.send_error(404, 'file not found')

def run():
    print('Server is starting...')

    server_address = ('192.168.1.4', 8000)
    httpd = http.server.HTTPServer(server_address, MyHttpHandler)
    print('Server is running...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()