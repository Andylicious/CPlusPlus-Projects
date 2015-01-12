# $Id: Makefile,v 1.4 2013-07-30 18:36:31-07 - - $

# By default, gmake sets the variable ${CXX}=g++ on a Linux system.
# To use Sun Studio C++ (CC), set the variable CXX=CC, either on
# the command line when calling make, or in an environment variable.

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory

COMPILECPP  = g++ -g -O0 -Wall -Wextra -std=gnu++0x
MAKEDEPCPP  = g++ -MM

MODULES     = listmap trace util
CPPHEADER   = xless.h xpair.h ${MODULES:=.h} templates.tcc
CPPSOURCE   = ${MODULES:=.cpp} main.cpp
ALLCPPSRC   = ${CPPHEADER} ${CPPSOURCE}
OBJECTS     = ${CPPSOURCE:.cpp=.o}
EXECBIN     = keyvalue
OTHERS      = ${MKFILE} README
ALLSOURCES  = ${ALLCPPSRC} ${OTHERS}

LISTING     = Listing.ps
CLASS       = cmps109-wm.213
PROJECT     = asg4

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	cid + ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

clean :
	- rm ${OBJECTS} ${DEPFILE} core

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

submit : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}

dep : ${ALLCPPSRC}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif

