
// A simple TCP client that sends a line of text to a server.

import java.io.BufferedReader;
//import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class TCPClient {

    public final static int SERVICE_PORT = 9999;

    public static void main(String[] args) {
	if (args.length != 1) {
	    System.out.println("provide hostname on command line");
	    return;
	}

	try {
	    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	    System.out.println("Give me a lowercase sentence: ");
	    String message = br.readLine();

	    // Connect to remote service
	    Socket socket = new Socket(args[0], SERVICE_PORT);

	    BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
	    PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);

	    // Send our line of text
	    writer.println(message);

	    // Read all of the data from the server
	    while ((message = reader.readLine()) != null) {
		System.out.println(message);
	    }

	    // close the socket
	    socket.close();
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
