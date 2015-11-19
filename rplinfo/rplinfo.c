#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "rest-engine.h"
#include "er-coap-engine.h"
#include "rplinfo.h"

/* debug */
#define DEBUG DEBUG_FULL
#include "net/ip/uip-debug.h"

uint16_t
ipaddr_add(const uip_ipaddr_t *addr, char *buf)
{
  uint16_t a, n;
  int i, f;
  n = 0;
  for (i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2)
    {
      a = (addr->u8[i] << 8) + addr->u8[i + 1];
      if (a == 0 && f >= 0)
        {
          if (f++ == 0)
            {
              n += sprintf(&buf[n], "::");
            }
        }
      else
        {
          if (f > 0)
            {
              f = -1;
            }
          else if (i > 0)
            {
              n += sprintf(&buf[n], ":");
            }
          n += sprintf(&buf[n], "%x", a);
        }
    }
  return n;
}

uint16_t
create_route_msg(char *buf, uip_ds6_route_t *r)
{
  uint8_t n = 0;
  n += sprintf(&(buf[n]), "{\"dest\":\"");
  n += ipaddr_add(&r->ipaddr, &(buf[n]));
  n += sprintf(&(buf[n]), "\",\"next\":\"");
  n += ipaddr_add(uip_ds6_route_nexthop(r), &(buf[n]));
  n += sprintf(&(buf[n]), "\"}");
  buf[n] = 0;
  PRINTF("buf_routes: %s\n", buf);
  return n;
}

static void
routes_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset);
RESOURCE(routes, "title=\"RPL route info\";rt=\"Data\"", routes_handler,
    routes_handler, NULL, NULL);
void
routes_handler(void* request, void* response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset)
{
  int32_t strpos = 0;
  uip_ds6_route_t *r;
  volatile uint8_t i;

  size_t len = 0;
  uint8_t index;
  const char *pstr;
  uint8_t count;

  /* count the number of routes and return the total */
  count = uip_ds6_route_num_routes();

  if ((len = REST.get_query_variable(request, "index", &pstr)))
    {
      index = (uint8_t) atoi(pstr);
      if (index >= count)
        {
          strpos = snprintf((char *) buffer, preferred_size, "{}");
        }
      else
        {
          /* seek to the route entry and return it */
          i = 0;
          for (r = uip_ds6_route_head(); r != NULL;
              r = uip_ds6_route_next(r), i++)
            {
              if (i == index)
                {
                  break;
                }
            }
          strpos = create_route_msg((char *) buffer, r);
        }
      REST.set_header_content_type(response, APPLICATION_JSON);

    }
  else
    { /* index not provided */
      strpos += snprintf((char *) buffer, preferred_size, "%d", count);
    }

  REST.set_response_payload(response, (char *) buffer, strpos);
}

/*  {"eui":"00050c2a8c9d4ea0","pref":"true","etx":124}*/
/* length of an neighbor entry, must be fixed width */
uint16_t
create_parent_msg(char *buf, rpl_parent_t *parent, uint8_t preferred,
    rpl_rank_t rank)
{
  uint8_t n = 0;
  uip_ipaddr_t * addr = rpl_get_parent_ipaddr(parent);
  n += sprintf(&(buf[n]), "{\"eui\":\"%04x%04x%04x%04x\",",
      UIP_HTONS(addr->u16[4]), UIP_HTONS(addr->u16[5]), UIP_HTONS(addr->u16[6]),
      UIP_HTONS(addr->u16[7]));
  n += sprintf(&(buf[n]), "\"p\":");
  if (preferred)
    {
      n += sprintf(&(buf[n]),"\"true\",");
    }
  else
    {
      n += sprintf(&(buf[n]),"\"false\",");
    }
  n += sprintf(&(buf[n]),"\"etx\":\"%d\",\"rank\":\"%d\"}", rank / RPL_DAG_MC_ETX_DIVISOR,
      rank);
  buf[n] = 0;
  PRINTF("buf_parents: %s\n", buf);
  return n;
}

static void
parents_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset);
RESOURCE(parents, "title=\"RPL parent info\";rt=\"Data\"", parents_handler,
    parents_handler, NULL, NULL);

static volatile uint8_t cur_neigh_entry;
static volatile uint8_t entry_char_skip;

void
parents_handler(void* request, void* response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset)
{
  int32_t strpos = 0;
  volatile uint8_t i;
  rpl_dag_t *dag;
  rpl_parent_t *parent = nbr_table_head(rpl_parents);
  size_t len = 0;
  uint8_t index;
  const char *pstr;
  uint8_t count;

  dag = rpl_get_any_dag();

  if (dag != NULL)
    {

      /* count the number of parents and return the total */
      count = uip_ds6_nbr_num(); // get number of parents
      rpl_print_neighbor_list(); // get parents for debug purposes

      if ((len = REST.get_query_variable(request, "index", &pstr)))
        {

          index = (uint8_t) atoi(pstr); // ascii to int index

          if (index >= count)
            { // if index > num of parents, return empty
              strpos = snprintf((char *) buffer, preferred_size, "{}");
            }
          else
            {
              /* seek to the parents entry and return it */
              for (i = 0; i <= count; i++)
                {
                  if (i == index)
                    {
                      break;
                    }
                  parent = nbr_table_next(rpl_parents, parent);
                }
              if (parent == dag->preferred_parent)
                {
                  strpos = create_parent_msg((char *) buffer, parent, 1,
                      dag->rank);
                }
              else
                {
                  strpos = create_parent_msg((char *) buffer, parent, 0,
                      dag->rank);
                }
            }

          REST.set_header_content_type(response, APPLICATION_JSON);

        }
      else
        { /* index not provided */
          strpos += snprintf((char *) buffer, preferred_size, "%d", count);
        }

    }
  else
    { /* no DAG */
      strpos += snprintf((char *) buffer, preferred_size,
          "{\"err\": \"no DAG\"}");
      REST.set_header_content_type(response, APPLICATION_JSON);
    }

  REST.set_response_payload(response, buffer, strpos);
}

//---------------------------------------------------------------------------------------------------

static void
get_variavel_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset);
static void
put_variavel_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset);
RESOURCE(variavel, "title=\"Grava\";rt=\"Text\"", get_variavel_handler, //get
    NULL,//post
    put_variavel_handler,//put
    NULL);
//delete
int variavelA = 3;
static void
get_variavel_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset)
{
  REST.set_response_payload(response, buffer,
      snprintf((char *) buffer, preferred_size, "VariavelA %d", variavelA));
}
put_variavel_handler(void *request, void *response, uint8_t *buffer,
    uint16_t preferred_size, int32_t *offset)
{
  uint8_t index;
  const char *valor;

  REST.get_query_variable(request, "index", &valor);
  index = (uint8_t) atoi(valor);

  variavelA = index;
  REST.set_response_status(response, REST.status.CHANGED);
}

//---------------------------------------------------------------------------------------------------

void
rplinfo_activate_resources(void)
{
  rest_activate_resource(&parents, "rplinfo/parents");
  rest_activate_resource(&routes, "rplinfo/routes");
  rest_activate_resource(&variavel, "rplinfo/grava");
}

