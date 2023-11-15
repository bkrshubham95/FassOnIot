import os
from posix_message_queue_wrapper import PosixMessageQueueWrapper
import math

path_name = os.getenv("IPC_FILE") #"faas_on_iot"

app_id = os.getenv("APP_ID")

def compute_roots(input_coefficients = [2, 5, 3]):
    
    Coeff1 = input_coefficients[0]
    Coeff2 = input_coefficients[1]
    Coeff3 = input_coefficients[2]
    
    # Checking if roots are real or complex based on value of discriminant i.e b2-4ac
  
    discriminant = Coeff2 * Coeff2 - 4 * Coeff1 * Coeff3  
    discriminant_root = math.sqrt(abs(discriminant))  
    
    # Computing roots using formula (-b +/- root(b2-4ac))/2a
    
    # If discriminant is positive then roots are real and different
    if discriminant > 0: 

    	root_info = "Roots are real"
    	root1 = str((-Coeff2 + discriminant_root) / (2 * Coeff1))
    	root2 = str((-Coeff2 - discriminant_root) / (2 * Coeff1))
    	final_result = root_info + '\n' + root1 + '\n' + root2

    # If discriminant is zero then roots are real but same
    elif discriminant == 0:

    	root_info = "Root is real"
    	final_result = root_info + '\n' + str(-Coeff2 / (2 * Coeff1))

    # If discriminant is negative then roots are complex
    else:  

    	root_info = "Roots are Complex"
    	root1 = ' '.join([- Coeff2 / (2 * Coeff1), "+", round(discriminant_root / (2 * Coeff1), 1), "i"])
    	root2 = ' '.join([- Coeff2 / (2 * Coeff1), "-", round(discriminant_root / (2 * Coeff1), 1), "i"])
    	final_result = root_info + '\n' + root1 + '\n' + root2


    return final_result



# Initialising Publisher
mq_pub = PosixMessageQueueWrapper(app_id, "get_roots", path_name)
if not mq_pub.registerPublisher('localhost', 8000):
	print('Could not register Publisher')
	exit()

# Initialising Subscriber
mq_sub = PosixMessageQueueWrapper(app_id, "compute_coefficients", path_name)
if not mq_sub.registerSubscriber('localhost', 8000):
	print('Could not register Subscriber')
	exit()


recvd_msg = mq_sub.recvMessage()
print("Message: ", recvd_msg)
final_result = compute_roots([int(i) for i in recvd_msg.split(',')])


mq_pub.sendMessage(final_result)
