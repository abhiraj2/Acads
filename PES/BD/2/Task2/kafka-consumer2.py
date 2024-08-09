from kafka import KafkaConsumer
from json import loads, dumps
import sys

topicName = sys.argv[2]

consumer = KafkaConsumer(
    topicName,
    bootstrap_servers = ['localhost:9092'],
    value_deserializer = lambda x: loads(x.decode('utf-8'))
)

output_dict = {}

for message in consumer:
    # print("Got Message", message)
    # print(dumps(output_dict, indent=4))
    message = message.value
    if 'EOF' in message.keys():
        break
    message = message['like']
    message = message.split(" ")
    inner_key = message[2].strip().lstrip('"').rstrip('"')
    if message[1] not in output_dict.keys() or inner_key not in output_dict[message[1]].keys():
        if message[1] not in output_dict.keys():
            output_dict[message[1]] = {}
        output_dict[message[1]][inner_key] = 0

    output_dict[message[1]][inner_key] +=1

output_dict = dict(sorted(output_dict.items()))
print(dumps(output_dict, indent=4))

