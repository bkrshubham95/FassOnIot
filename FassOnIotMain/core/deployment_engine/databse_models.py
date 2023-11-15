from sqlalchemy import create_engine, Column, Integer, String, Boolean, ForeignKey, Float, TIMESTAMP, Table, DateTime, MetaData
from sqlalchemy.dialects.postgresql import ARRAY
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship, Session
from datetime import datetime
from sqlalchemy.ext.automap import automap_base




# Create a SQLAlchemy engine to connect to the database



# Create a base class for declarative models
Base = declarative_base()

# Define the process and schema and stats models


class Applications(Base):

    __tablename__ = "applications"


    id =  Column(Integer, primary_key = True)
    name    =  Column(String(30))
    version    =  Column(String(30))




    runtimeEnvironment     =  Column(String(30))
    requiredPlatform     =  Column(String(30))
    is_containerized  =  Column(Boolean, unique=False, default=True)

    dependencies     =  Column(String(30))

    subsTopic     =  Column("subs_topic", ARRAY(String))

    pubsTopic     =  Column("pubs_topic", ARRAY(String))

    commands = relationship('Commands', back_populates='application')


    status     =  Column(String(30))

    deployementEngineReportedStatus     =  Column(String(30))

    isProcessed = Column(Boolean, unique=False, default=False)

    created = Column(DateTime(timezone=True))

    updated = Column(DateTime(timezone=True))

    extractedPath = Column(String(30))

   



class Commands(Base):

    __tablename__ = "commands"
    id = Column(Integer, primary_key=True)

    mode = Column(String(30))

    execCommands = Column("execCommands", ARRAY(String))

    waitForExit = Column(String(30))

    application_id = Column(Integer, ForeignKey('applications.id'))







# engine = create_engine(db_url)



# Base = automap_base()

# Base.prepare(engine, reflect = True)
# print(Base.classes)

# class pubSubClass(Base):
#     __table__ = 'applicationdb_pubs_topic'
#     applicationdb_pubs_id = Column(Integer, primary_key=True)

# ApplicationsDb = Base.classes.applications

# #ApplicationsDb_pubs_topic = Base.classes.applicationsdb_pubs_topic
# session = Session(engine)

# results = session.query(ApplicationsDb).first()


# #results.pubs_topic= session.query(ApplicationsDb_pubs_topic).where(ApplicationsDb_pubs_topic.applicationdb_id == results.id).pubs_topic

# results_pubs_topic = session.query(pubSubClass).first()

# print(results_pubs_topic)


# # class MyClass(Base):
# #     __table__ = Table('applicants', Base.metadata, autoload = True, autoload_with=engine)

# # myclass = MyClass()

    
# #metadata = MetaData()

# #table_name = Table('Apllications', metadata, autoload = True, autoload_with = engine)


# # session = Session(engine)

# # results = session.query(MyClass).first()

# # print(results)

# #Session = sessionmaker(bind=engine)

# #session.query




# # with Session() as session:
# #     query = session.query(table_name)
# #     results = query.all()

# # session.close()

# # global  glast_checked_timestamp 
    
# # last_checked_timestamp= datetime.now()

# # for row in results:
# #     print(row)
#     # data = deployer(row.name, row.version,row.Environment  , row.requiredPlatform  , row.run_containerized , row.dependecies  
#     # ,row.subsTopic   , row.Pubs_Topic  ,row.mode   , row.execCommands   ,row.waitForExit  , row.status  , row.deInternalStatus  ,row.process , row.stats)

#     # self.setup(data)


# # # Create the tables
# # engine = create_engine(db_url, echo=True)
# # statements = Base.metadata.create_all(engine)

 
# # # Create a session to interact with the database
# # Session = sessionmaker(bind=engine)
# # session = Session()



# # # Commit the changes to the database
# # session.commit()

# # # Close the session
# # session.close()



# # class Process(Base):
# #     __tablename__ = "process"

# #     id   =  Column(Integer, primary_key = True)
# #     cmd     =  Column(String(30))
# #     status     =  Column(String(30))
# #     statusInfo     =  Column(String(30))

# #     process_id = Column(Integer, ForeignKey('schema.id'))


# #     schema = relationship('Schema', back_populates='process')


# # class Stats(Base):
# #     __tablename__ = "stats"

# #     id  =  Column(Integer, primary_key=True)
# #     msgPublished  =  Column(Integer)
# #     msgSentToOpp   =  Column(Integer)
# #     resourceUsage   =  Column(Float)

# #     stats_id = Column(Integer, ForeignKey('schema.id'))

# #     stats = relationship('Schema', back_populates='stats')






# # # users_table = Table('schema', Base.metadata, autoload=True, autoload_with=engine)
# # # create_table_command = users_table.create().compile(engine)
# # # print('Here is the command you are looking for --------------------- ',create_table_command)


# # # Create a session to interact with the database
# # Session = sessionmaker(bind=engine)
# # session = Session()

# # # Insert data into the schema table
# # schema1 = Schema(id='1', name='first_exp', version= '1.0.0', runtimeEnvironment='venv', requiredPlatform= 'python', run_containerized=False,
# #                     dependecies = 'numpy, panda', subsTopic='2', Pubs_Topic='1', mode = '1', execCommands = 'ls -a', waitForExit = 'True',
# #                     status= 'running', deInternalStatus= 'creating folders', timestamp=datetime.now())
# # session.add_all([schema1])

# # # Insert data into the process table
# # process1 = Process(process_id='1', cmd = 'ls -a', status = 'running', statusInfo = 'process is running')
# # session.add_all([process1])


# # stats1 = Stats(stats_id = '1', msgPublished = '1', msgSentToOpp = '1', resourceUsage ='0.5')

# # session.add(stats1)

# # # Commit the changes to the database
# # session.commit()

# # # Close the session
# # session.close()




