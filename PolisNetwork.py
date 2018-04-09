import http.server, os

# Custom HTTPRequestHandler Class
class MyHttpHandler(http.server.BaseHTTPRequestHandler):
    # GET Command Handler
    def do_GET(self):
        try:
            handleData(self)
        except IOError:
            self.send_error(404, 'file not found')

def run():
    print('Server is starting...')

    server_address = ('192.168.1.4', 80)
    httpd = http.server.HTTPServer(server_address, MyHttpHandler)
    print('Server is running...')
    httpd.serve_forever()

def handleData(self):
    self.wfile.write(self.path.encode())
    get_string = self.path.decode("utf-8")
    get_string = get_string.split('?')
    print(get_string)
    


if __name__ == '__main__':
    run()