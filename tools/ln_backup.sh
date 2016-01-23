#!/bin/bash

HOST=$1

BACKUP_DIR="ln_backup"

EVAL_BIN="eval.bin"
EVAL_BIN_GZ="${EVAL_BIN}.gz"
LEAN_LOG="learn.log"

ssh -t -t ${HOST} << EOF
	mkdir -p ln_backup
	exit
EOF

while :
do
	gzip -c ${EVAL_BIN} > ${EVAL_BIN_GZ}

	scp ${EVAL_BIN_GZ} ${HOST}:~/${BACKUP_DIR}/
	scp out/${LEAN_LOG} ${HOST}:~/${BACKUP_DIR}/

	sleep 3600
done
