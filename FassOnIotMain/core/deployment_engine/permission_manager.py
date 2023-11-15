from filelock import FileLock, Timeout
import logging

class permission_manager():

    def __init__(self, app_id):

        self.app_id = app_id
        self.file_name = "pub_sub_details"

    def app_pubsub(self, sub_list, pub_list):

        file = self.file_name

        lockfile = file + ".lock"
        lock = FileLock(lockfile)

        lock.acquire()
        try:
            with open(file, "a") as f:

                ## printing app_id
                line=self.app_id+":"
                f.write(line)

                ## printing all elements in the pub_list 
                j=0
                for i in pub_list:
                    if j < len(pub_list)-1:
                        f.write(i) 
                        f.write(",")
                        j+=1
                    else:
                        f.write(i)

                f.write(":")

                ## print all elements in the sub_list
                j=0
                for i in sub_list:
                    if j < len(sub_list)-1:
                        f.write(i) 
                        f.write(",")
                        j+=1
                    else:
                        f.write(i)

                f.write("\n")

        except Exception as e:
            logging.exception(e)

        finally:
            lock.release()


