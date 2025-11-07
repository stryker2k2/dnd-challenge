#!/bin/bash

target="192.168.1.101"
develop="192.168.1.102"

set +x

echo "[+] Cleaning Target Server"
ssh dev@$target 'pkill defcon || true'
ssh dev@$target 'rm -rf /home/dev/ctf/* || true'

echo "[+] Compiling CTF Executable on Develop Server"
cd /home/dev/repo/dnd-challenge/server && make clean && make all && cd ..

echo "[+] Moving CTF Executable to Target Box"
scp /home/dev/repo/dnd-challenge/server/output/defcon dev@$target:/home/dev/ctf/
scp /home/dev/repo/dnd-challenge/server/output/service dev@$target:/home/dev/ctf/

# echo "[+] Done. Launch with 'ncat $target 3724'"
# ssh dev@$target 'bash -c /home/dev/ctf/defcon'

set -x
