#!/bin/bash

set +x

echo "[+] Cleaning Remote Server"
ssh dev@192.168.122.216 'pkill defcon || true'
ssh dev@192.168.122.216 'rm -rf /home/dev/repo/dnd-challenge || true'

echo "[+] Copying Repo to Remote Server"
ssh dev@192.168.122.216 'mkdir /home/dev/repo/dnd-challenge'
scp -r /home/jack/repo/dnd-challenge/server dev@192.168.122.216:/home/dev/repo/dnd-challenge

echo "[+] Compiling CTF Executable"
ssh dev@192.168.122.216 'cd /home/dev/repo/dnd-challenge/server && make clean && make all'
ssh dev@192.168.122.216 'cp /home/dev/Documents/key.txt /home/dev/repo/dnd-challenge/server/output'

echo "[+] Done. Launch with 'ncat 192.168.122.216 3724'"
ssh dev@192.168.122.216 '/home/dev/repo/dnd-challenge/server/output/defcon'

set -x