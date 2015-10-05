#!/bin/sh

#find . -name "Makefile" | xargs -i sh -c "sed -e 's/^CC=/#CC=/' -e 's/^RANLIB=/#RANLIB=/' -e 's/^AR=/#AR=/' -e 's/$(AR) $(LIB)/$(AR) r $(LIB)/' > {}"

#files = `find . -name "Makefile"`

# Do Configuration

./Configure $1 shared no-asm

#./Configure atp_b_config shared no-asm
#./Configure atp_l_config shared no-asm

# Omit all compiling names
for file in `find . -name "Makefile"`
do
	echo "modifying " $file
	cp $file "$file"_bk
	sed -e 's/^CC=/#CC=/' -e 's/^RANLIB=/#RANLIB=/' -e 's/^AR=/#AR=/' -e 's/$(AR) $(LIB)/$(AR) r $(LIB)/' "$file"_bk > $file
	rm -f "$file"_bk
done
