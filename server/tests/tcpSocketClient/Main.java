import java.io.*;
import java.net.*;

class Main {
	public static void main(String[] args) {
		try (
			Socket echoSocket = new Socket("localhost", 9999);
			PrintWriter out = new PrintWriter(echoSocket.getOutputStream(), true);
			BufferedReader in = new BufferedReader(new InputStreamReader(echoSocket.getInputStream()));
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in))
		) {
			System.out.println("Client connected!");
			String userInput;
			while ((userInput = stdIn.readLine()) != null) {
			    out.println(userInput);
			}
		} catch (Exception e) {
			System.out.println(e);
		}
	}
}
