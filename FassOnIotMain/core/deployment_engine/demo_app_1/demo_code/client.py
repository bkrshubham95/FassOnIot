import os
from posix_message_queue_wrapper import PosixMessageQueueWrapper

input_coefficients = ','.join(['2','5','3'])

path_name = os.getenv("IPC_FILE") #"faas_on_iot"

app_id = os.getenv("APP_ID")

# Initialising Publisher
mq_pub = PosixMessageQueueWrapper(app_id, "compute_coefficients", path_name)
if not mq_pub.registerPublisher('localhost', 8000):
	print('Could not register Publisher')
	exit()

# Initialising Subscriber
mq_sub = PosixMessageQueueWrapper(app_id, "get_roots", path_name)
if not mq_sub.registerSubscriber('localhost', 8000):
	print('Could not register Subscriber')
	exit()


mq_pub.sendMessage(input_coefficients)
recvd_msg = mq_sub.recvMessage()
print("Message: ", recvd_msg)