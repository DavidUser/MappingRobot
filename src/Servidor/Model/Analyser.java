package Servidor.Model;

import Embarcado.logic.Position;

public class Analyser {
	private Analyser instance;
	public Communication communication;

	private Analyser() {
		communication = new Communication();
	}

	public Analyser getInstance() {
		if (instance == null)
			instance = new Analyser();
		return instance;
	}

	public void takeDecision() {
		throw new UnsupportedOperationException();
	}

	private Position calculatesRoute() {
		throw new UnsupportedOperationException();
	}
}
