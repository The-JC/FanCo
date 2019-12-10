from flask import Flask, request
from flask_restful import Resource, Api, reqparse
from json import dumps
from flask_jsonpify import jsonify

import random

app = Flask(__name__)
api = Api(app)

class Fans(Resource):
	def get(self):
		data=[]
		for i in range(18):
			data.append(random.randint(0, 12000))
		return jsonify(data)

class Fan(Resource):
	def put(self, fanid):
		print(fanid)
		parser = reqparse.RequestParser()
		parser.add_argument("set_speed", type=int, location='json', required=True)
		
		args = parser.parse_args()

		print("Set speed of " + str(fanid) + " to " + str(args['set_speed']) + "RPM")

		return jsonify({'status': 'ok'})
		pass


api.add_resource(Fans, '/api/fans', endpoint="fans")
api.add_resource(Fan, '/api/fans/<int:fanid>', endpoint="fan")

if __name__ == '__main__':
	app.run(port='5000')