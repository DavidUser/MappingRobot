package Servidor.Model;

public class Snapshot {
	private int[] measures; //TODO improve container to better space cost or performace cost, possible use a vector

	public Snapshot(int [] measures) { //expected 360 measures
		//TODO verify size of 360 measures else throw error
		this.measures = measures;
	}

	public int getDegreeMeasure(int measure) {
		throw new UnsupportedOperationException();
	}
}
