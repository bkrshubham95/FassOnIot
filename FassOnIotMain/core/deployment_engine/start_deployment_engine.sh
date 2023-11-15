de_dir=$default_dir/deployment_engine
cd $workspace_dir 
python3 -m venv venv
source venv/bin/activate
pip install -r $de_dir/requirements.txt
export IPC_FILE=$PWD/faas_on_iot
python $de_dir/deployer_main.py &> $workspace_dir/deployment_engine.log &

