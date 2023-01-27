
// A simple TCP server that capitalizes a line of text.

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    public final static int SERVICE_PORT = 9999;

    public static void main(String[] args) {
	try {
	    // Create the listening socket
	    ServerSocket sockserver = new ServerSocket(SERVICE_PORT);

	    System.out.println("Server ready, waiting for connection");

	    for (;;) {
		// Wait for a connection
		Socket socket = sockserver.accept();

		// Handle this connection
		InputStream input = socket.getInputStream();
		BufferedReader reader = new BufferedReader(new InputStreamReader(input));

		OutputStream output = socket.getOutputStream();
		PrintWriter writer = new PrintWriter(output, true);

		String line = reader.readLine();
		writer.println(line.toUpperCase());

		// Close the connection
		socket.close();
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
