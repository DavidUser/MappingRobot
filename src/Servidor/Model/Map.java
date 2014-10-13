package Servidor.Model;

import Embarcado.logic.Position;

//TODO improve class to set precision of a existing map without lose reads
public class Map {
	private int height, width;
	private int scale; //distance for a point to another growing in height or width, in centimeters

	public Map(int height, int width, int scale) {
		setHeight(height);
		setWidth(width);
		if (scale > 0)
			this.scale = scale; //TODO else throw a error
	}

	public void takeNewPoint(Position position, Snapshot snapshot) {
		throw new UnsupportedOperationException();
	}

	public void buildObjects() {
		throw new UnsupportedOperationException();
	}

	public void setHeight(int height) {
		this.height = height;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public int getHeight() {
		return this.height;
	}

	public int getWidth() {
		return this.width;
	}

	public int getScale() {
		return this.scale;
	}
}
