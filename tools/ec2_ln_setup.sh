#!/bin/bash

source config/ec2rc

echo EC2_HOST=${EC2_HOST}
echo SSH_KEY=${SSH_KEY}
echo KIFU_TARBALL=${KIFU_TARBALL}
echo SUNFISH_REPO=${SUNFISH_REPO}
echo TARGET_BRANCH=${TARGET_BRANCH}
echo REMOTE_WORK_DIR=${REMOTE_WORK_DIR}

KIFU_TARBALL_NAME=`basename ${KIFU_TARBALL}`

YUM_PACKAGES="git gcc-c++ cmake"

CONF_FILE_PATH="config/batch_learn.ini"
CONF_FILE_NAME=`basename ${CONF_FILE_PATH}`

LOCAL_BACKUP_DIR=${BACKUP_DIR}/${EC2_HOST}

mkdir -p ${LOCAL_BACKUP_DIR}
cp ${CONF_FILE_PATH} ${LOCAL_BACKUP_DIR}/

scp -i ${SSH_KEY} ${KIFU_TARBALL} ec2-user@${EC2_HOST}:~/
scp -i ${SSH_KEY} ${CONF_FILE_PATH} ec2-user@${EC2_HOST}:~/

ssh -i ${SSH_KEY} -t -t ec2-user@${EC2_HOST} <<EOF
	sudo yum update -y
	sudo yum install -y ${YUM_PACKAGES}
	git clone --branch "${TARGET_BRANCH}" --depth 1 "${SUNFISH_REPO}" "${REMOTE_WORK_DIR}"
	cd "${REMOTE_WORK_DIR}"
	make ln -j
	tar zxf ~/${KIFU_TARBALL_NAME}
	cp ~/${CONF_FILE_NAME} ./${CONF_FILE_PATH}
	./sunfish_ln > /dev/null 2>&1 &
	exit
EOF
