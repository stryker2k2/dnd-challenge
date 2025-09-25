# DND-CHALLENGE
This is the DND Challenge originally made for Black Lantern's Christmas Party... but is slowly being molded into the DEFCON 34 \(2026\) Newbie Village CTF Challenge.

This over-arching Git Repo has the following folders:
- client (Python Script with the Answers)
- examples (Native C example of Overflow using memcpy & strcpy)
- server (Source Code and Make File for the Vulnerable DND CTF game)


### Compilation & Usage
```bash
# TERMINAL 1
cd ./server
make clean
make linux
./output

# TERMINAL 2
ncat localhost 3724
```


### Requirements
1. Linux Distro (https://ubuntu.com/)
1. ncat (`sudo apt install ncat`)
1. EDB Debugger (https://github.com/eteran/edb-debugger)
1. Ghidra SRE (https://github.com/NationalSecurityAgency/ghidra)
    - Amazon Correto (https://docs.aws.amazon.com/corretto/latest/corretto-25-ug/linux-info.html)