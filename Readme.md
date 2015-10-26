
Cooja Test-Bed for Retrieving RPL Routing Information using COAP
================================================================

This test-bed project is a fork from Erbium COAP Server [1], RPLINFO [2]  and libcoap [3]

To install this test-bed: 
-------------------------
- Install the Contiki 3.x tree following the instructions on [https://github.com/contiki-os/contiki](https://github.com/contiki-os/contiki) 

Install libcoap:

	$ cd ~/
	$ wget http://downloads.sourceforge.net/project/libcoap/coap-18/libcoap-4.1.1.tar.gz 
	$ tar -xvzf libcoap-4.1.1.tar.gz
	$ cd libcoap-4.1.1
	$ ./configure
	$ make

Clone this repository:

	$ cd ~/contiki/examples
	$ git clone -b contiki3.x https://github.com/marciolm/er-rest-example-rpl.git 

Open a terminal and run Cooja simulation:

	$ cd er-rest-example-rpl
	$ ./inicia-cooja.sh 

* To connect your Linux host machine in the simulated WSN, you have to start the 6LBR, start the bridge br0 and enable the IPv6 routing.

Open a second terminal:

	$ cd ~/contiki/examples/er-rest-example-rpl
	$ ./inicia-router.sh

There are some scrips to retrive the routing information via COAP. To run them, start a third terminal:

	$ cd ~/contiki/examples/er-rest-example-rpl
	$ ./coap-read-4.sh

* [1] https://github.com/contiki-os/contiki/tree/master/examples/er-rest-example
* [2] https://github.com/malvira/rplinfo
* [3] https://github.com/cetic/6lbr/wiki
* [4] http://sourceforge.net/projects/libcoap