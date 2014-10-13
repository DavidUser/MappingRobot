package Servidor.Model;

import java.util.Vector;
import Servidor.Model.Snapshot;

public class SnapshotCollectionMessage implements Message {
	public Vector<Snapshot> snapshotCollection = new Vector<Snapshot>();

	public list<Snapshot> getSnapshots() {
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
