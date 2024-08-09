from json import loads, dumps
from kafka import KafkaProducer
import sys

producer = KafkaProducer(
    bootstrap_servers = ['localhost:9092'],
    value_serializer = lambda x:dumps(x).encode('utf-8')
)

comments = sys.argv[1]
likes = sys.argv[2]
eve = sys.argv[3]

for line in sys.stdin:
    message = {'EOF':'EOF'}
    if 'EOF' in line:
        producer.send(comments, value=message)
        producer.send(likes, value=message)
        producer.send(eve, value=message)
    words = line
    if line[:7] == "comment":
        print("Comment")
        message = {'comment': line[8:]}
        producer.send(comments, value=message)
        
    elif line[:4] == "like":
        print("Like")
        message = {'like': line[5:]}
        producer.send(likes, value=message)
    else:
        print("Share")
        message = {'share': line[6:]}
    producer.send(eve, value=message)

producer.flush()
producer.close()