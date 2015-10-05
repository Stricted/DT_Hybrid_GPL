#COMMIT=$(shell git log --pretty=format:"%h" | head -n 1)
#VERSION=1.0.2-$(COMMIT)
#ifeq ($(strip $(COMMIT)),)
#	VERSION=1.0.2
#endif
VERSION=1.0.2
#ARCH=$(shell dpkg-architecture -qDEB_BUILD_ARCH)
ARCH=$(shell uname -i)
PACKAGE=fonap_$(VERSION)_$(ARCH).deb
freplace = $(shell if [ ! -d `dirname $(2)` ]; then mkdir -p `dirname $(2)`; fi; cp $(1) $(2))
BUILDIR:=$(shell pwd)/build
EBUILDIR:=$(shell echo $(BUILDIR) | sed 's/\//\\\//g')

#CHILLI=coova
#FONSMCD=fonsmcd
#PACKAGE=fonap_$(VERSION)_$(ARCH).deb

BASE_CONFIGS= --program-prefix="" --program-suffix="" --prefix=/usr \
         --sysconfdir=/etc --exec-prefix=/usr --bindir=/usr/bin \
         --datadir=/usr/share --includedir=/usr/include \
         --infodir=/usr/share/info --libdir=/usr/lib \
         --libexecdir=/usr/lib --localstatedir=/var \
         --mandir=/usr/share/man --sbindir=/usr/sbin \
         --enable-shared --disable-static \
         --with-gnu-ld --host=mips-linux-gnu

FONAPI_CONFIGS=$(BASE_CONFIGS) 
CHILLOUT_CONFIGS=$(BASE_CONFIGS) 
FONSMCD_CONFIGS=$(BASE_CONFIGS)
XL2TPD_CONFIGS=$(BASE_CONFIGS) 
COOVA_MOD_CONFIGS=$(BASE_CONFIGS) 
COOVA_CONFIGS=$(BASE_CONFIGS) \
         --enable-binstatusfile --enable-ipwhitelist \
         --enable-uamdomainfile --enable-redirdnsreq \
	 --enable-modules --disable-accounting-onoff \
	 --disable-dhcpradius --disable-debug
# Optional: --enable-multiroute

# pablo: 2011-05-05: disabled --enable-miniconfig so
# chilli_opt is more strict checking

FONAPI_BINARIES=deb/usr/lib/coova-chilli/fon-api.so 

COOVA_MOD_BINARIES=deb/usr/lib/coova-chilli/chillout.so

COOVA_BINARIES=deb/usr/sbin/chilli
COOVA_FILES=deb/etc/cron.daily/chilli \
deb/etc/init.d/chilli deb/etc/default/chilli \
deb/etc/fon/www/wait.html deb/etc/fon/www/wait.gif \
deb/etc/fon/www/fon.png deb/etc/init.d/chilli_ipup deb/etc/init.d/chilli_ipdown 

DNSMASQ_FILES=deb/etc/dnsmasq.conf

FONSMCD_BINARIES=deb/usr/sbin/fonsmcd
FONSMCD_FILES=deb/etc/fon_device deb/etc/fon_keyword \
deb/etc/fon_mac deb/etc/fon_revision deb/etc/fonsmcd.conf \
deb/etc/fon_version deb/etc/init.d/fonsmcd deb/etc/public_sm_rsa_key.pem \
deb/etc/init.d/fonsmcd_online deb/etc/init.d/fonsmcd_offline

CHILLOUTD_FILES=deb/usr/sbin/rule.sh deb/etc/firewall.fonap deb/etc/init.d/fonap
CHILLOUTD_BINARIES=deb/usr/sbin/chilloutd

XL2TPD_BINARIES=deb/usr/sbin/xl2tpd
XL2TPD_FILES=deb/etc/xl2tpd.conf

PPP_BINARIES=deb/usr/sbin/pppd
PPP_FILES=deb/etc/ppp/ppp-xl2tpd.client deb/usr/lib/pppd/2.4.4/passwordfd.so deb/usr/lib/pppd/2.4.4/pppol2tp.so deb/etc/ppp/ip-up deb/etc/ppp/ip-down

OTHER_FILES=deb/DEBIAN/control deb/usr/share/doc/fonap/ChangeLog.gz \
deb/usr/share/doc/fonap/COPYRIGHT.gz
CONFFILES:=$(shell echo `cat DEBIAN/conffiles | sed 's/\//deb\//'`)

ifndef GPLREL
all: $(PACKAGE) 
#	$(COOVA_MOD_BINARIES) \ 
$(PACKAGE): deb \
	$(CHILLOUTD_BINARIES) $(CHILLOUTD_FILES) \
	$(COOVA_BINARIES) $(COOVA_FILES) \
	$(FONAPI_BINARIES) \
	$(DNSMASQ_FILES) \
	$(FONSMCD_BINARIES) $(FONSMCD_FILES) \
	$(XL2TPD_BINARIES) $(XL2TPD_FILES) \
	$(PPP_BINARIES) $(PPP_FILES) \
	$(CONFFILES) $(OTHER_FILES)
#	fakeroot dpkg-deb -b deb/ .
else
all: $(PACKAGE) 
#	$(COOVA_MOD_BINARIES) \ 
$(PACKAGE): deb \
	$(CHILLOUTD_BINARIES) \
	$(COOVA_BINARIES) \
	$(XL2TPD_BINARIES) \
	$(PPP_BINARIES)
#	fakeroot dpkg-deb -b deb/ .
endif

deb/usr/share/doc/fonap/ChangeLog.gz:
	mkdir -p deb/usr/share/doc/fonap/
	touch deb/usr/share/doc/fonap/ChangeLog
	gzip --best -f deb/usr/share/doc/fonap/ChangeLog


deb/usr/share/doc/fonap/COPYRIGHT.gz:
	touch deb/usr/share/doc/fonap/COPYRIGHT
	gzip --best -f deb/usr/share/doc/fonap/COPYRIGHT

deb/DEBIAN/control: DEBIAN/control
	cp -R DEBIAN deb/
	rm -rf $(find deb/ -name .svn)
	sed -i "s/Version:/Version: $(VERSION)/" deb/DEBIAN/control
	sed -i "s/Architecture:/Architecture: $(ARCH)/" deb/DEBIAN/control

deb/usr/sbin/chilli: 
	-mkdir -p $(BUILDIR)
	cd src/coova/ && [ -e ./configure ] || sh bootstrap
	cd src/coova/ && ./configure $(COOVA_CONFIGS)
	make -C src/coova/ -f Makefile
	make -C src/coova/ DESTDIR=$(BUILDIR) install
	mkdir -p deb/usr/sbin deb/usr/lib
	cp $(BUILDIR)/usr/sbin/chilli deb/usr/sbin/
	cp $(BUILDIR)/usr/sbin/chilli_radconfig deb/usr/sbin/
	cp $(BUILDIR)/usr/sbin/chilli_query deb/usr/sbin/
	cp $(BUILDIR)/usr/sbin/chilli_opt deb/usr/sbin/
	cp -a $(BUILDIR)/usr/lib/lib*.so* deb/usr/lib/
	#strip deb/usr/sbin/chilli*
	#strip deb/usr/lib/lib*

deb/etc/cron.daily/chilli: files/coova/chilli.cron.in
	$(call freplace,files/coova/chilli.cron.in,deb/etc/cron.daily/chilli)
deb/etc/init.d/chilli: files/coova/chilli.init.in
	$(call freplace,files/coova/chilli.init.in,deb/etc/init.d/chilli)
deb/etc/default/chilli: files/coova/chilli.default.in
	$(call freplace,files/coova/chilli.default.in,deb/etc/default/chilli)
deb/etc/init.d/chilli_ipup: files/coova/chilli_ipup
	$(call freplace,files/coova/chilli_ipup,deb/etc/init.d/chilli_ipup)
deb/etc/init.d/chilli_ipdown: files/coova/chilli_ipdown
	$(call freplace,files/coova/chilli_ipdown,deb/etc/init.d/chilli_ipdown)
		
deb/usr/lib/coova-chilli/fon-api.so:
	-mkdir -p $(BUILDIR)
	cd src/fon-api/ && [ -e ./configure ] || sh bootstrap
	cd src/fon-api/ && ./configure $(FONAPI_CONFIGS)
	make -C src/fon-api/ -f Makefile
	make -C src/fon-api/ DESTDIR=$(BUILDIR) install
	mkdir -p deb/usr/lib/coova-chilli/
	cp -a $(BUILDIR)/usr/lib/coova-chilli/fon-api.so deb/usr/lib/coova-chilli/
	cp -a $(BUILDIR)/usr/sbin/fonctl deb/usr/sbin/
	cp -a src/fon-api/api-example.sh deb/usr/sbin/
	#strip -K fon_api_module deb/usr/lib/coova-chilli/fon-api.so
	#strip deb/usr/sbin/fonctl

deb/usr/lib/coova-chilli/chillout.so:
	-mkdir -p $(BUILDIR)
	cd src/chillout-mod/ && [ -e ./configure ] || sh bootstrap
	cd src/chillout-mod/ && ./configure $(COOVA_MOD_CONFIGS)
	make -C src/chillout-mod/ -f Makefile
	make -C src/chillout-mod/ DESTDIR=$(BUILDIR) install
	mkdir -p deb/usr/lib/coova-chilli/
	cp -a $(BUILDIR)/usr/lib/coova-chilli/chillout.so deb/usr/lib/coova-chilli/
	#strip -K chillout_module deb/usr/lib/coova-chilli/chillout.so

deb/etc/dnsmasq.conf: files/dnsmasq/dnsmasq.conf
	mkdir -p deb/etc/
	cp files/dnsmasq/dnsmasq.conf deb/etc/dnsmasq.conf

deb/usr/sbin/fonsmcd: 
	-mkdir -p $(BUILDIR)
	#cd src/fonsmcd/ && [ -e ./configure ] || sh bootstrap
	#cd src/fonsmcd/ && ./configure $(FONSMCD_CONFIGS)
	make -C src/fonsmcd/ -f Makefile
	make -C src/fonsmcd/ DESTDIR=$(BUILDIR) install
	mkdir -p deb/usr/sbin 
	cp $(BUILDIR)/usr/sbin/fonsmcd deb/usr/sbin/
	#strip deb/usr/sbin/fonsmcd

deb/usr/sbin/chilloutd: 
	-mkdir -p $(BUILDIR)
	cd src/chillout/ && [ -e ./configure ] || sh bootstrap
	cd src/chillout/ && ./configure $(CHILLOUT_CONFIGS)
	make -C src/chillout/ -f Makefile
	make -C src/chillout/ DESTDIR=$(BUILDIR) install
	mkdir -p deb/usr/sbin deb/usr/lib
	cp $(BUILDIR)/usr/sbin/chillout* deb/usr/sbin/
	cp -a $(BUILDIR)/usr/lib/libchillout*.so* deb/usr/lib/
	#strip deb/usr/sbin/chillout*
	#strip deb/usr/lib/libchillout*

deb/usr/sbin/xl2tpd: 
	-mkdir -p $(BUILDIR)
	cd src/xl2tpd/ && [ -e ./configure ] || sh bootstrap
	cd src/xl2tpd/ && ./configure $(XL2TPD_CONFIGS)
	make -C src/xl2tpd/ -f Makefile
	make -C src/xl2tpd/ DESTDIR=$(BUILDIR) install
	cp $(BUILDIR)/usr/sbin/xl2tpd deb/usr/sbin/
	#strip deb/usr/sbin/xl2tpd

deb/etc/xl2tpd.conf: files/etc/xl2tpd.conf
	-mkdir -p deb/etc
	cp $< $@
deb/etc/ppp/ppp-xl2tpd.client: files/etc/ppp/ppp-xl2tpd.client
	-mkdir -p deb/etc/ppp
	cp $< $@
deb/etc/ppp/ip-up: files/etc/ppp/ip-up
	-mkdir -p deb/etc/ppp
	cp $< $@
deb/etc/ppp/ip-down: files/etc/ppp/ip-down
	-mkdir -p deb/etc/ppp
	cp $< $@
deb/usr/lib/pppd/2.4.4/passwordfd.so: src/ppp/pppd/plugins/passwordfd.so
	-mkdir -p deb/usr/lib/pppd/2.4.4/
	cp $< $@ 
deb/usr/lib/pppd/2.4.4/pppol2tp.so: src/ppp/pppd/plugins/pppol2tp/pppol2tp.so
	-mkdir -p deb/usr/lib/pppd/2.4.4/
	cp $< $@ 

deb/usr/sbin/pppd: $(PPP_SOURCE_F)
	-mkdir $(BUILDIR)
	cd src/ppp && chmod 777 * && ./configure --prefix=/usr
	make -C src/ppp clean
	make -C src/ppp
	cp src/ppp/pppd/pppd deb/usr/sbin/pppd

deb: 
	mkdir -p deb/

deb/etc/fon/www/wait.html: files/www/wait.html
	-mkdir -p deb/etc/fon/www
	cp $< $@
deb/etc/fon/www/wait.gif: files/www/wait.gif
	-mkdir -p deb/etc/fon/www
	cp $< $@
deb/etc/fon/www/fon.png: files/www/fon.png
	-mkdir -p deb/etc/fon/www
	cp $< $@
deb/etc/fon_mac: files/fonsmcd/fon_mac 
	cp $< $@
deb/etc/fon_keyword: files/fonsmcd/fon_keyword
	cp $< $@
deb/etc/fon_device: files/fonsmcd/fon_device
	cp $< $@
deb/etc/fon_version: files/fonsmcd/fon_version
	cp $< $@
deb/etc/fon_revision: files/fonsmcd/fon_revision
	cp $< $@
deb/etc/fonsmcd.conf: files/fonsmcd/fonsmcd.conf
	cp $< $@
deb/etc/public_sm_rsa_key.pem: files/fonsmcd/public_sm_rsa_key.pem 
	cp $< $@
deb/etc/init.d/fonsmcd: files/fonsmcd/fonsmcd.init
	cp $< $@
deb/etc/init.d/fonsmcd_online: files/fonsmcd/fonsmcd_online
	cp $< $@
deb/etc/init.d/fonsmcd_offline: files/fonsmcd/fonsmcd_offline
	cp $< $@
deb/etc/default/fonap: files/fonap/fonap.default
	-mkdir -p deb/etc/default
	cp $< $@
deb/etc/init.d/fonap: files/fonap/fonap.init
	-mkdir -p deb/etc/init.d
	cp $< $@
deb/etc/firewall.fonap: files/fonap/firewall.fonap
	-mkdir deb/etc
	cp $< $@
deb/usr/sbin/rule.sh: src/chillout/rule.sh
	cp $< $@
#$(PACKAGE):
#	mkdir -p deb
# make chilli
#	make -C $(CHILLI)
#	cp -r $(CHILLI)/deb/* deb/
# make dns
#	make -C $(DNS)
#	cp -r $(DNS)/deb/* deb/
# make fonsmcd
#	make -C $(FONSMCD)
#	cp -r $(FONSMCD)/deb/* deb/
#	mkdir -p deb/etc/init.d/
#	cp -r files/* deb/
#	mkdir -p deb/usr/share/doc/fonap/
#	cp changelog deb/usr/share/doc/fonap/
#	cp copyright deb/usr/share/doc/fonap/
#	gzip --best -f deb/usr/share/doc/fonap/changelog
#	cp -R DEBIAN deb/
#	sed -i "s/Version:/Version: $(VERSION)/" deb/DEBIAN/control
#	sed -i "s/Architecture:/Architecture: $(ARCH)/" deb/DEBIAN/control
#	fakeroot dpkg-deb -b deb/ ./
#lintian:
#	lintian $(PACKAGE)

DETRITUS_FILES = \
src/xl2tpd/INSTALL src/xl2tpd/ltmain.sh src/xl2tpd/install-sh \
src/xl2tpd/config.h.in src/xl2tpd/configure src/xl2tpd/config.sub \
src/xl2tpd/depcomp src/xl2tpd/config.guess src/xl2tpd/aclocal.m4 \
src/xl2tpd/Makefile.in src/xl2tpd/autom4te.cache \
src/xl2tpd/autom4te.cache/traces.1 src/xl2tpd/autom4te.cache/output.0 \
src/xl2tpd/autom4te.cache/requests src/xl2tpd/autom4te.cache/output.1 \
src/xl2tpd/autom4te.cache/traces.0 src/xl2tpd/missing \
src/xl2tpd/COPYING src/coova/www/config.sh src/coova/conf/chilli \
src/coova/conf/defaults src/coova/conf/up.sh \
src/coova/conf/chilli.conf src/coova/conf/CoovaChilliLib.py \
src/coova/conf/functions src/coova/conf/down.sh \
src/coova/conf/newmulti.sh src/coova/doc/chilli-radius.5 \
src/coova/doc/chilli.8 src/coova/doc/chilli_radconfig.1 \
src/coova/doc/chilli_query.1 src/coova/doc/chilli_rtmon.1 \
src/coova/doc/chilli_proxy.1 src/coova/doc/chilli_script.1 \
src/coova/doc/chilli_opt.1 src/coova/doc/chilli.conf.5 \
src/coova/doc/chilli_radsec.1 src/coova/doc/chilli_response.1 \
src/coova/doc/chilli_redir.1 src/chillout-mod/ltmain.sh \
src/chillout-mod/install-sh src/chillout-mod/config.h.in \
src/chillout-mod/configure src/chillout-mod/config.sub \
src/chillout-mod/depcomp src/chillout-mod/config.guess \
src/chillout-mod/aclocal.m4 src/chillout-mod/Makefile.in \
src/chillout-mod/autom4te.cache \
src/chillout-mod/autom4te.cache/traces.1 \
src/chillout-mod/autom4te.cache/output.0 \
src/chillout-mod/autom4te.cache/requests \
src/chillout-mod/autom4te.cache/output.1 \
src/chillout-mod/autom4te.cache/traces.0 src/chillout-mod/missing \
src/fonsmcd/ltmain.sh src/fonsmcd/install-sh src/fonsmcd/config.h.in \
src/fonsmcd/configure src/fonsmcd/config.sub src/fonsmcd/depcomp \
src/fonsmcd/config.guess src/fonsmcd/aclocal.m4 \
src/fonsmcd/Makefile.in src/fonsmcd/autom4te.cache \
src/fonsmcd/autom4te.cache/traces.1 \
src/fonsmcd/autom4te.cache/output.0 \
src/fonsmcd/autom4te.cache/requests \
src/fonsmcd/autom4te.cache/output.1 \
src/fonsmcd/autom4te.cache/traces.0 src/fonsmcd/missing \
src/chillout/INSTALL src/chillout/ltmain.sh src/chillout/install-sh \
src/chillout/config.h.in src/chillout/configure \
src/chillout/config.sub src/chillout/depcomp src/chillout/config.guess \
src/chillout/aclocal.m4 src/chillout/Makefile.in \
src/chillout/autom4te.cache src/chillout/autom4te.cache/traces.1 \
src/chillout/autom4te.cache/output.0 \
src/chillout/autom4te.cache/requests \
src/chillout/autom4te.cache/output.1 \
src/chillout/autom4te.cache/traces.0 src/chillout/missing 

ifndef GPLREL
clean:
	-make -C src/chillout distclean
	-make -C src/fonsmcd distclean
	-make -C src/coova distclean
	-make -C src/xl2tpd distclean
	-make -C src/ppp clean
	-make -C src/chillout-mod distclean
	-rm -fR deb/ build/
	-rm -fr $(PACKAGE)
	-rm -fr $(DETRITUS_FILES)
else
clean:
	-make -C src/chillout distclean
	-make -C src/coova distclean
	-make -C src/xl2tpd distclean
	-make -C src/ppp clean
	-rm -fR deb/ build/
	-rm -fr $(PACKAGE)
	-rm -fr $(DETRITUS_FILES)
endif	

tagses:
	$(shell ctags `find -name *.[ch] | sed ':a;N;$!ba;s/\n/ /g'`)

.PHONY: deb/usr/share/doc/fonap/ChangeLog.gz clean tagses
