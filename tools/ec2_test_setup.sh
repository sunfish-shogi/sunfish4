#!/bin/bash

source config/ec2_test

YUM_PACKAGES="git gcc-c++ cmake ruby-devel"
GEM_PACKAGES="rgl"

SHOGI_SERVER_DIR="shogi-server"

BACKUP_SCRIPT="test_backup.sh"
LOCAL_BACKUP_SCRIPT="./tools/${BACKUP_SCRIPT}"

scp -i ${EC2_SSH_KEY} ${REMOTE_BACKUP_KEY} ec2-user@${EC2_HOST}:~/.ssh/id_rsa
scp -i ${EC2_SSH_KEY} ${LOCAL_BACKUP_SCRIPT} ec2-user@${EC2_HOST}:~/${BACKUP_SCRIPT}

ssh -i ${EC2_SSH_KEY} -t -t ec2-user@${EC2_HOST} <<EOF
	chmod 600 .ssh/id_rsa

	sudo yum update -y
	sudo yum install -y ${YUM_PACKAGES}
	gem install --remote ${GEM_PACKAGES}

	git clone --branch "${SHOGI_SERVER_BRANCH}" --depth 1 "${SHOGI_SERVER_REPO}" "${SHOGI_SERVER_DIR}"
	cd ${SHOGI_SERVER_DIR}
	nohup ruby shogi-server test 4081 > stdout.log 2> stderr.log < /dev/null &
	nohup ../${BACKUP_SCRIPT} < /dev/null &
	cd ..

	exit
EOF
