#!/bin/bash

target="192.168.1.101"
develop="192.168.1.102"

set +x

echo "[+] Cleaning Remote Server"
ssh dev@$virtmgr 'pkill defcon || true'
ssh dev@$virtmgr 'rm -rf /home/dev/repo/dnd-challenge || true'

echo "[+] Copying Repo to Remote Server"
ssh dev@$virtmgr 'mkdir /home/dev/repo/dnd-challenge'
scp -r /home/jack/repo/dnd-challenge/server dev@$virtmgr:/home/dev/repo/dnd-challenge

echo "[+] Compiling CTF Executable"
ssh dev@$virtmgr 'cd /home/dev/repo/dnd-challenge/server && make clean && make all'

echo "[+] Moving CTF Executable to Target Box"
scp dev@$virtmgr:/home/dev/repo/dnd-challenge/server/output/defcon dev@$proxmox:/home/dev/

echo "[+] Done. Launch with 'ncat $proxmox 3724'"
ssh dev@$proxmox 'bash -c /home/dev/defcon'

set -x
