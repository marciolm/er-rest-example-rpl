A Quick Introduction to the Erbium (Er) REST Engine
===================================================

EXAMPLE FILES
-------------

- er-example-server.c: A RESTful server example showing how to use the REST
  layer to develop server-side applications (at the moment only CoAP is
  implemented for the REST Engine).
- er-example-client.c: A CoAP client that polls the /actuators/toggle resource
  every 10 seconds and cycles through 4 resources on button press (target
  address is hard-coded).
- er-plugtest-server.c: The server used for draft compliance testing at ETSI
  IoT CoAP Plugtests. Erbium (Er) participated in Paris, France, March 2012 and
  Sophia-Antipolis, France, November 2012 (configured for minimal-net).

PRELIMINARIES
-------------

- Make sure rpl-border-router has the same stack and fits into mote memory:
  You can disable RDC in border-router project-conf.h (not really required as BR keeps radio turned on).
    #undef NETSTACK_CONF_RDC
    #define NETSTACK_CONF_RDC     nullrdc_driver
- For convenience, define the Cooja addresses in /etc/hosts
      aaaa::0212:7401:0001:0101 cooja1
      aaaa::0212:7402:0002:0202 cooja2
      ...
- Get the Copper (Cu) CoAP user-agent from
  [https://addons.mozilla.org/en-US/firefox/addon/copper-270430](https://addons.mozilla.org/en-US/firefox/addon/copper-270430)
- Optional: Save your target as default target
      make TARGET=sky savetarget

COOJA HOWTO
-----------

###Server only:

    make TARGET=cooja server-only.csc

Open new terminal

    make connect-router-cooja

- Start Copper and discover resources at coap://cooja2:5683/
- Choose "Click button on Sky 2" from the context menu of mote 2 (server) after
  requesting /test/separate
- Do the same when observing /test/event

###With client:

    make TARGET=cooja server-client.csc

Open new terminal

    make connect-router-cooja

- Wait until red LED toggles on mote 2 (server)
- Choose "Click button on Sky 3" from the context menu of mote 3 (client) and
  watch serial output

-------

Erbium currently implements draft 13.  Central features are commented in
er-example-server.c.  In general, apps/er-coap-13 supports:

- All draft 13 header options
- CON Retransmissions (note COAP_MAX_OPEN_TRANSACTIONS)
- Blockwise Transfers (note REST_MAX_CHUNK_SIZE, see er-plugtest-server.c for
  Block1 uploads)
- Separate Responses (no rest_set_pre_handler() required anymore, note
  coap_separate_accept(), _reject(), and _resume())
- Resource Discovery
- Observing Resources (see EVENT_ and PRERIODIC_RESOURCE, note
  COAP_MAX_OBSERVERS)

REST IMPLEMENTATIONS
--------------------

The Makefile uses WITH_COAP to configure different implementations for the
Erbium (Er) REST Engine.

- WITH_COAP=13 uses Erbium CoAP 13 apps/er-coap-13/.  The default port for
  coap-13 is 5683.
- WITH_COAP=0 is a stub to link an Erbium HTTP engine that uses the same
  resource abstraction (REST.x() functions and RESOURCE macros.

TODOs
-----

- Dedicated Observe buffers
- Optimize message struct variable access (directly access struct without copying)
- Observe client
- Multiple If-Match ETags
- (Message deduplication)
