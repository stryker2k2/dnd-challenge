# DND-CHALLENGE
This is the DND Challenge originally made for Black Lantern's Christmas Party... but is slowly being molded into the DEFCON 34 \(2026\) Newbie Village CTF Challenge.

This program here in this folder (./server) contains the Source Code and the Make File for the vulnerable DND-style Text-based game.

### Requirements
```bash
sudo apt install build-essential libc6-dev-i386 gcc-mingw-w64
```

### Find MD5 Hash
```bash
#domd5hash
```

### Wait for Program to Die
```bash
watch "netstat -pnt | grep 3724"
```

### Notes/Ideas
1. Beginner Level (Bronze)
    - Demonstrate Basic Example
2. Intermediate Level (Silver)
    - Buffer Overflow redirect to printKey()
3. Advanced Level (Gold)
    - Buffer Overflow with Custom Shellcode to pwn box
    - Advanced Key hidden is /home/user/Documents/key.txt

### TODO
- Change the story from "You attend BLS Holiday Party" to "You attend DEFCON"
- Verify that the executable operates the same way on Ubuntu 24.04 Headless Server
- Figure out a way to have a player-facing website spin up a Proxmox Ubuntu 24.04 Server from a template on demand at a click of a button so when the player is ready to test it against the actual target, all they have to do it go to 'https://box.dnd.local/getbox' and click "Spin Up Box and Give Me IP" button
- Create 'https://tutorial.dnd.local/' with a tutorial for the first Easy Target and maybe some hints for the Second and Third Target
- Hit up Micheal about making 3D Printed Challenge Coins for Bronze, Silver, Gold Boxes
