guards="------------------------------------------"
pre_guards="\n\n$guards\n"
post_guards="\n$guards\n\n"
export default_dir=$PWD

export binaries_dir=$HOME/faas_on_iot_binaries
export workspace_dir=$HOME/faas_on_iot_workspace
mkdir -p $binaries_dir
mkdir -p $workspace_dir

trap ctrl_c INT

function ctrl_c() {
    printf $pre_guards
    echo "Killing all services"
    cd $default_dir/api_server
    sudo docker-compose stop
    pkill -P $$
    kill `ps  | grep python | awk '{ print $1 }'`
    echo "Bye bye"
    printf $post_guards
    pkill -P $PPID
}

function move_data_xchange() {
    printf $pre_guards
    data_xchange_build_dir=data_xchange/build
    echo "Moving Data XChange"
    cp $data_xchange_build_dir/data_xchange $binaries_dir
    cp $data_xchange_build_dir/posix_message_queue_wrapper* $binaries_dir
    export PYTHONPATH=$binaries_dir:$PYTHONPATH
    echo "Done"
    printf $post_guards
}

function move_fit() {
    printf $pre_guards
    fit_dir=cli/fit
    echo "Moving fit"
    cp $fit_dir/fit $binaries_dir
    echo "Done"
    printf $post_guards
}

function move_api_server() {
    printf $pre_guards
    api_server_build_dir=api_server/target
    echo "Moving API Server"
    cp $api_server_build_dir/api_server*jar $binaries_dir
    echo "Done"
    printf $post_guards

}

function start_data_xchange() {
    printf $pre_guards
    echo "Starting Data XChange"
    cd $workspace_dir
    touch faas_on_iot
    $binaries_dir/data_xchange &> data_xchange.log &
    cd $default_dir
    echo "Done"
    printf $post_guards
}

function start_api_server() {
    printf $pre_guards
    echo "Starting API Server"
    cd $workspace_dir
    java -jar $binaries_dir/api_server*jar &> api_server.log &
    cd $default_dir
    echo "Done"
    printf $post_guards
}

function start_deployment_engine() {
    printf $pre_guards
    echo "Starting Deployment Engine"
    bash $default_dir/deployment_engine/start_deployment_engine.sh
    cd $default_dir
    echo "Done"
    printf $post_guards
}

function start_database() {
    printf $pre_guards
    echo "Starting database"
    cd api_server
    if [ ! -d "$workspace_dir/postgres_data" ]; then
        mkdir -p $workspace_dir/postgres_data
        sudo docker-compose down
    fi
    sudo docker-compose up -d
    cd $default_dir
    echo "Done"
    printf $post_guards
}

function start_ui() {
    printf $pre_guards
    echo "Starting UI"
    cd ui
    npm install
    node app.js &> $workspace_dir/ui.log &
    cd $default_dir
    echo "Done"
    printf $post_guards
}





move_data_xchange
move_fit
move_api_server
start_database
start_api_server
sleep 5
start_data_xchange
start_deployment_engine
start_ui

echo "Ctrl+C to stop"
for job in `jobs -p`
do
    wait $job
done
