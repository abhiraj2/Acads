from kafka import KafkaConsumer
from json import loads, dumps
import sys

topicName = sys.argv[3]

consumer = KafkaConsumer(
    topicName,
    bootstrap_servers = ['localhost:9092'],
    value_deserializer = lambda x: loads(x.decode('utf-8'))
)

output_dict = {}
for message in consumer:
    #print("Got Message", message)
    #print(dumps(output_dict, indent=4))
    message = message.value
    shared = 0
    if 'EOF' in message.keys():
        break
    if 'like' in message.keys():
        message = message['like']
        inner_key = 'like'
        
    elif 'comment' in message.keys():
        message = message['comment']
        inner_key = 'comment'
    else:
        message = message['share']
        inner_key = 'share'
    
    message = message.strip()
    message = message.split(" ")
    shared = len(message[3:])
    if message[1] not in output_dict.keys():
        output_dict[message[1]] = 0
    if inner_key == 'like':
        output_dict[message[1]] += 1
    elif inner_key == 'comment':
        output_dict[message[1]] += 5
    else:
        output_dict[message[1]] += (20*shared)

#print(dumps(output_dict, indent=4))
for i in output_dict.keys():
    output_dict[i] /=1000    
output_dict = dict(sorted(output_dict.items()))
print(dumps(output_dict, indent=4))
