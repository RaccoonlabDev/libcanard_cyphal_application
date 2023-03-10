#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
DSDL_DIR="$SCRIPT_DIR/../Libs/public_regulated_data_types"

if [ ! -d $1 ]; then
    OUT_DIR=$1 # custom build directory
else
    OUT_DIR="$SCRIPT_DIR/../build/nunavut_out"
fi

mkdir -p $OUT_DIR
PARAMS="--target-language c --target-endianness=little -v" # without asserts yet: --enable-serialization-asserts
nnvg $PARAMS "$DSDL_DIR/reg" --lookup-dir "$DSDL_DIR/uavcan" --outdir $OUT_DIR
nnvg $PARAMS "$DSDL_DIR/uavcan" --outdir $OUT_DIR
