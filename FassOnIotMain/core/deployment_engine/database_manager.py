import os
import pickle
import getpass
import json
import shutil
from collections import namedtuple

from deployer import deployer


from sqlalchemy import create_engine, MetaData, Table, select
from sqlalchemy.orm import sessionmaker
from datetime import datetime
import time

deployer_object = deployer()

retry_count = 0

last_checked_timestamp = datetime.now()


class database_manager():

    def process_initial_entries(self) -> None:

        db_url = 'postgresql://postgres:admin123@localhost:5432/fassiotdb'

        engine = create_engine(db_url)
        
        metadata = MetaData()

        table_name = Table('schema', metadata, autoload = True, autoload_with = engine)

        Session = sessionmaker(bind=engine)

        with Session() as session:
            query = session.query(table_name)
            results = query.all()

        session.close()

        global  glast_checked_timestamp 
         
        last_checked_timestamp= datetime.now()

        for row in results:
            data = deployer(row.name, row.version,row.Environment  , row.requiredPlatform  , row.run_containerized , row.dependecies  
            ,row.subsTopic   , row.Pubs_Topic  ,row.mode   , row.execCommands   ,row.waitForExit  , row.status  , row.deInternalStatus  ,row.process , row.stats)


            #################
            self.setup(data)
        
        while True:
            self.check_entries(self,last_checked_timestamp)
            time.sleep(60)
            last_checked_timestamp= datetime.now()


    def check_entries(self, last_checked_timestamp):
        db_url = 'postgresql://postgres:admin123@localhost:5432/fassiotdb'

        engine = create_engine(db_url)
        
        metadata = MetaData()

        table_name = Table('schema', metadata, autoload = True, autoload_with = engine)

        Session = sessionmaker(bind=engine)

        with session() as session:
            query = select('schema').where(table_name.c.timestamp > last_checked_timestamp)
            results = session.execute(query)
            self.process_query_results(results)
        session.close()


    def process_query_results(self, results):
        for row in results:
            data = deployer(row.name, row.version,row.Environment  , row.requiredPlatform  , row.run_containerized , row.dependecies  
            ,row.subsTopic   , row.Pubs_Topic  ,row.mode   , row.execCommands   ,row.waitForExit  , row.status  , row.deInternalStatus  ,row.process , row.stats)

            self.setup(data)
        pass