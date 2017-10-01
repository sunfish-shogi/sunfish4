#!/bin/bash -eu

source config/ec2_ln

echo EC2_HOST=${EC2_HOST}
echo EC2_SSH_KEY=${EC2_SSH_KEY}
echo KIFU_TARBALL=${KIFU_TARBALL}
echo SUNFISH_REPO=${SUNFISH_REPO}
echo TARGET_BRANCH=${TARGET_BRANCH}
echo WORK_DIR=${WORK_DIR}

KIFU_TARBALL_NAME=`basename ${KIFU_TARBALL}`

YUM_PACKAGES="git gcc-c++ cmake"

CONF_FILE_PATH="config/batch_learn.ini"
CONF_FILE_NAME=`basename ${CONF_FILE_PATH}`

REVISION_FILE=revision

DATE=`date "+%Y-%m-%d"`
DATE_FILE=date

EVAL_BIN_GZ=eval.bin.gz
EVAL_BIN=eval.bin

LOCAL_BACKUP_DIR=${BACKUP_DIR}/${DATE}_${NAME}_${EC2_HOST}

if [ -e ${LOCAL_BACKUP_DIR} ]; then
	echo "error: ${LOCAL_BACKUP_DIR} is already exists."
	exit 1
fi

mkdir -p ${LOCAL_BACKUP_DIR}
cp ${CONF_FILE_PATH} ${LOCAL_BACKUP_DIR}/

scp -i ${EC2_SSH_KEY} ${REMOTE_BACKUP_KEY} ec2-user@${EC2_HOST}:~/.ssh/id_rsa
scp -i ${EC2_SSH_KEY} ${KIFU_TARBALL} ec2-user@${EC2_HOST}:~/
scp -i ${EC2_SSH_KEY} ${CONF_FILE_PATH} ec2-user@${EC2_HOST}:~/
if [ "x${SRC_EVAL_BIN_GZ}" != "x" ]; then
	scp -i ${EC2_SSH_KEY} ${SRC_EVAL_BIN_GZ} ec2-user@${EC2_HOST}:~/${EVAL_BIN_GZ}
fi

ssh -i ${EC2_SSH_KEY} -t -t ec2-user@${EC2_HOST} <<EOF
	chmod 600 .ssh/id_rsa
	sudo yum update -y
	sudo yum install -y ${YUM_PACKAGES}
	git clone --branch "${TARGET_BRANCH}" --depth 1 "${SUNFISH_REPO}" "${WORK_DIR}"
	cd "${WORK_DIR}"
	make ln -j
	tar zxf ~/${KIFU_TARBALL_NAME}
	if [ "x${SRC_EVAL_BIN_GZ}" != "x" ]; then
		gzip -dc ~/${EVAL_BIN_GZ} > ${EVAL_BIN}
	fi
	cp ~/${CONF_FILE_NAME} ./${CONF_FILE_PATH}
	nohup ./sunfish_ln --silent > out/stdout.log 2> out/stderr.log < /dev/null &
	sleep 5
	nohup ./tools/ln_backup.sh ${REMOTE_BACKUP_HOST} ${NAME} < /dev/null &
	sleep 5
	git rev-parse HEAD > ${REVISION_FILE}
	echo ${DATE} > ${DATE_FILE}
	exit
EOF

scp -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}:~/${WORK_DIR}/${REVISION_FILE} ${LOCAL_BACKUP_DIR}/${REVISION_FILE}
