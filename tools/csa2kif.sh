#!/bin/bash -eu

SUNFISH_ROOT=$(cd $(dirname $0)/..;pwd)
CONV_SRC=$1
CONV_DST=$2
TOOLS_PATH=${SUNFISH_ROOT}/sunfish_tools

csa2kif () {
	SRC_PATH=$1
	DST_PATH=$2
	echo "$SRC_PATH"
	echo "    > $DST_PATH"
	$TOOLS_PATH --csa2kifu < $SRC_PATH | nkf -W -s -Lw > $DST_PATH
}

if [ -d $CONV_SRC ]; then
	for SRC_PATH in $(find $CONV_SRC -type f -name '*.csa'); do
		FILE_NAME=$(basename $SRC_PATH)
		DST_PATH=$CONV_DST
		if [ -d $DST_PATH ]; then
			DST_PATH=${DST_PATH%/}/${FILE_NAME%.*}.kif
		fi
		csa2kif $SRC_PATH $DST_PATH
	done
	exit 0
fi

if [ -d $CONV_DST ]; then
	FILE_NAME=$(basename $CONV_SRC)
	CONV_DST=${CONV_DST%/}/${FILE_NAME%.*}.kif
fi
csa2kif $CONV_SRC $CONV_DST
