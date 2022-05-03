

This README details how to work the modules.
for my_process.c
To call the module you must first load it into the Kernel you do this by calling make on it
Once that is done do sudo insmod ./my_process.ko myint=$(id -u <username>)
Look at the Kernel messages with dmesg and you will see the output. 

for module_wss.c
To work the module first load it with
sudo insmod module_wss.ko pid=[Your pid to test]
Then it will print to the dmesg every 5 seconds.
