
ALL=UDPServer.class UDPClient.class TCPClient.class TCPServer.class

all: $(ALL)

.SUFFIXES: .java .class

.java.class:
	javac -deprecation $<

UDPServer.class: UDPServer.java
UDPClient.class: UDPClient.java

clean:
	rm -f $(ALL)
