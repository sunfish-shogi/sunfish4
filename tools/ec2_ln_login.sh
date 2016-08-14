#!/bin/bash

source config/ec2_ln

echo EC2_HOST=${EC2_HOST}
echo EC2_SSH_KEY=${EC2_SSH_KEY}

ssh -i ${EC2_SSH_KEY} ec2-user@${EC2_HOST}
