#!/bin/bash

source config/ec2rc

echo EC2_HOST=${EC2_HOST}
echo SSH_KEY=${SSH_KEY}

ssh -i ${SSH_KEY} ec2-user@${EC2_HOST}
