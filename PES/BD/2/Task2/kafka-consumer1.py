from kafka import KafkaConsumer
from json import loads, dumps
import sys

topicName = sys.argv[1]

consumer = KafkaConsumer(
    topicName,
    bootstrap_servers = ['localhost:9092'],
    value_deserializer = lambda x: loads(x.decode('utf-8'))
)

output_dict = {}

for message in consumer:
    #print("Got Message", message)
    message = message.value
    if 'EOF' in message.keys():
        break
    message = message['comment']
    message = message.split(" ")
    comment = message[3:]
    if message[1] not in output_dict.keys():
        output_dict[message[1]] = []
    comment = ' '.join(comment).strip().lstrip('"').rstrip('"')
    output_dict[message[1]].append(comment)

output_dict = dict(sorted(output_dict.items()))
print(dumps(output_dict, indent=4))

