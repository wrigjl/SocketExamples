import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UDPClient {

    public final static int SERVICE_PORT = 9999;

    public static void main(String[] args) {
	if (args.length != 1) {
	    System.out.println("provide hostname on command line");
	    return;
	}
	
	try {
	    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	    System.out.println("Give me a lowercase sentence: ");
	    byte[] message = br.readLine().getBytes();

	    DatagramSocket sock = new DatagramSocket();

	    InetAddress serverAddress = InetAddress.getByName(args[0]);

	    DatagramPacket sendingPacket = new DatagramPacket(message,
							      message.length,
							      serverAddress,
							      SERVICE_PORT);
	    sock.send(sendingPacket);

	    byte[] rxBuffer = new byte[1024];
	    DatagramPacket rxPacket = new DatagramPacket(rxBuffer,
							 rxBuffer.length);
	    sock.receive(rxPacket);
	    String receivedData = new String(rxPacket.getData());
	    System.out.println("received: " + receivedData);

	    sock.close();
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
