import socket
import sys
import time
import numpy

import matplotlib.pyplot as plt
import array
import json 



# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('192.168.0.115', 10000)
print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)

matrix = numpy.zeros(shape=(7,500))
# Listen for incoming connections
sock.listen(1)

plt.ion()
ax = plt.gca()
ax.set_autoscale_on(True)
line1, = ax.plot(matrix[0],'r')
line2, = ax.plot(matrix[1],'b')
plt.ylabel('r=KalmanX, b=KalmanY', fontsize=18)


while True:
    # Wait for a connection
    #print >>sys.stderr, 'waiting for a connection'
    connection, client_address = sock.accept()
    try:
        #print >>sys.stderr, 'connection from', client_address

        # Receive the data in small chunks and retransmit it
        while True:
            message = connection.recv(10000)
            #print >>sys.stderr, 'received "%s"' % message
            if message==False :
                print >>sys.stderr, 'no any data', client_address
            break
    except:
	print "error occured"
    else:
	#n = json.dumps(message) 
	try:
		json_obj = json.loads(message)
		matrix[0] = numpy.roll(matrix[0],1)
		matrix[0][499] = float(json_obj["Kalman"]["kalmanX"])
		line1.set_ydata(matrix[0])
		matrix[1] = numpy.roll(matrix[1],1)
		matrix[1][499] = float(json_obj["Kalman"]["kalmanY"])
		line2.set_ydata(matrix[1])

		ax.relim()
		ax.autoscale_view(True,True,True)
		plt.draw()
	except:
		print "incorrect json string"
	


    finally:
        # Clean up the connection
        connection.close()

    













