import socket  
import struct
import time
import threading

Msg = '''  "media" : [
      {
         "attributes" : {
            "candidate" : [
               {
                  "component" : 1,
                  "foundation" : 1,
                  "generation" : 1,
                  "ip" : "60.28.219.137",
                  "name" : "rtcp",
                  "network_name" : "Realtek PCIe GBE Family Controller ",
                  "password" : "uUJUtOgFkmZ1ynzp",
                  "port" : "8080",
                  "priority" : "0.5",
                  "proto" : "udp",
                  "type" : "relay",
                  "username" : "Ah7Y8xQN+V2TWMkj"
               },
               {
                  "component" : 1,
                  "foundation" : 1,
                  "generation" : 1,
                  "ip" : "60.28.219.137",
                  "name" : "rtcp",
                  "network_name" : "Realtek PCIe GBE Family Controller - ",
                  "password" : "uUJUtOgFkmZ1ynzp",
                  "port" : "8081",
                  "priority" : "0.5",
                  "proto" : "udp",
                  "type" : "relay",
                  "username" : "Ah7Y8xQN+V2TWMkj"
               }
            ]
         },
         "label" : 1,
         "rtcp_mux" : true,
         "rtpmap" : [
            {
               "103" : {
                  "clockrate" : 16000,
                  "codec" : "audio/ISAC"
               }
            },
            {
               "104" : {
                  "clockrate" : 32000,
                  "codec" : "audio/ISAC"
               }
            },
            {
               "9" : {
                  "clockrate" : 16000,
                  "codec" : "audio/G722"
               }
            },
            {
               "102" : {
                  "clockrate" : 8000,
                  "codec" : "audio/ILBC"
               }
            },
            {
               "0" : {
                  "clockrate" : 8000,
                  "codec" : "audio/PCMU"
               }
            },
            {
               "8" : {
                  "clockrate" : 8000,
                  "codec" : "audio/PCMA"
               }
            },
            {
               "106" : {
                  "clockrate" : 32000,
                  "codec" : "audio/CN"
               }
            },
            {
               "105" : {
                  "clockrate" : 16000,
                  "codec" : "audio/CN"
               }
            },
            {
               "13" : {
                  "clockrate" : 8000,
                  "codec" : "audio/CN"
               }
            },
            {
               "127" : {
                  "clockrate" : 8000,
                  "codec" : "audio/red"
               }
            },
            {
               "126" : {
                  "clockrate" : 8000,
                  "codec" : "audio/telephone-event"
               }
            }
         ]
      },
      {
         "attributes" : null,
         "label" : 2,
         "rtcp_mux" : true,
         "rtpmap" : [
            {
               "100" : {
                  "codec" : "video/VP8"
               }
            },
            {
               "101" : {
                  "codec" : "video/red"
               }
            },
            {
               "102" : {
                  "codec" : "video/ulpfec"
               }
            }
         ]
      }
   ]
}'''

def testFunc():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    sock.connect(('localhost', 6666))  
    #time.sleep(2)  
    ftm = 'i'
    ftm += str(len(Msg))
    ftm += 's'
    data = struct.pack(ftm, len(Msg), Msg)
    for i in range(1):
        #sock.send(data)  
        print sock.recv(12)  
    #time.sleep(2)  
    sock.close()  

def threadTest():
    #for i in range(30):
        t = threading.Thread(target=testFunc)
        t.start()

if __name__ == '__main__':
    threadTest()
