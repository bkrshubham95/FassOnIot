from sqlalchemy import create_engine, Column, Integer, String, Boolean, ForeignKey, Float, TIMESTAMP, Table
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
from datetime import datetime

# Define the database URL
db_url = 'postgresql://postgres:admin123@localhost:5432/fassiotdb'

# Create a SQLAlchemy engine to connect to the database



# Create a base class for declarative models
Base = declarative_base()

# Define the process and schema and stats models


class Schema(Base):

    __tablename__ = "schema"


    id =  Column(Integer, primary_key = True)
    name    =  Column(String(30))
    version    =  Column(String(30))




    runtimeEnvironment     =  Column(String(30))
    requiredPlatform     =  Column(String(30))
    run_containerized  =  Column(Boolean, unique=False, default=True)

    dependecies     =  Column(String(30))

    subsTopic     =  Column(String(30))

    Pubs_Topic     =  Column(String(30))


    mode     =  Column(String(30))
    execCommands     =  Column(String(30))

    waitForExit     =  Column(String(30))


    status     =  Column(String(30))

    deInternalStatus     =  Column(String(30))

    timestamp = Column(TIMESTAMP(timezone=False), nullable=False,default=datetime.now())

    process = relationship('Process', back_populates='schema')

    stats = relationship('Stats', back_populates='stats')



 


class Process(Base):
    __tablename__ = "process"

    id   =  Column(Integer, primary_key = True)
    cmd     =  Column(String(30))
    status     =  Column(String(30))
    statusInfo     =  Column(String(30))

    process_id = Column(Integer, ForeignKey('schema.id'))


    schema = relationship('Schema', back_populates='process')


class Stats(Base):
    __tablename__ = "stats"

    id  =  Column(Integer, primary_key=True)
    msgPublished  =  Column(Integer)
    msgSentToOpp   =  Column(Integer)
    resourceUsage   =  Column(Float)

    stats_id = Column(Integer, ForeignKey('schema.id'))

    stats = relationship('Schema', back_populates='stats')




# Create the tables
engine = create_engine(db_url, echo=True)
statements = Base.metadata.create_all(engine)

# users_table = Table('schema', Base.metadata, autoload=True, autoload_with=engine)
# create_table_command = users_table.create().compile(engine)
# print('Here is the command you are looking for --------------------- ',create_table_command)


# Create a session to interact with the database
Session = sessionmaker(bind=engine)
session = Session()

# Insert data into the schema table
schema1 = Schema(id='1', name='first_exp', version= '1.0.0', runtimeEnvironment='venv', requiredPlatform= 'python', run_containerized=False,
                    dependecies = 'numpy, panda', subsTopic='2', Pubs_Topic='1', mode = '1', execCommands = 'ls -a', waitForExit = 'True',
                    status= 'running', deInternalStatus= 'creating folders', timestamp=datetime.now())
session.add_all([schema1])

# Insert data into the process table
process1 = Process(process_id='1', cmd = 'ls -a', status = 'running', statusInfo = 'process is running')
session.add_all([process1])


stats1 = Stats(stats_id = '1', msgPublished = '1', msgSentToOpp = '1', resourceUsage ='0.5')

session.add(stats1)

# Commit the changes to the database
session.commit()

# Close the session
session.close()




