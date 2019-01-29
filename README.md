# QSDL-QUEST
Quest Simulation &amp; Verification Tool for SDL/QSDL (PhD Thesis) - Main QUEST Program (QSDL --> C++)

QUEST translates an SDL or QSDL specification into executable C++ code for simulation or verification of this specification. SDL is a non-proprietary internationally standardized language (ITU–T standard Z.100 and Z.105). QSDL is an extension of this language to add performance metrics and requirements.

QUEST consists of different parts:
1. QUEST Data Structure - QSDL-DS repository: https://github.com/Diefenbruch/QSDL-DS
2. QUEST QSDL Parser - QSDL-QP repository: https://github.com/Diefenbruch/QSDL-QP 
3. QUEST QSDL Code Generator - QSDL-CG repository: https://github.com/Diefenbruch/QSDL-CG
4. QUEST Main program - QSDL-QUEST repository: https://github.com/Diefenbruch/QSDL-QUEST

QUEST genrates C++ code and Makefile for each QSDL specification. This C++ code will be linked to additional libraries:

5. Simulation Class Library (SCL):  https://github.com/Diefenbruch/QSDL-SCL
6. Temporal Logic Library (TL):  https://github.com/Diefenbruch/QSDL-TL
7. Performance Evaluation and Visualization (PEV):  https://github.com/Diefenbruch/QSDL-PEV

Requirements:

* Linux operating system
* GNU tools and GCC installed
* X11 Windows installed (for PEV Visualization only)

Installation of QUEST:

 1. Create qsdl user in Linux (e.g. with /home/qsdl user space)
 2. Login as qsdl user and be in /home/qsdl directory (if you choose another directory you have to adjust quest-ini.sh script)
 3. git clone https://github.com/Diefenbruch/QSDL-INIT . 
 4. git clone https://github.com/Diefenbruch/QSDL-DS src/ds 
 5. git clone https://github.com/Diefenbruch/QSDL-CG src/cg
 6. git clone https://github.com/Diefenbruch/QSDL-QP src/qp
 7. git clone https://github.com/Diefenbruch/QSDL-QUEST src/quest
 8. git clone https://github.com/Diefenbruch/QSDL-SCL src/scl
 9. git clone https://github.com/Diefenbruch/QSDL-TL src/tl
10. git clone https://github.com/Diefenbruch/QSDL-PEV src/pev
11. source quest-ini.sh
12. make install-includes
13. make -w depend
14. make all

Results:
* /home/qsdl/include - C++ include files to compile quest
* /home/qsdl/lib - libraries used to compile and link quest
* /home/qsdl/[arch]/bin - quest executable (start with -h to see paramerter description)
* /home/qsdl/src - quest C++ source files

Install Example specifications:

1. Login as qsdl user
2. Be in /home/qsdl directory
3. git clone https://github.com/Diefenbruch/QSDL-SPECS specifications

Example (replace [arch] with your architecture directory):

1. Run /home/qsdl/bin/[arch]/quest -vwgmiyxpel srs.pr
2. switch to /home/qsdl/output/srs
3. make 
4. Run /home/qsdl/srs/obj/[arch]/srs -h    (help message)
5. Set environment variables for the experiment: source /home/qsdl/specifications/srs.sh
5. Simulation run: /home/qsdl/srs/obj/[arch]/srs -vys -e 1 -z 100
6. Validation run: /home/qsdl/srs/obj/[arch]/srs -vyr -e 1 -p 18 -t msc -u -i 1 -s bs
7. Verification run: /home/qsdl/srs/obj/[arch]/srs -vyv -e 1 -p 18 -t msc -u -i 1 -s fs -r "[]<>inUDCon"
8. Performance Evaluation & Visualization run: /home/qsdl/srs/obj/[arch]/srs -vy s -t pev -c srs.pev
