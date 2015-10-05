/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* Define to enable Accounting-On and Accounting-Off */
/* #undef ENABLE_ACCOUNTING_ONOFF */

/* Define to enable binary status file */
#define ENABLE_BINSTATFILE 1

/* Define to enable bonjour */
/* #undef ENABLE_BONJOUR */

/* Define to enable HTTP AAA Proxy */
/* #undef ENABLE_CHILLIPROXY */

/* Define to enable chilli_query */
#define ENABLE_CHILLIQUERY 1

/* Define to enable RadSec AAA Proxy */
/* #undef ENABLE_CHILLIRADSEC */

/* Define to enable Redir server */
/* #undef ENABLE_CHILLIREDIR */

/* Define to enable chilli_script helper */
/* #undef ENABLE_CHILLISCRIPT */

/* Define to enable Chilli XML */
/* #undef ENABLE_CHILLIXML */

/* Define to enable cluster */
/* #undef ENABLE_CLUSTER */

/* Define to enable debugging */
/* #undef ENABLE_DEBUG */

/* Define to enable verbose debugging */
/* #undef ENABLE_DEBUG2 */

/* Define to enable DHCP option setting */
/* #undef ENABLE_DHCPOPT */

/* Define to enable DHCP/RADIUS integration */
/* #undef ENABLE_DHCPRADIUS */

/* Define to logging of DNS requests */
/* #undef ENABLE_DNSLOG */

/* Define to enable EAPOL */
/* #undef ENABLE_EAPOL */

/* Define to enable CoovaEWT API */
/* #undef ENABLE_EWTAPI */

/* Define to enable Chilli IEEE 802.1Q */
#define ENABLE_IEEE8021Q 1

/* Define to enable IEEE 802.3 */
/* #undef ENABLE_IEEE8023 */

/* Define to support file based whitelists */
#define ENABLE_IPWHITELIST 1

/* Define to enable Chilli JSON */
/* #undef ENABLE_JSON */

/* Enable larger limits for use with non-embedded systems */
#define ENABLE_LARGELIMITS 1

/* Define to enable Layer3 only support */
/* #undef ENABLE_LAYER3 */

/* Define to enable Chilli Leaky Bucket shaping */
#define ENABLE_LEAKYBUCKET 1

/* Define to enable minimal cmdline config */
/* #undef ENABLE_MINICONFIG */

/* Define to enable Coova miniportal */
/* #undef ENABLE_MINIPORTAL */

/* Define to enable dynamically loadable modules */
#define ENABLE_MODULES 1

/* Define to enable multiple routes */
/* #undef ENABLE_MULTIROUTE */

/* Define to enable NetBIOS */
/* #undef ENABLE_NETBIOS */

/* Define to enable network interface nat */
/* #undef ENABLE_NETNAT */

/* Define to enable PPPoE */
/* #undef ENABLE_PPPOE */

/* Define to enable VSA proxy */
/* #undef ENABLE_PROXYVSA */

/* Define to enable Chilli RADIUS (EAP) Proxy support */
#define ENABLE_RADPROXY 1

/* Define to DNS query on redirect to pick up dynamic walled garden */
#define ENABLE_REDIRDNSREQ 1

/* Define to Redir content-injection support */
/* #undef ENABLE_REDIRINJECT */

/* Define to enable Chilli session walled garden */
/* #undef ENABLE_SESSGARDEN */

/* Define to enable Simple Service Discovery Protocol */
/* #undef ENABLE_SSDP */

/* Define to enable status file */
#define ENABLE_STATFILE 1

/* Define to enable Chilli tap support */
#define ENABLE_TAP 1

/* Define to support loading of uamdomains (with regex) from file */
#define ENABLE_UAMDOMAINFILE 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <asm/types.h> header file. */
#define HAVE_ASM_TYPES_H 1

/* Define to 1 if you have the `dirname' function. */
#define HAVE_DIRNAME 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
#define HAVE_ENDIAN_H 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `getline' function. */
#define HAVE_GETLINE 1

/* Define to 1 if you have the `getprotoent' function. */
#define HAVE_GETPROTOENT 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `glob' function. */
#define HAVE_GLOB 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the `inet_ntoa' function. */
#define HAVE_INET_NTOA 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define to 1 if you have the `resolv' library (-lresolv). */
/* #undef HAVE_LIBRESOLV */

/* Define to 1 if you have the `rt' library (-lrt). */
#define HAVE_LIBRT 1

/* Define to use Jenkins lookup3 */
/* #undef HAVE_LOOKUP3 */

/* Define if you have matrixssl */
/* #undef HAVE_MATRIXSSL */

/* Define to 1 if you have the <matrixSsl.h> header file. */
/* #undef HAVE_MATRIXSSL_H */

/* Define to 1 if you have the <matrixSsl/matrixSsl.h> header file. */
/* #undef HAVE_MATRIXSSL_MATRIXSSL_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to use coova kernel module */
/* #undef HAVE_NETFILTER_COOVA */

/* Define if you have netfilter_queue */
/* #undef HAVE_NETFILTER_QUEUE */

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <net/ethernet.h> header file. */
#define HAVE_NET_ETHERNET_H 1

/* Define to 1 if you have the <net/if_arp.h> header file. */
#define HAVE_NET_IF_ARP_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <net/if_tun.h> header file. */
/* #undef HAVE_NET_IF_TUN_H */

/* Define to 1 if you have the <net/route.h> header file. */
#define HAVE_NET_ROUTE_H 1

/* Define if you have openssl */
/* #undef HAVE_OPENSSL */

/* Define to 1 if you have the <poll.h> header file. */
#define HAVE_POLL_H 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX_H 1

/* Define to 1 if you have the <resolv.h> header file. */
#define HAVE_RESOLV_H 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to use SuperFastHash */
#define HAVE_SFHASH 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/epoll.h> header file. */
#define HAVE_SYS_EPOLL_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/msg.h> header file. */
#define HAVE_SYS_MSG_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysinfo.h> header file. */
#define HAVE_SYS_SYSINFO_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "coova-chilli"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "support@coova.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "coova-chilli"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "coova-chilli 1.2.6"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "coova-chilli"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.6"

/* Define to the type of arg 1 for `select'. */
#define SELECT_TYPE_ARG1 int

/* Define to the type of args 2, 3 and 4 for `select'. */
#define SELECT_TYPE_ARG234 (fd_set *)

/* Define to the type of arg 5 for `select'. */
#define SELECT_TYPE_ARG5 (struct timeval *)

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define if you have curl enabled */
/* #undef USING_CURL */

/* Define to use SV IPC message queue */
/* #undef USING_IPC_MSG */

/* Define if you have mmap enabled */
/* #undef USING_MMAP */

/* Define if you have pcap enabled */
/* #undef USING_PCAP */

/* Define if you have poll() enabled */
/* #undef USING_POLL */

/* Version number of package */
#define VERSION "1.2.6"

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */
