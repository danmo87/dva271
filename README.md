# Description:
Simple example for obtaining (GATT) heart rate from the Zephyr bluetooth/peripheral sample (as of nRF toolchain 2.9)

# Installation:
Create a venv through 
$ python3 -m venv btvent 
and activate it through
$ source btvent/bin/activate

Now install bleak using pip
$ pip install bleak

# How to run:
Modify the peripheral sample's kconfig variable CONFIG_BT_PRIVACY=y to CONFIG_BT_PRIVACY=n in order to keep the same address.
Change the "address" variable in mybletest.py to the address of your peripheral device.

Make sure bluetooth is turned on:
$ sudo systemctl start bluetooth
$ bluetoothctl power on OR $ btmgmt power on

Now run it with $ python mybletest.py
