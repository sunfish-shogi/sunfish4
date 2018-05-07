#!/bin/bash

HOST=$1
NAME=$2

BACKUP_DIR="ln_backup_${NAME}"

SSH_OPT="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"

EVAL_EX_BIN="eval-ex.bin"
EVAL_EX_BIN_GZ="${EVAL_EX_BIN}.gz"
LEAN_LOG="learn.log"

ssh ${SSH_OPT} -t -t ${HOST} << EOF
	mkdir -p ${BACKUP_DIR}
	exit
EOF

while :
do
	gzip -c ${EVAL_EX_BIN} > ${EVAL_EX_BIN_GZ}

	scp ${SSH_OPT} ${EVAL_EX_BIN_GZ} ${HOST}:~/${BACKUP_DIR}/
	scp ${SSH_OPT} out/${LEAN_LOG} ${HOST}:~/${BACKUP_DIR}/

	sleep 3600
done
