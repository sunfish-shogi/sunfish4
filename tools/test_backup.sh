#!/bin/bash

HOST=$1
NAME=$2

BACKUP_DIR="test_backup_${NAME}"

SSH_OPT="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"

MK_GAME_RESULTS="./mk_game_results"
TARGET_DIR="20*"
GAME_RESULTS="game_results"

ssh ${SSH_OPT} -t -t ${HOST} << EOF
	mkdir -p ${BACKUP_DIR}
	exit
EOF

while :
do
	${MK_GAME_RESULTS} ${TARGET_DIR} > ${GAME_RESULTS}

	scp ${SSH_OPT} ${GAME_RESULTS} ${HOST}:~/${BACKUP_DIR}/

	sleep 600
done
