package Servidor.Model;

public interface Message {

	public int getTypeID();

	public byte[] getBytes();

	public void build();
}