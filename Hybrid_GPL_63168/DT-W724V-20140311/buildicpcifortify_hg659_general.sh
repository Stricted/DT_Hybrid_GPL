#!/bin/bash
set +x

make VENDOR=hg659 CUSTOMER=general samba-clean

./buildicpcifortify.sh debug_yes hg659 general samba

exit 0

