
guards="------------------------------------------"
pre_guards="\n\n$guards\n"
post_guards="\n$guards\n\n"
default_dir=$PWD
maven_base=`find ~ -type d -name "apache-maven*" 2>/dev/null`
export PATH=$maven_base/bin:$PATH

function build_data_xchange() {

    printf $pre_guards
    echo "Building Data XChange"
    cd data_xchange
    mkdir -p build && cd build
    cmake -j 4 -DCMAKE_BUILD_TYPE=Release -B. -S.. && make -j 4 && echo "Data XChange built"
    printf $post_guards
    cd $default_dir
}

function build_api_server() {
    printf $pre_guards
    echo "Building API Servr"
    cd api_server
    mvn clean package spring-boot:repackage -Dmaven.test.skip && echo "API Server built"
    printf $post_guards
    cd $default_dir
}

function build_fit() {
    printf $pre_guards
    echo "Building Fit"
    cd cli/fit
    go build && echo "Fit built"
    printf $post_guards
    cd $default_dir
}


build_data_xchange
build_api_server
build_fit
