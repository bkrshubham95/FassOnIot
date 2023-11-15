from posix_message_queue_wrapper import PosixMessageQueueWrapper

pathname = "faas_on_iot"

pos1 = PosixMessageQueueWrapper("101", "topic1", pathname)
pos2 = PosixMessageQueueWrapper("102", "topic1", pathname)


result2 = pos2.registerSubscriber('localhost', 8000)
print(result2)
result1 = pos1.registerPublisher('localhost', 8000)
print(result1)

pos1.sendMessage('Hello')
rcvd_msg = pos2.recvMessage()
print(rcvd_msg)
