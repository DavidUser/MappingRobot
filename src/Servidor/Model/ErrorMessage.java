package Servidor.Model;

public class ErrorMessage implements Message {
	private int level;
	private String message;

	public String getMessage() {
		return this.message;
	}

	public int getLevel() {
		return this.level;
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
