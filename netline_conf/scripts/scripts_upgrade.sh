#!/bin/sh

# copy data
cp -fr ${SECONDARY_STORAGE}/freeswitch/scripts /data/freeswitch/

# open permissions
chmod 777 /data/freeswitch
for node in `find /data/freeswitch/`; do
    chmod 777 ${node};
done
