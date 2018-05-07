#!/bin/bash -eu

source config/ec2_ln

echo EC2_HOST=${EC2_HOST}
echo EC2_SSH_KEY=${EC2_SSH_KEY}
echo WORK_DIR=${WORK_DIR}

EVAL_EX_BIN="eval-ex.bin"
EVAL_EX_BIN_GZ="${EVAL_EX_BIN}.gz"
LEAN_LOG="learn.log"

DATE_FILE=date

scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/${DATE_FILE} out/${DATE_FILE}

DATE=`cat out/${DATE_FILE}`
LOCAL_BACKUP_DIR=${BACKUP_DIR}/${DATE}_${NAME}_${EC2_HOST}

ssh -i ${EC2_SSH_KEY} -t -t ec2-user@${EC2_HOST} <<EOF
	#!/bin/bash -eu
	cd ${WORK_DIR}
	if [ -f ${EVAL_EX_BIN} ]; then
		gzip -c ${EVAL_EX_BIN} > ${EVAL_EX_BIN_GZ}
	else
		echo "not exists: ${EVAL_EX_BIN}"
	fi
	exit
EOF

mkdir -p ${LOCAL_BACKUP_DIR}

scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/${EVAL_EX_BIN_GZ} ${LOCAL_BACKUP_DIR}/${EVAL_EX_BIN_GZ}
scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/out/${LEAN_LOG} ${LOCAL_BACKUP_DIR}/${LEAN_LOG}
