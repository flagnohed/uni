import socket as s
import sys
from tcpparser import change_contentlenght, replace_word
import threading

RECV_SIZE = 4096
DEST_PORT = 80
SRC_PORT = 1234
SRC_IP = '127.0.0.1'
UNICODE = 'utf-8'

def start_proxy():
    server = s.socket(s.AF_INET,s.SOCK_STREAM)
    server.setsockopt(s.SOL_SOCKET,s.SO_REUSEADDR,1)
    addr = (SRC_IP,SRC_PORT)
    try:
        server.bind(addr)
    except:
        print(f'Could not bind to host: {SRC_IP}, port: {SRC_PORT}')
        sys.exit(0)
    print(f'Could bind to host: {SRC_IP}, port: {SRC_PORT}')
    server.listen(5)
    
    print('listening...')

    thread_handler(server,addr)

def thread_handler(server,addr):
    print(f'Recieved connection from {addr[0], addr[1]}')

    while True:
        conn,adr = server.accept()
        data = conn.recv(RECV_SIZE).decode(UNICODE,errors='ignore')
        try:
            threading.Thread(target=header_handler, args=(conn,data)).start()
        except Exception:
            print('Threading error')
            break
    server.close()
   

def header_handler(conn,data):
    t = data.split('\r\n\r\n',1)[0]
    head = t.split('\r\n',1)[1]
    header_dict = dict(a.split(': ') for a in head.split('\r\n'))
    
    proxy_handler(header_dict,conn,data)

def proxy_handler(head,conn,data):
    client = s.socket(s.AF_INET,s.SOCK_STREAM)
    client.connect((head['Host'],DEST_PORT))

    client.send(bytes(data,UNICODE))

    first_response = True

    while True:
        resp = client.recv(RECV_SIZE)
        decoded_resp = resp.decode(UNICODE,errors='ignore')
        if first_response:
            if decoded_resp.lower().find('content-type: image') == -1:
                if decoded_resp.lower().find('content-encoding: gzip') == -1:
                    changed_words = replace_word(decoded_resp)
                    adjust_length = change_contentlenght(changed_words)
                    resp = bytes(adjust_length,UNICODE)
            first_response = False        
        if len(resp) > 0:
            conn.send(resp)
        else:
            break
    client.close()
    conn.close()
   

if __name__ == '__main__':
    start_proxy()





