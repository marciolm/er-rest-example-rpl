
Cooja Test-Bed for Retrieving RPL Routing Information using COAP under Contiki
==============================================================================

This test-bed project is a fork from Erbium COAP Server [1], RPLINFO [2]  and libcoap [3]

To install this test-bed: 
-------------------------
- Install the Contiki 3.x tree following the instructions on [https://github.com/contiki-os/contiki](https://github.com/contiki-os/contiki) 

	$ cd ~/
	$ git clone https://github.com/contiki-os/contiki

- Install libcoap:

	$ cd ~/
	$ wget http://downloads.sourceforge.net/project/libcoap/coap-18/libcoap-4.1.1.tar.gz 
	$ tar -xvzf libcoap-4.1.1.tar.gz
	$ cd libcoap-4.1.1
	$ ./configure
	$ make

- Clone this repository:

	$ cd ~/contiki/examples
	$ git clone -b contiki3.x https://github.com/marciolm/er-rest-example-rpl.git 

- Open a terminal and run Cooja simulation:

	$ cd er-rest-example-rpl
	$ ./inicia-cooja.sh 

* To connect your Linux host machine in the simulated WSN, you have to start theSlip connection to 6LBR, start the bridge br0 and enable the IPv6 routing using the inicia-router.sh script.

Open a second terminal:

	$ cd ~/contiki/examples/er-rest-example-rpl
	$ ./inicia-router.sh

There are some scripts to retrive the routing information via COAP. To run them, start a third terminal:

	$ cd ~/contiki/examples/er-rest-example-rpl
	$ ./coap-read-4.sh

* [1] https://github.com/contiki-os/contiki/tree/master/examples/er-rest-example
* [2] https://github.com/malvira/rplinfo
* [3] http://sourceforge.net/projects/libcoap
