#!/bin/bash -eu

source config/ec2_ln

echo EC2_HOST=${EC2_HOST}
echo EC2_SSH_KEY=${EC2_SSH_KEY}
echo KIFU_TARBALL=${LKIFU_TARBALL}
echo SUNFISH_REPO=${SUNFISH_REPO}
echo TARGET_BRANCH=${TARGET_BRANCH}
echo WORK_DIR=${WORK_DIR}

EVAL_BIN="eval.bin"
EVAL_BIN_GZ="${EVAL_BIN}.gz"
LEAN_LOG="learn.log"

DATE_FILE=date

scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/${DATE_FILE} out/${DATE_FILE}

DATE=`cat out/${DATE_FILE}`
LOCAL_BACKUP_DIR=${BACKUP_DIR}/${DATE}_${NAME}_${EC2_HOST}

ssh -i ${EC2_SSH_KEY} -t -t ec2-user@${EC2_HOST} <<EOF
	cd ${WORK_DIR}
	gzip -c ${EVAL_BIN} > ${EVAL_BIN_GZ}
	exit
EOF

mkdir -p ${LOCAL_BACKUP_DIR}

scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/${EVAL_BIN_GZ} ${LOCAL_BACKUP_DIR}/${EVAL_BIN_GZ}
scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/out/${LEAN_LOG} ${LOCAL_BACKUP_DIR}/${LEAN_LOG}
