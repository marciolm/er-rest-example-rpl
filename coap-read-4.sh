num_motes=6
for ((i=2;i<=$num_motes;i++)); do 
   echo "================================================================"
   num_parents=`~/libcoap-4.1.1/examples/coap-client -B 5 -m get coap://[aaaa::200:0:0:$i]:5683/rplinfo/parents | tail -1`
   echo "numero de pais do mote cooja$i = $num_parents "
   for ((j=0;j<$num_parents;j++)); do
      ~/libcoap-4.1.1/examples/coap-client -B 5 -m get coap://[aaaa::200:0:0:$i]:5683/rplinfo/parents?index=$j | tail -1
   done
   num_rotas=`~/libcoap-4.1.1/examples/coap-client -B 5 -m get coap://[aaaa::200:0:0:$i]:5683/rplinfo/routes | tail -1` 
   echo "numero de rotas no mote cooja$i = $num_rotas " 
   for ((j=0;j<$num_rotas;j++)); do
      ~/libcoap-4.1.1/examples/coap-client -B5 -m get coap://[aaaa::200:0:0:$i]:5683/rplinfo/routes?index=$j | tail -1 
   done
done
