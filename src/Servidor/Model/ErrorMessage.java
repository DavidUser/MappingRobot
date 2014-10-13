package Servidor.Model;

public class ErrorMessage implements Message {

	public String getMessage() {
		throw new UnsupportedOperationException();
	}

	public int getLevel() {
		throw new UnsupportedOperationException();
	}

	public int getTypeID() {
		throw new UnsupportedOperationException();
	}

	public byte[] getBytes() {
		throw new UnsupportedOperationException();
	}

	public void build() {
		throw new UnsupportedOperationException();
	}
}