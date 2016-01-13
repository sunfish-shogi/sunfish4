#!/bin/bash

SUNFISH_ROOT=$(cd $(dirname $0)/..;pwd)
SHOGI_SERVER_ROOT=path/to/shogi-server

ENGINE_PATH=${SUNFISH_ROOT}/sunfish_usi
USI_TO_CSA=${SHOGI_SERVER_ROOT}/bin/usiToCsa

export GAMENAME="floodgate-600-10"
export HASH=256
export HOST="wdoor.c.u-tokyo.ac.jp"
export MARGIN_MSEC="2500"
export ID="Sunfish4"
export KEEP_ALIVE="1"
export LOG_DIR="${SUNFISH_ROOT}/out"
export PASSWORD=""
export PONDER="false"
export PORT="4081"

${ENV} ${USI_TO_CSA} ${ENGINE_PATH}
