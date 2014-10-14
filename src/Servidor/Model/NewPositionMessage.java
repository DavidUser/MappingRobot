package Servidor.Model;

import Embarcado.logic.Position;

public class NewPositionMessage implements Message {
	public Position position;

	public Position getPosition() {
		return this.position;
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
