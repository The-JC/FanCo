from flask import Flask, request
from flask_restful import Resource, Api, reqparse
from json import dumps
from flask_jsonpify import jsonify
import smbus

import random
import time

NUMFANS = 18
addresses = [0x04]
bus = smbus.SMBus(1)

app = Flask(__name__)
api = Api(app)

class Fans(Resource):
	def get(self):
		data= [0] * NUMFANS

		# read back data of each fan 
		for i, address in enumerate(addresses):
			input_data = readData(address)

			if input_data < 3000:
				data[i] = input_data
			else:
				print("Error: on fan id" + str(i))

		return jsonify(data)

class Fan(Resource):
	def put(self, fanid):
		parser = reqparse.RequestParser()
		parser.add_argument("set_speed", type=int, location='json', required=True)
		
		args = parser.parse_args()

		print("Set speed of " + str(fanid) + " to " + str(args['set_speed']) + "RPM")

		set_speed = args['set_speed']

		# FanId bigger then number of fans or under 0
		if fanid >= NUMFANS or fanid < 0:
			print('Id can\'t be found')
			return jsonify({'status': 'fail', 'error': 'Id can\'t be found'}), 404
		
		# Check fanid is contained in i2c address list
		if fanid >= len(addresses):
			print('no Fan addres found')
			return jsonify({'status': 'fail', 'error': 'no Fan addres found'}), 501

		sendData(addresses[fanid], set_speed)

		return jsonify({'status': 'ok'})

def readData(address):
	try:
		block = bus.read_i2c_block_data(address, 0, 3)
	except IOError:
		print("Failed reading address " + str(address))
		return
	
	# join high & low byte together
	read_buffer = 0
	read_buffer = block[0]
	read_buffer = (read_buffer << 8) + block[1]
	time.sleep(0.1)
	return read_buffer

def sendData(address, data):
	buffer = [0,0]

	#Split data into high & low byte
	lowByte = (data >> 8) & 0xFF
	highByte = data & 0xFF
	buffer[0] = lowByte
	buffer[1] = highByte
	
	bus.write_i2c_block_data(address, 0, buffer)
	time.sleep(0.1)

api.add_resource(Fans, '/api/fans', endpoint="fans")
api.add_resource(Fan, '/api/fans/<int:fanid>', endpoint="fan")

if __name__ == '__main__':
	app.run(port='5000', debug=True)