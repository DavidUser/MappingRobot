package Servidor.Model;

public class Communication {
	public CommunicationControl control;

	private void write(byte msg) {
		throw new UnsupportedOperationException();
	}

	private byte read() {
		throw new UnsupportedOperationException();
	}

	public void send(Message msg) {
		throw new UnsupportedOperationException();
	}

	public Message receive() {
		throw new UnsupportedOperationException();
	}
}
