1.Speedport Hybrid is composed of host processor and coprocessor.The file list of DT_Hybrid_GPL_02.00.010 
(1)ReadMe.txt: This file

(2)crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21.Rel1.0.tar.bz2: The toolchain used by Speedport Hybrid host processor
   Hybrid_GPL_63168.tar.bz2: The open source codes of Speedport Hybrid host processor

(3)arm-hsan-linux-uclibcgnueabi.tar.bz2: The toolchain used by Speedport Hybrid coprocessor
   Hybrid_GPL_5115.tar.bz2: The open source codes of Speedport Hybrid coprocessor

2.How to compile the open soruce codes for Speedport Hybrid host processor
A)Untar&cp the toolchain to /opt/toolchains directory of compiler Linux PC. 
tar -jxvf crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21.Rel1.0.tar.bz2
cp -af crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21 /opt/toolchains/

B)Run cross-compile commands
tar -jxvf Hybrid_GPL_63168.tar.bz2
cd Hybrid_GPL_63168
sh compiler_cmo

3.How to compile the open soruce codes for Speedport Hybrid coprocessor
A)Untar&cp the toolchain to /opt/toolchain_hisi directory of compiler Linux PC. 
tar -jxvf arm-hsan-linux-uclibcgnueabi.tar.bz2
cp -af arm-hsan-linux-uclibcgnueabi /opt/toolchain_hisi/

B)Run cross-compile commands
tar -jxvf Hybrid_GPL_5115.tar.bz2
cd Hybrid_GPL_5115
sh compiler_cmo



