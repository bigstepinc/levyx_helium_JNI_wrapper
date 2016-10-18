#(C) Bigstep inc. Helium test code
JAVA_HOME=$(shell jrunscript -e 'java.lang.System.out.println(java.lang.System.getProperty("java.home"));')
CPPFLAGS=-g -fPIC -I$(JAVA_HOME)/../include/ -I$(JAVA_HOME)/../include/linux
LDFLAGS=-g 
LDLIBS=-lhe -lpthread

SRCS=helium_native_test.c
OBJS=$(subst .c,.o,$(SRCS))

libhelium.so: Helium.o
	g++ $(LDFLAGS) -shared -fPIC -o libhelium.so Helium.o $(LDLIBS) 

helium_native_test: $(OBJS)
	g++ $(LDFLAGS) -o helium_native_test helium_native_test.o $(LDLIBS) 

helium_native_test.o: helium_native_test.c
	g++ $(CPPFLAGS) -c helium_native_test.c

Helium.class: src/com/bigstep/Helium.java src/cz/adamh/utils/NativeUtils.java
	javac $^

Helium.h: Helium.class
	javah -cp . -jni com.bigstep.Helium

Helium.o: Helium.h Helium.cc
	g++ $(CPPFLAGS) -c Helium.cc

helium.jar: com/bigstep/Helium.class libhelium.so cz/adamh/utils/NativeUtils.class
	jar cf helium.jar $^

all: helium_native_test libhelium.so Helium.class helium.jar

clean: 
	rm -rf helium_native_test *.o *.class libhelium.so helium.jar
