import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;


public class UDPServer {

    public final static int SERVICE_PORT = 9999;

    public static void main(String[] args) {
	try {
	    DatagramSocket sock = new DatagramSocket(SERVICE_PORT);
		
	    byte[] rxBuffer = new byte[1024];
	    byte[] txBuffer = new byte[1024];
		
	    DatagramPacket inputPacket = new DatagramPacket(rxBuffer, rxBuffer.length);
		
	    System.out.println("Server ready, waiting for packets");

	    for (;;) {
		sock.receive(inputPacket);
		
		String receivedData = new String(inputPacket.getData());
		System.out.println("From client: " + receivedData);
		
		txBuffer = receivedData.toUpperCase().getBytes();
		
		// get client address and port
		InetAddress address = inputPacket.getAddress();
		int port = inputPacket.getPort();
		
		// generate response packet
		DatagramPacket outputPacket = new DatagramPacket(txBuffer, txBuffer.length, address, port);
		
		sock.send(outputPacket);
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
