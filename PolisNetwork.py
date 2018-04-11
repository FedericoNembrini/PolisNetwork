# example: http://localhost/?thingTag=cccccccccccc&metricTag=daaaaaaaaaaa&value=35

import http.server, os, requests, time
address = '192.168.1.30'
port = 80

def run():
    print('Server is starting...')

    server_address = (address, port)
    httpd = http.server.HTTPServer(server_address, MyHttpHandler)
    
    print('Server is running...')
    httpd.serve_forever()

# Custom HTTPRequestHandler Class
class MyHttpHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            if(self.path != '/favicon.ico'):
                handle_Data(self)
            self.wfile.write(self.path.encode())
        except IOError:
            self.send_error(404, 'File not found')

def handle_Data(self):
    get_string = self.path.split('?')[1]
    thingTag, metricTag, value = get_string.split('&')
    send_Data((thingTag.split('=')[1]), (metricTag.split('=')[1]), (value.split('=')[1]))

def send_Data(thingTag, metricTag, value):
    url = 'http://polis.inno-school.org/polis/php/api/publishMetric.php'
    payload = {'thingTag': thingTag,'metricTag': metricTag,'value': value}

    result = requests.post(url, data = payload)
    print(result.text)

if __name__ == '__main__':
    run()