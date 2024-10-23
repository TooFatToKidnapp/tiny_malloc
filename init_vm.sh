#!/bin/bash

set -ex

source /vagrant/.ssh_env

apt update
apt upgrade -y
apt install make clang gcc vim curl zsh git time -y

echo -e "nameserver 8.8.8.8\nnameserver 1.1.1.1" | sudo tee /etc/resolv.conf

echo $SSH_KEY >> /home/vagrant/.ssh/authorized_keys

#sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

#chsh -s $(which zsh) vagrant
