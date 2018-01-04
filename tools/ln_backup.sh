#!/bin/bash

HOST=$1
NAME=$2

BACKUP_DIR="ln_backup_${NAME}"

SSH_OPT="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"

BIN_FILES="eval.bin fv.bin ag.bin"
LEARN_LOG="learn.log"

TMP_DIR="temp"

mkdir -p ${TMP_DIR}

ssh ${SSH_OPT} -t -t ${HOST} << EOF
        mkdir -p ${BACKUP_DIR}
        exit
EOF

while :
do
        for FILE in ${BIN_FILES}; do
                cp ${FILE} ${TMP_DIR}/${FILE}
                if [ `cat ${FILE} | wc -c` -eq `cat ${TMP_DIR}/${FILE} | wc -c` ]; then
                        GZ="${TMP_DIR}/${FILE}.gz"
                        gzip -c ${TMP_DIR}/${FILE} > ${GZ}
                        scp ${SSH_OPT} ${GZ} ${HOST}:~/${BACKUP_DIR}/
                else
                        echo "file size not matched: ${FILE}"
                fi
        done

        scp ${SSH_OPT} out/${LEARN_LOG} ${HOST}:~/${BACKUP_DIR}/

        sleep 3600
done
