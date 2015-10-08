sudo brctl addbr br0
sudo ifconfig br0 up
sudo sysctl -w net.ipv6.conf.br0.accept_ra=1
sudo sysctl -w net.ipv6.conf.br0.accept_ra_rt_info_max_plen=64
pushd ~/6lbr/examples/6lbr/test
sudo ../package/usr/bin/6lbr conf-templates/cooja-1-router.conf
popd

