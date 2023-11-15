# Directory for the non-IoT components of the project

# Dependencies
```
sudo apt install openjdk-17-jdk
sudo apt-get install     ca-certificates     curl     gnupg
sudo rm /etc/apt/sources.list.d/docker.list
sudo curl -sL https://get.docker.com | sed 's/9)/10)/' | sh
pip3 install docker-compose
sudo apt install libprotobuf-dev cmake
wget https://go.dev/dl/go1.20.3.linux-armv6l.tar.gz
sudo rm -rf /usr/local/go && sudo tar -C /usr/local -xzf go1.20.3.linux-armv6l.tar.gz
sudo apt install libyaml-cpp-dev
sudo apt install protobuf-compiler # Based on protoc version proto file may need to be recompiled
sudo apt install npm
sudo apt install libpq-dev
# Home directory
wget https://dlcdn.apache.org/maven/maven-3/3.9.1/binaries/apache-maven-3.9.1-bin.tar.gz
tar -xvf apache-maven-3.9.1-bin.tar.gz
```

# Build
`bash build_core.sh # Inside core directory`

# Start all components
`bash deploy_core.sh # Inside core directory`  

- UI should be accessible on port 3000 and API Server on 8082
