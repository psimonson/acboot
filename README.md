## Simple Boot Sector With Program

This is my simple boot sector written in NASM, with a program written in C. Finally
got it to load correctly and fixed my printf. Now it all works. I did NOT put a license
on this because, I want to show how to get a bare bones shell working without setting up
a bunch of stuff (e.g. IRQs, IDT, PIT, etc...). This is simplely something to get you started
in pure C. You can add the IRQs and stuff yourself.

### Disclamer

This is provided "AS-IS", without any WARRANTY of any kind. Use at your own risk.
I cannot be held accountable for what YOU do with this code or what it does to your
system. So please do NOT blaim me for your own mistakes. Also, this code will only work
on 80386 or newer. Do NOT expect it to work on a dinosaur PC/laptop.

### Known Bugs

 - After you run the 'find' command ONLY if the file is found 'help' command doesn't work any more.

### Notes

 - Finish implementing my simple custom file system table. [Partially Done]
 - Load executables (.APP extension only). [Partially Done]
 - Make a shell program launch from IO.SYS (use IO.SYS as stage2 loader). [DONE]
 - More (when I think of it).

### Screen shots (More to come)

 - ### Screen shot (02/11/2020)
![Alt text](screens/screen1_02112020.png?raw=true "What it looks like now (02/11/2020).")
 - ### Screen shot (02/15/2020)
![Alt text](screens/screen2_02152020.png?raw=true "What it looks like now (02/15/2020).")
 - ### Screen shot (02/15/2020)
![Alt text](screens/screen3_02152020.png?raw=true "What it looks like now (02/15/2020).")
 - ### Screen shot (02/15/2020)
![Alt text](screens/screen4_02152020.png?raw=true "What it looks like now (02/15/2020).")
 - ### Screen shot (02/16/2020)
![Alt text](screens/screen5_02162020.png?raw=true "What it looks like now (02/16/2020).")
 - ### Screen shot (02/16/2020)
![Alt text](screens/screen6_02162020.png?raw=true "What it looks like now (02/16/2020).")
 - ### Screen shot (02/26/2020)
![Alt text](screens/screen7_02262020.png?raw=true "What it looks like now (02/26/2020).")

### Developer

 - Philip R. Simonson (aka 5n4k3)

